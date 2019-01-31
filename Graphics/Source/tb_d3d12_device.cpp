#include "tb_d3d12_device.h"

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
                                                    (void**)(&dbgController));
    if (SUCCEEDED(HRDbgInterface)) {
      dbgController->EnableDebugLayer();

      // Enable additional debug layers.
      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
  }
#endif

  //Factory creation
  IDXGIFactory4* factory;

  HRESULT HRFactory = CreateDXGIFactory2(dxgiFactoryFlags,
                                         __uuidof(**(&factory)),
                                         (void**)(&factory));

  if (FAILED(HRFactory)) {
    throw std::exception();
  }

  //Create device
	if (m_useCPU) {
    IDXGIAdapter* warpAdapter;
    HRESULT HRWarpAdapter = factory->EnumWarpAdapter(__uuidof(**(&warpAdapter)),
                                                     (void**)(&warpAdapter));
    if (FAILED(HRWarpAdapter)) {
      throw std::exception();
    }

    HRESULT HRCreateDevice = D3D12CreateDevice(warpAdapter,
			                                         D3D_FEATURE_LEVEL_11_0,
                                               __uuidof(**(&m_device)),
                                               (void**)(&m_device));
    if (FAILED(HRCreateDevice)) {
      throw std::exception();
    }
    warpAdapter->Release();
	}
	else {
		IDXGIAdapter1* hardwareAdapter;
		GetHardwareAdapter(factory, &hardwareAdapter);

    HRESULT HRCreateDevice = D3D12CreateDevice(hardwareAdapter,
			                                         D3D_FEATURE_LEVEL_11_0,
                                               __uuidof(**(&m_device)),
                                               (void**)(&m_device));
    if (FAILED(HRCreateDevice)) {
      throw std::exception();
    }
    hardwareAdapter->Release();
	}

  //////////////////////////////////////////////////////////////////////////////

  // Describe and create the command queue.
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

  HRESULT HRCommand = m_device->CreateCommandQueue(&queueDesc,
                                                   __uuidof(**(&m_commandQueue)),
                                                   (void**)(&m_commandQueue));

  if (FAILED(HRCommand)) {
    throw std::exception();
  }

  // Describe and create the swap chain.
  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = FrameCount;
  swapChainDesc.Width = m_width;
  swapChainDesc.Height = m_height;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  IDXGISwapChain1* swapChain;
  // Swap chain needs the queue so that it can force a flush on it.
  factory->CreateSwapChain(m_commandQueue.get(), &swapChainDesc, &swapChain);
  HRESULT HRSwapChain = factory->CreateSwapChainForHwnd(m_commandQueue.get(),
                                                        Win32Application::GetHwnd(),
                                                        &swapChainDesc,
                                                        nullptr,
                                                        nullptr,
                                                        &swapChain);

  if (FAILED(HRSwapChain)) {
    throw std::exception();
  }

  // This sample does not support fullscreen transitions.
  ThrowIfFailed(factory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

  ThrowIfFailed(swapChain.As(&m_swapChain));
  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

  //////////////////////////////////////////////////////////////////////////////

	//Create descriptor heaps.
	{
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT HRDescriptorHeap = m_device->CreateDescriptorHeap(&rtvHeapDesc,
                                                              __uuidof(**(&m_rtvHeap)),
                                                              reinterpret_cast<void**>(&m_rtvHeap));

    if (FAILED(HRDescriptorHeap)) {
      std::exception();
    }

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

  //////////////////////////////////////////////////////////////////////////////

	//Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UInt32 n = 0; n < FrameCount; ++n) {
      HRESULT HRBuffer = m_swapChain->GetBuffer(n,
                                                __uuidof(**(&m_renderTargets[n])),
                                                reinterpret_cast<void**>(&m_renderTargets[n]));
      if (FAILED(HRBuffer)) {
        std::exception();
      }

			m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);
		}
	}

  HRESULT HRCommandAllocator = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                                __uuidof(**(&m_commandAllocator)),
                                                                reinterpret_cast<void**>(&m_commandAllocator));
  if (FAILED(HRCommandAllocator)) {
    std::exception();
  }
}

void
DeviceD3D12::GetHardwareAdapter(IDXGIFactory2* pFactory,
                                IDXGIAdapter1** ppAdapter) {
  
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