#include "tb_graphics_api.h"

#include <tb_types.h>
#include <tb_string_conversions.h>

//#include "tb_device.h"
//#include "tb_swap_chain.h"
//#include "tb_render_target.h"
//
//#include "tb_d3d12_device.h"
//#include "tb_d3d12_shader.h"

#include <D3Dcompiler.h>

namespace toyboxSDK {

GraphicsAPI::GraphicsAPI() {
}

GraphicsAPI::~GraphicsAPI() {
}


void
GraphicsAPI::init(UInt32 w,
                  UInt32 h,
                  void* hwnd,
                  TB_GRAPHICS_API::E api) {
  CreateDevice();
  CreateCommandQueue();
  CreateSwapChainCommanAllocators();
  CreateFences();
  
  m_w = w;
  m_h = h;
  m_hwnd = hwnd;

  CreateSwapChain();

  //PIXBeginEvent(m_commandQueue, 0, L"Setup");
  const UInt32 PIX_EVENT_UNICODE_VERSION = 0;
  m_commandQueue->BeginEvent(PIX_EVENT_UNICODE_VERSION, _T("Setup"), (wcslen(_T("Setup")) + 1) * sizeof(_T("Setup")));
  {
    CreateCommandList();
    CreateShaders();
    CreateConstantBuffer();
  }
}

void
GraphicsAPI::onStartUp() {
  m_bUseCPU = false;

  m_iFrameBuffers = 2;
  m_iCurrentFrame = 0;
  m_fenceValues.resize(m_iFrameBuffers);
}

void
GraphicsAPI::onShutDown() {
}

void
GraphicsAPI::CreateDevice() {
  IDXGIFactory4* factory = GetFactory();

  //Create device
	if (m_bUseCPU) {
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
}

void
GraphicsAPI::CreateCommandQueue() {
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
}

void
GraphicsAPI::CreateSwapChainCommanAllocators() {
  m_commandAllocators.resize(m_iFrameBuffers);

  for (UInt32 n = 0; n < m_iFrameBuffers; ++n) {
    HRESULT HRCommandAlloc = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, 
                                                              __uuidof(**(&m_commandAllocators[n])),
                                                              (void**)(&m_commandAllocators[n]));

    if (FAILED(HRCommandAlloc)) {
      std::exception();
    }
  }
}

void
GraphicsAPI::CreateFences() {
  // Create synchronization objects.
  HRESULT HRFence = m_device->CreateFence(m_fenceValues[m_iCurrentFrame],
                                          D3D12_FENCE_FLAG_NONE,
                                          __uuidof(**(&m_fence)),
                                          (void**)(&m_fence));

  if (FAILED(HRFence)) {
    std::exception();
  }

  m_fenceValues[m_iCurrentFrame]++;

  m_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
}

void
GraphicsAPI::CreateSwapChain() {
  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

  swapChainDesc.Width = m_w;
  swapChainDesc.Height = m_h;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //Common swap chain format.
  swapChainDesc.Stereo = false;
  swapChainDesc.SampleDesc.Count = 1; //Don't use multi-sampling.
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.BufferCount = m_iFrameBuffers; //Use double-buffering to minimize latency.
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; //All Windows Store apps must use this SwapEffect.
  swapChainDesc.Flags = 0;
  swapChainDesc.Scaling = DXGI_SCALING_NONE;
  swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

  IDXGIFactory4* factory = GetFactory();

  HRESULT HRSwapChain = factory->CreateSwapChainForHwnd(m_commandQueue,
                                                        static_cast<HWND>(m_hwnd),
  	            	                                      &swapChainDesc,
  	            	                                      nullptr,
  	            	                                      nullptr,
  	            	                                      &m_swapChain);
  
  if (FAILED(HRSwapChain)) {
    std::exception();
  }

  factory->Release();

  //Swapchain RenderTargets
  {
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = m_iFrameBuffers;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT HRHeap = m_device->CreateDescriptorHeap(&desc,
                                                    __uuidof(**(&m_rtvHeap)),
                                                    (void**)(&m_rtvHeap));

    if (FAILED(HRHeap)) {
      std::exception();
    }

    m_rtvHeap->SetName(L"RTV Descriptor Heap");

    for (UInt32 n = 0; n < m_iFrameBuffers; ++n) {
      m_fenceValues[n] = m_fenceValues[m_iCurrentFrame];
    }

    m_iCurrentFrame = 0;

    D3D12_CPU_DESCRIPTOR_HANDLE hCPUHeapHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    m_renderTargets.resize(m_iFrameBuffers);
    for (UInt32 n = 0; n < m_iFrameBuffers; ++n) {
      D3D12_CPU_DESCRIPTOR_HANDLE handle;
      handle.ptr = hCPUHeapHandle.ptr + (m_rtvDescriptorSize * n);
      HRESULT HRRTV = m_swapChain->GetBuffer(n,
                                             __uuidof(**(&m_renderTargets[n])),
                                             (void**)(&m_renderTargets[n]));

      if (FAILED(HRRTV)) {
        std::exception();
      }

      m_device->CreateRenderTargetView(m_renderTargets[n], nullptr, handle);

      WString name;
      name = _T("RenderTarget") + StringConversion::toWString(n);

      m_renderTargets[n]->SetName(name.c_str());
    }
  }

  //Set the 3D rendering viewport to target the entire window.
  m_viewport = { 0.0f,
                 0.0f,
                 static_cast<float>(m_w),
                 static_cast<float>(m_h),
                 0.0f,
                 1.0f };
}

void
GraphicsAPI::CreateCommandList() {
  HRESULT HRCommandList = m_device->CreateCommandList(0,
                                                      D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                      m_commandAllocators[m_iCurrentFrame],
                                                      nullptr,
                                                      __uuidof(**(&m_commandList)),
                                                      (void**)(&m_commandList));

  if (FAILED(HRCommandList)) {
    std::exception();
  }
}

void
GraphicsAPI::CreateShaders() {
  CompileShader(_T("D:\\Users\\F_A_R\\sources\\repos\\Toybox\\Bin\\x64\\Resources\\Shaders\\GBuffer_vs.hlsl"),
                TB_SHADER_TYPE::E::kVertex,
                &GBufferVSShaderBlob,
                &GBufferVSBytecodePtr,
                &GBufferVSbytecodeSz);
  CompileShader(_T("D:\\Users\\F_A_R\\sources\\repos\\Toybox\\Bin\\x64\\Resources\\Shaders\\GBuffer_ps.hlsl"),
                TB_SHADER_TYPE::E::kFragment,
                &GBufferPSShaderBlob,
                &GBufferPSBytecodePtr,
                &GBufferPSbytecodeSz);
}

void
GraphicsAPI::CreateConstantBuffer() {
  D3D12_HEAP_PROPERTIES heapProperty;
  heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
  heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  heapProperty.CreationNodeMask = 1;
  heapProperty.VisibleNodeMask = 1;

  D3D12_RESOURCE_DESC resourceDesc;
  ZeroMemory(&resourceDesc, sizeof(resourceDesc));
  resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  resourceDesc.Alignment = 0;
  resourceDesc.SampleDesc.Count = 1;
  resourceDesc.SampleDesc.Quality = 0;
  resourceDesc.MipLevels = 1;
  resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
  resourceDesc.DepthOrArraySize = 1;
  resourceDesc.Width = sizeof(CBuffer);
  resourceDesc.Height = 1;
  resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

  HRESULT HRCBCR = m_device->CreateCommittedResource(&heapProperty,
                                                     D3D12_HEAP_FLAG_NONE,
                                                     &resourceDesc,
                                                     D3D12_RESOURCE_STATE_GENERIC_READ,
                                                     nullptr,
                                                     __uuidof(**(&m_CB)),
                                                     (void**)(&m_CB));

  if (FAILED(HRCBCR)) {
    std::exception();
  }
}

void
GraphicsAPI::CompileShader(WString filepath,
                           TB_SHADER_TYPE::E type,
                           ID3DBlob** shaderBlobOut,
                           void** bytecodePtrOut,
                           SizeT* bytecodeSzOut) {
#if TB_DEBUG_MODE
  // Enable better shader debugging with the graphics debugging tools.
  UInt32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  UInt32 compileFlags = 0;
#endif
  String shaderModel;
  switch (type)
  {
  case toyboxSDK::TB_SHADER_TYPE::E::kVertex:
    shaderModel = "vs_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kHull:
    shaderModel = "hs_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kTeselation:
    exit(666); //This doesn't exist in directx.
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kDomain:
    shaderModel = "ds_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kGeometry:
    shaderModel = "gs_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kFragment:
    shaderModel = "ps_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kMesh:
    exit(666); //This doesn't exists yet in directx.
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kRaytracing:
    exit(666); //This isn't a shader yet.
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kCompute:
    shaderModel = "cs_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kTexture:
    exit(666); //This never has existed.
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kNone:
    break;
  default:
    break;
  }

  ID3DBlob* errorBlob = nullptr;

  HRESULT HRShader = D3DCompileFromFile(filepath.c_str(),
                                        nullptr,
                                        D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                        "main",
                                        shaderModel.c_str(),
                                        compileFlags,
                                        0,
                                        &(*shaderBlobOut),
                                        &errorBlob);

  if (FAILED(HRShader)) {
    std::exception();
  }

  (*bytecodePtrOut) = (*shaderBlobOut)->GetBufferPointer();
  (*bytecodeSzOut) = (*shaderBlobOut)->GetBufferSize();
}

IDXGIFactory4*
GraphicsAPI::GetFactory(UInt32 flags) {
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
GraphicsAPI::GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter) {
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