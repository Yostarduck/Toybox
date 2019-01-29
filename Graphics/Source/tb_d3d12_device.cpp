#include "tb_d3d12_device.h"
#include <d3d12.h>
#include <dxgi1_4.h>

namespace toyboxSDK {

DeviceD3D12::DeviceD3D12() {
}

DeviceD3D12::~DeviceD3D12() {
}

void
DeviceD3D12::CreateDevice() {
  UInt32 dxgiFactoryFlags = 0;

#if TB_DEBUG_MODE
  //Enable the debug layer (requires the Graphics Tools "optional feature").
  //NOTE: Enabling the debug layer after device creation will invalidate the active device.
  {
    std::unique_ptr<ID3D12Debug> dbgController;
    HRESULT HRDbgInterface = D3D12GetDebugInterface(__uuidof(**(&dbgController)),
                                                    reinterpret_cast<void**>(&dbgController));
    if (SUCCEEDED(HRDbgInterface)) {
      dbgController->EnableDebugLayer();

      // Enable additional debug layers.
      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
  }
#endif

  //Factory creation
  std::unique_ptr<IDXGIFactory4> factory;

  HRESULT HRFactory = CreateDXGIFactory2(dxgiFactoryFlags,
                                         __uuidof(**(&factory)),
                                         reinterpret_cast<void**>(&factory));

  if (FAILED(HRFactory)) {
    throw std::exception();
  }

  //Create device
	if (m_useCPU) {
    std::unique_ptr<IDXGIAdapter> warpAdapter;
    HRESULT HRWarpAdapter = factory->EnumWarpAdapter(__uuidof(**(&warpAdapter)),
                                                     reinterpret_cast<void**>(&warpAdapter));
    if (FAILED(HRWarpAdapter)) {
      throw std::exception();
    }

    HRESULT HRCreateDevice = D3D12CreateDevice(warpAdapter.get(),
			                                         D3D_FEATURE_LEVEL_11_0,
                                               __uuidof(&m_device),
                                               reinterpret_cast<void**>(&m_device));
    if (FAILED(HRCreateDevice)) {
      throw std::exception();
    }
	}
	else {
		std::unique_ptr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(factory.get(), &hardwareAdapter);

    HRESULT HRCreateDevice = D3D12CreateDevice(hardwareAdapter.get(),
			                                         D3D_FEATURE_LEVEL_11_0,
                                               __uuidof(&m_device),
                                               reinterpret_cast<void**>(&m_device));
    if (FAILED(HRCreateDevice)) {
      throw std::exception();
    }
	}

  //////////////////////////////////////////////////////////////////////////////

	//Create command queue. (Only for the swap chain)
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

  HRESULT HRCommand = m_device->CreateCommandQueue(&queueDesc,
                                                   __uuidof(&m_commandQueue),
                                                   reinterpret_cast<void**>(&m_commandQueue));

  if (FAILED(HRCommand)) {
    throw std::exception();
  }

	//Create swap chain.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = m_width;
	swapChainDesc.Height = m_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

  std::unique_ptr<IDXGISwapChain1> swapChain;
  HRESULT HRSwapChain = factory->CreateSwapChainForHwnd(m_commandQueue.get(),		// Swap chain needs the queue so that it can force a flush on it.
		                                                    Win32Application::GetHwnd(),
		                                                    &swapChainDesc,
		                                                    nullptr,
		                                                    nullptr,
		                                                    reinterpret_cast<IDXGISwapChain1**>(&swapChain));

  if (FAILED(HRSwapChain)) {
    std::exception();
  }

  //////////////////////////////////////////////////////////////////////////////

	// This sample does not support fullscreen transitions.
  HRESULT HRWndAssociation = factory->MakeWindowAssociation(Win32Application::GetHwnd(),
                                                            DXGI_MWA_NO_ALT_ENTER);

  if (FAILED(HRWndAssociation)) {
    std::exception();
  }

  HRESULT HRQueryInterface = swapChain->QueryInterface(__uuidof(&m_swapChain),
                                                       reinterpret_cast<void**>(&m_swapChain));
  if (FAILED(HRQueryInterface)) {
    std::exception();
  }

  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	//Create descriptor heaps.
	{
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	//Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UINT n = 0; n < FrameCount; n++)
		{
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);
		}
	}

	ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                 IID_PPV_ARGS(&m_commandAllocator)));
}

void
DeviceD3D12::GetHardwareAdapter(IDXGIFactory2* pFactory,
                                std::unique_ptr<IDXGIAdapter1>* ppAdapter) {
  
	std::unique_ptr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UInt32 adapterIndex = 0;
       DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex,
                                                       reinterpret_cast<IDXGIAdapter1**>(&adapter));
       ++adapterIndex) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.get(),
                                    D3D_FEATURE_LEVEL_11_0,
                                    _uuidof(ID3D12Device),
                                    nullptr))) {
			break;
		}
	}

	*ppAdapter = std::move(adapter);
}

}