#include "tb_d3d12_device.h"
#include "tb_d3d12_swap_chain.h"

namespace toyboxSDK {

DeviceD3D12::DeviceD3D12() {
}

DeviceD3D12::~DeviceD3D12() {
}

void
DeviceD3D12::CreateDevice() {
  IDXGIFactory4* factory = GetFactory();

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

  factory->Release();

  //Create the command queue.
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

  HRESULT HRCommandQueue = m_device->CreateCommandQueue(&queueDesc,
                                                        __uuidof(**(&m_commandQueue)),
                                                        (void**)(&m_commandQueue));

  if (FAILED(HRCommandQueue)) {
    std::exception();
  }

  //Create command allocator
  HRESULT HRCommandAllocator = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                                __uuidof(**(&m_commandAlloc)),
                                                                (void**)(&m_commandAlloc));

  if (FAILED(HRCommandAllocator)) {
    std::exception();
  }
}

SwapChain*
DeviceD3D12::CreateSwapChain(const SwapChainDesc& desc, void* hwnd) const {
  SwapChain* swapChain = new SwapChainD3D12;
  swapChain->create(*this, desc, hwnd);
  return swapChain;
}

IDXGIFactory4*
DeviceD3D12::GetFactory(UInt32 flags) const {
  UInt32 dxgiFactoryFlags = flags;

#if TB_DEBUG_MODE
  // Enable the debug layer (requires the Graphics Tools "optional feature").
  //NOTE: Enabling the debug layer after device creation will invalidate the active device.
  {
    ID3D12Debug* dbgController;
    HRESULT HRDbgInterface = D3D12GetDebugInterface(__uuidof(**(&dbgController)),
                                                    (void**)(&dbgController));
    if (SUCCEEDED(HRDbgInterface)) {
      dbgController->EnableDebugLayer();

      // Enable additional debug layers.
      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }

    dbgController->Release();
  }
#endif

  IDXGIFactory4* factoryOut;
  HRESULT HRFactory = CreateDXGIFactory2(dxgiFactoryFlags,
                                         __uuidof(**(&factoryOut)),
                                         (void**)(&factoryOut));

  if (FAILED(HRFactory)) {
    throw std::exception();
  }

  return factoryOut;
}

void
DeviceD3D12::GetHardwareAdapter(IDXGIFactory4* pFactory,
                                IDXGIAdapter1** ppAdapter) {
  
	IDXGIAdapter1* adapter;
	*ppAdapter = nullptr;

	for (UInt32 adapterIndex = 0;
       DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex,
                                                       (IDXGIAdapter1**)(&adapter));
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
		if (SUCCEEDED(D3D12CreateDevice(adapter,
                                    D3D_FEATURE_LEVEL_11_0,
                                    _uuidof(ID3D12Device),
                                    nullptr))) {
			break;
		}
	}

	*ppAdapter = std::move(adapter);
}

}