#include "tb_graphics_api.h"

#include <tb_file_system.h>
#include <tb_string_conversions.h>
#include <tb_types.h>

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

  const UInt32 PIX_EVENT_UNICODE_VERSION = 0;
  m_commandQueue->BeginEvent(PIX_EVENT_UNICODE_VERSION,
                             _T("Setup"),
                             (wcslen(_T("Setup")) + 1) * sizeof(_T("Setup")));
  {
    CreateCommandList();
    CreateShaders();
    CreateConstantBuffer();
    CreateQuadVB();
    CreateQuadIB();
    CreateShaderHeap();
    CreateGBufferRootSignature();
    CreateForwardRootSignature();
    CreateGPSOGBuffer();
    CreateGPSOForward();
    CreateDSV();
    CreateRTV();

    HRESULT HRCommandClose = m_commandList->Close();
    if (FAILED(HRCommandClose)) {
      throw std::exception();
    }

    ID3D12CommandList* ppCommandLists[] = { m_commandList };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    WaitForGPU();
  }
  m_commandQueue->EndEvent();
}

void
GraphicsAPI::ApplyGBuffer() {
  const Int32 RTSize = 4;
  float mClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

  for (Int32 i = 0; i < RTSize; ++i) {
    AddResourceBarrier(m_commandList, m_RTTexture[i], D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
  }

  AddResourceBarrier(m_commandList, m_DSTexture, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

  ID3D12DescriptorHeap* ppHeaps[1] = { m_ShaderDHPtr };

  m_commandList->SetPipelineState(m_GPSOGBuffer);

  for (Int32 i = 0; i < RTSize; ++i) {
    D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorhandle;
    CPUDescriptorhandle.ptr = m_RTCPUHeapStartHandle.ptr +
                              (static_cast<SizeT>(m_SHandleIncrementSize) * i);

    m_commandList->ClearRenderTargetView(CPUDescriptorhandle, mClearColor, 0, nullptr);
  }

  m_commandList->ClearDepthStencilView(m_DSCPUHeapStartHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0xff, 0, nullptr);

  D3D12_GPU_DESCRIPTOR_HANDLE handleOffset;
  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr;

  m_commandList->OMSetRenderTargets(RTSize, &m_RTCPUHeapStartHandle, true, &m_DSCPUHeapStartHandle);
  m_commandList->SetDescriptorHeaps(1, ppHeaps);
  m_commandList->SetGraphicsRootSignature(m_GBufferRootSignature);
  m_commandList->SetGraphicsRootDescriptorTable(0, m_ShaderGPUHeapStartHandle);

  m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  m_commandList->IASetIndexBuffer(&m_QuadIBView);
  m_commandList->IASetVertexBuffers(0, 1, &m_QuadVBView);
  m_commandList->DrawInstanced(4, 1, 0, 0);
}

void
GraphicsAPI::ApplyForward() {
  const Int32 RTSize = 4;
  float mClearColor[4] = { 0.0, 0.0f, 0.0f, 1.0f };

  for (Int32 i = 0; i < RTSize; ++i) {
    AddResourceBarrier(m_commandList, m_RTTexture[i], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
  }

  AddResourceBarrier(m_commandList, m_DSTexture, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

  ID3D12DescriptorHeap* ppHeaps[1] = { m_ShaderDHPtr };

  m_commandList->SetPipelineState(m_GPSOForward);

  D3D12_GPU_DESCRIPTOR_HANDLE handleOffset;

  m_commandList->OMSetRenderTargets(1, &GetRenderTargetView(), true, nullptr);
  m_commandList->SetDescriptorHeaps(1, ppHeaps);
  m_commandList->SetGraphicsRootSignature(m_ForwardRootSignature);

  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 1); //RTs
  m_commandList->SetGraphicsRootDescriptorTable(0, handleOffset);
  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 5); //Ds
  m_commandList->SetGraphicsRootDescriptorTable(1, handleOffset);

  m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  m_commandList->IASetIndexBuffer(&m_QuadIBView);
  m_commandList->IASetVertexBuffers(0, 1, &m_QuadVBView);
  m_commandList->DrawInstanced(4, 1, 0, 0);
}

void
GraphicsAPI::PresentFrame() {
  // The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT HRPresent = m_swapChain->Present(1, 0);

	// If the device was removed either by a disconnection or a driver upgrade, we 
	// must recreate all device resources.
	if (HRPresent == DXGI_ERROR_DEVICE_REMOVED || HRPresent == DXGI_ERROR_DEVICE_RESET) {
		//m_deviceRemoved = true;
    exit(666);
	}
	else {
    if (FAILED(HRPresent)) {
      throw std::exception();
    }

    {
      // Schedule a Signal command in the queue.
      const UInt64 currentFenceValue = m_fenceValues[m_iCurrentFrame];
      HRESULT HRSignal = m_commandQueue->Signal(m_fence, currentFenceValue);
      if (FAILED(HRSignal)) {
        throw std::exception();
      }

      // Advance the frame index.
      m_iCurrentFrame = (m_iCurrentFrame + 1) % m_iFrameBuffers;

      // Check to see if the next frame is ready to start.
      if (m_fence->GetCompletedValue() < m_fenceValues[m_iCurrentFrame]) {
        HRESULT HRSEC = m_fence->SetEventOnCompletion(m_fenceValues[m_iCurrentFrame], m_fenceEvent);
        if (FAILED(HRSEC)) {
          throw std::exception();
        }
        WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
      }

      // Set the fence value for the next frame.
      m_fenceValues[m_iCurrentFrame] = currentFenceValue + 1;
    }
	}
}

void
GraphicsAPI::WaitForGPU() {
  // Schedule a Signal command in the queue.
  HRESULT HRCommandSignal = m_commandQueue->Signal(m_fence, m_fenceValues[m_iCurrentFrame]);
  if (FAILED(HRCommandSignal)) {
    throw std::exception();
  }

  // Wait until the fence has been crossed.
  HRESULT HRSEOC = m_fence->SetEventOnCompletion(m_fenceValues[m_iCurrentFrame], m_fenceEvent);
  if (FAILED(HRSEOC)) {
    throw std::exception();
  }
  WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

  // Increment the fence value for the current frame.
  m_fenceValues[m_iCurrentFrame]++;
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
    throw std::exception();
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
      throw std::exception();
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
    throw std::exception();
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
    throw std::exception();
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
      throw std::exception();
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
        throw std::exception();
      }

      m_device->CreateRenderTargetView(m_renderTargets[n], nullptr, handle);

      WString name = _T("SwapChain RT(") + StringConversion::toWString(n) + _T(")");

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
    throw std::exception();
  }
}

void
GraphicsAPI::CreateShaders() {
  FileSystem FSys;
  TString path = FSys.GetWorkingPath();

  CompileShader(path + _T("Resources\\Shaders\\GBuffer_vs.hlsl"),
                TB_SHADER_TYPE::E::kVertex,
                &GBufferVSShaderBlob,
                &GBufferVSBytecodePtr,
                &GBufferVSbytecodeSz);
  CompileShader(path + _T("Resources\\Shaders\\GBuffer_ps.hlsl"),
                TB_SHADER_TYPE::E::kFragment,
                &GBufferPSShaderBlob,
                &GBufferPSBytecodePtr,
                &GBufferPSbytecodeSz);

  CompileShader(path + _T("Resources\\Shaders\\Forward_vs.hlsl"),
                TB_SHADER_TYPE::E::kVertex,
                &ForwardVSShaderBlob,
                &ForwardVSBytecodePtr,
                &ForwardVSbytecodeSz);
  CompileShader(path + _T("Resources\\Shaders\\Forward_ps.hlsl"),
                TB_SHADER_TYPE::E::kFragment,
                &ForwardPSShaderBlob,
                &ForwardPSBytecodePtr,
                &ForwardPSbytecodeSz);
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
  //resourceDesc.Width = sizeof(CBuffer);
  resourceDesc.Width = 256;
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
    throw std::exception();
  }
}

void
GraphicsAPI::CreateQuadVB() {
  ScreenQuadVertex QuadVerts[] =
  {
		{ { -1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f,  1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
	  { {  1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }
	};

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
	resourceDesc.Width = sizeof(QuadVerts);
	resourceDesc.Height = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT HRVBCR = m_device->CreateCommittedResource(&heapProperty,
                                                     D3D12_HEAP_FLAG_NONE,
                                                     &resourceDesc,
                                                     D3D12_RESOURCE_STATE_GENERIC_READ,
                                                     nullptr,
                                                     __uuidof(**(&m_QuadVB)),
                                                     (void**)(&m_QuadVB));
	
  if (FAILED(HRVBCR)) {
    throw std::exception();
  }

	UInt8* VBDataBegin;
	HRESULT HRMapVB = m_QuadVB->Map(0, nullptr, reinterpret_cast<void**>(&VBDataBegin));
  if (FAILED(HRMapVB)) {
    throw std::exception();
  }
	memcpy(VBDataBegin, QuadVerts, sizeof(QuadVerts));
	m_QuadVB->Unmap(0, nullptr);

	m_QuadVBView.BufferLocation = m_QuadVB->GetGPUVirtualAddress();
	m_QuadVBView.StrideInBytes = sizeof(ScreenQuadVertex);
	m_QuadVBView.SizeInBytes = sizeof(QuadVerts);
}

void
GraphicsAPI::CreateQuadIB() {
  std::vector<Int32> indexes;
  indexes.resize(6);
  indexes[0] = 2;
  indexes[1] = 1;
  indexes[2] = 0;
  indexes[3] = 3;
  indexes[4] = 1;
  indexes[5] = 2;

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
  resourceDesc.Width = sizeof(UInt32) * 6;
  resourceDesc.Height = 1;
  resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

  HRESULT HRIDCR = m_device->CreateCommittedResource(&heapProperty,
                                                     D3D12_HEAP_FLAG_NONE,
                                                     &resourceDesc,
                                                     D3D12_RESOURCE_STATE_GENERIC_READ,
                                                     nullptr,
                                                     __uuidof(**(&m_QuadIB)),
                                                     (void**)(&m_QuadIB));

  if (FAILED(HRIDCR)) {
    throw std::exception();
  }

  UInt8* IBDataBegin;
  HRESULT HRIB = m_QuadIB->Map(0, nullptr, reinterpret_cast<void**>(&IBDataBegin));
  if (FAILED(HRIB)) {
    throw std::exception();
  }
  memcpy(IBDataBegin, &indexes[0], sizeof(UInt32) * indexes.size());
  m_QuadIB->Unmap(0, nullptr);

  m_QuadIBView.BufferLocation = m_QuadIB->GetGPUVirtualAddress();
  m_QuadIBView.Format = DXGI_FORMAT_R32_UINT;
  m_QuadIBView.SizeInBytes = sizeof(UInt32) * indexes.size();
}

void
GraphicsAPI::CreateShaderHeap() {
  //m_cbvsrvHeap.Create(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10, true);

  //CBv = 1 (0)
  //RTv = 4 (1 - 4)
  //DSv = 1 (5)

  //Start of create GPU Heap
  D3D12_DESCRIPTOR_HEAP_DESC GPUHDesc;
  GPUHDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  GPUHDesc.NodeMask = 0;
  GPUHDesc.NumDescriptors = 6;
  GPUHDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;


  HRESULT hr = m_device->CreateDescriptorHeap(&GPUHDesc,
                                              __uuidof(ID3D12DescriptorHeap),
                                              (void**)&m_ShaderDHPtr);
  if (FAILED(hr)) {
    throw std::exception();
  }

  m_ShaderDHPtr->SetName(_T("Shader heap"));

  m_ShaderCPUHeapStartHandle = m_ShaderDHPtr->GetCPUDescriptorHandleForHeapStart();
  m_ShaderGPUHeapStartHandle = m_ShaderDHPtr->GetGPUDescriptorHandleForHeapStart();

  m_SHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(GPUHDesc.Type);
  //End of create of GPU Heap

  D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
  CPUHandle.ptr = m_ShaderCPUHeapStartHandle.ptr;

  D3D12_CONSTANT_BUFFER_VIEW_DESC	descBuffer;

  //Camera CBV
  descBuffer.BufferLocation = m_CB->GetGPUVirtualAddress();
  //Constant buffer must be larger than 256 bytes
  //descBuffer.SizeInBytes = sizeof(CBuffer); //Memory should be aligned
  //descBuffer.SizeInBytes = (sizeof(CBuffer) + 255) & ~255; //Aligns memory to 256
  descBuffer.SizeInBytes = 256; //Aligns memory to 256

  m_device->CreateConstantBufferView(&descBuffer, CPUHandle);
}

void
GraphicsAPI::CreateGBufferRootSignature() {
  //Init descriptor tables

  D3D12_DESCRIPTOR_RANGE range[1];
  //CBV (The camera one)
  range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
  range[0].NumDescriptors = 1;
  range[0].BaseShaderRegister = 0;
  range[0].RegisterSpace = 0;
  range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

  D3D12_ROOT_PARAMETER rootParameters[1];
  rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
  rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
  rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
  rootParameters[0].DescriptorTable.pDescriptorRanges = &range[0];

  D3D12_ROOT_SIGNATURE_DESC descRootSignature;
  descRootSignature.NumParameters = 1;
  descRootSignature.pParameters = &rootParameters[0];
  descRootSignature.NumStaticSamplers = 0;
  descRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

  ID3DBlob* rootSigBlob;
  ID3DBlob* errorBlob;

  HRESULT HRRootSerialize = D3D12SerializeRootSignature(&descRootSignature,
                                                        D3D_ROOT_SIGNATURE_VERSION_1,
                                                        &rootSigBlob,
                                                        &errorBlob);

  if (FAILED(HRRootSerialize)) {
    throw std::exception();
  }

  HRESULT HRRootSignature = m_device->CreateRootSignature(0,
                                                          rootSigBlob->GetBufferPointer(),
                                                          rootSigBlob->GetBufferSize(),
                                                          __uuidof(**(&m_GBufferRootSignature)),
                                                          (void**)(&m_GBufferRootSignature));
  if (FAILED(HRRootSignature)) {
    throw std::exception();
  }
}


void
GraphicsAPI::CreateForwardRootSignature() {
  //Init descriptor tables

  D3D12_DESCRIPTOR_RANGE range[2];

  //SRV (GBuffer RTs)
  range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  range[0].NumDescriptors = 4;
  range[0].BaseShaderRegister = 0;
  range[0].RegisterSpace = 0;
  range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
  //SRV (The depth stencil)
  range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  range[1].NumDescriptors = 1;
  range[1].BaseShaderRegister = 4;
  range[1].RegisterSpace = 0;
  range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


  D3D12_ROOT_PARAMETER rootParameters[2];
  //
  rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
  rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
  rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
  rootParameters[0].DescriptorTable.pDescriptorRanges = &range[0];
  //
  rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
  rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
  rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
  rootParameters[1].DescriptorTable.pDescriptorRanges = &range[1];

  D3D12_STATIC_SAMPLER_DESC StaticSamplers;
  {
    StaticSamplers.ShaderRegister = 0;
    StaticSamplers.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    StaticSamplers.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    StaticSamplers.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    StaticSamplers.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    StaticSamplers.MipLODBias = 0;
    StaticSamplers.MaxAnisotropy = 16;
    StaticSamplers.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    StaticSamplers.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
    StaticSamplers.MinLOD = 0.f;
    StaticSamplers.MaxLOD = D3D12_FLOAT32_MAX;
    StaticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    StaticSamplers.RegisterSpace = 0;
  }

  D3D12_ROOT_SIGNATURE_DESC descRootSignature;
  //descRootSignature.Init(3, rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
  descRootSignature.NumParameters = 2;
  descRootSignature.pParameters = &rootParameters[0];
  descRootSignature.NumStaticSamplers = 1;
  descRootSignature.pStaticSamplers = &StaticSamplers;
  descRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

  ID3DBlob* rootSigBlob;
  ID3DBlob* errorBlob;

  HRESULT HRRootSerialize = D3D12SerializeRootSignature(&descRootSignature,
                                                        D3D_ROOT_SIGNATURE_VERSION_1,
                                                        &rootSigBlob,
                                                        &errorBlob);

  if (FAILED(HRRootSerialize)) {
    throw std::exception();
  }

  HRESULT HRRootSignature = m_device->CreateRootSignature(0,
                                                          rootSigBlob->GetBufferPointer(),
                                                          rootSigBlob->GetBufferSize(),
                                                          __uuidof(**(&m_ForwardRootSignature)),
                                                          (void**)(&m_ForwardRootSignature));
  if (FAILED(HRRootSignature)) {
    throw std::exception();
  }
}

static D3D12_INPUT_ELEMENT_DESC VertexDesc[] = {
  { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
};

void
GraphicsAPI::CreateGPSOGBuffer() {
  D3D12_DEPTH_STENCIL_DESC DepthStencilDefault;
  {
    DepthStencilDefault.DepthEnable = TRUE;
    DepthStencilDefault.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    DepthStencilDefault.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    DepthStencilDefault.StencilEnable = FALSE;
    DepthStencilDefault.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    DepthStencilDefault.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
    {
      D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS
    };
    DepthStencilDefault.FrontFace = defaultStencilOp;
    DepthStencilDefault.BackFace = defaultStencilOp;
  }
  D3D12_BLEND_DESC BlendStateDefault;
  {
    BlendStateDefault.AlphaToCoverageEnable = FALSE;
    BlendStateDefault.IndependentBlendEnable = FALSE;
    const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
    {
      FALSE,FALSE,
      D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
      D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
      D3D12_LOGIC_OP_NOOP,
      D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    for (UInt32 i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
      BlendStateDefault.RenderTarget[i] = defaultRenderTargetBlendDesc;
    }
  }
  D3D12_RASTERIZER_DESC RasterizerDefault;
  {
    RasterizerDefault.FillMode = D3D12_FILL_MODE_SOLID;
    RasterizerDefault.CullMode = D3D12_CULL_MODE_BACK;
    RasterizerDefault.FrontCounterClockwise = FALSE;
    RasterizerDefault.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    RasterizerDefault.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    RasterizerDefault.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    RasterizerDefault.DepthClipEnable = TRUE;
    RasterizerDefault.MultisampleEnable = FALSE;
    RasterizerDefault.AntialiasedLineEnable = FALSE;
    RasterizerDefault.ForcedSampleCount = 0;
    RasterizerDefault.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
  }

  D3D12_GRAPHICS_PIPELINE_STATE_DESC descPipelineState;
  ZeroMemory(&descPipelineState, sizeof(descPipelineState));

  descPipelineState.VS = { GBufferVSBytecodePtr, GBufferVSbytecodeSz };
  //descPipelineState.DS = { GBufferDSBytecodePtr, GBufferDSbytecodeSz };
  //descPipelineState.HS = { GBufferHSBytecodePtr, GBufferHSbytecodeSz };
  //descPipelineState.GS = { GBufferGSBytecodePtr, GBufferGSbytecodeSz };
  descPipelineState.PS = { GBufferPSBytecodePtr, GBufferPSbytecodeSz };
  descPipelineState.InputLayout.pInputElementDescs = VertexDesc;
  descPipelineState.InputLayout.NumElements = _countof(VertexDesc);
  descPipelineState.pRootSignature = m_GBufferRootSignature;
  descPipelineState.DepthStencilState = DepthStencilDefault;
  descPipelineState.DepthStencilState.DepthEnable = false;
  descPipelineState.BlendState = BlendStateDefault;
  descPipelineState.RasterizerState = RasterizerDefault;
  descPipelineState.RasterizerState.DepthClipEnable = false;
  descPipelineState.SampleMask = UINT_MAX;
  descPipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  descPipelineState.NumRenderTargets = 4;
  descPipelineState.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
  descPipelineState.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
  descPipelineState.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
  descPipelineState.RTVFormats[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;
  descPipelineState.SampleDesc.Count = 1;

  HRESULT HRPSO = m_device->CreateGraphicsPipelineState(&descPipelineState,
                                                        __uuidof(**(&m_GPSOGBuffer)),
                                                        (void**)(&m_GPSOGBuffer));
  if (FAILED(HRPSO)) {
    throw std::exception();
  }
}

void
GraphicsAPI::CreateGPSOForward() {
  D3D12_DEPTH_STENCIL_DESC DepthStencilDefault;
  {
    DepthStencilDefault.DepthEnable = TRUE;
    DepthStencilDefault.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    DepthStencilDefault.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    DepthStencilDefault.StencilEnable = FALSE;
    DepthStencilDefault.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    DepthStencilDefault.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
    {
      D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS
    };
    DepthStencilDefault.FrontFace = defaultStencilOp;
    DepthStencilDefault.BackFace = defaultStencilOp;
  }
  D3D12_BLEND_DESC BlendStateDefault;
  {
    BlendStateDefault.AlphaToCoverageEnable = FALSE;
    BlendStateDefault.IndependentBlendEnable = FALSE;
    const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
    {
      FALSE,FALSE,
      D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
      D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
      D3D12_LOGIC_OP_NOOP,
      D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    for (UInt32 i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
      BlendStateDefault.RenderTarget[i] = defaultRenderTargetBlendDesc;
    }
  }
  D3D12_RASTERIZER_DESC RasterizerDefault;
  {
    RasterizerDefault.FillMode = D3D12_FILL_MODE_SOLID;
    RasterizerDefault.CullMode = D3D12_CULL_MODE_BACK;
    RasterizerDefault.FrontCounterClockwise = FALSE;
    RasterizerDefault.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    RasterizerDefault.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    RasterizerDefault.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    RasterizerDefault.DepthClipEnable = TRUE;
    RasterizerDefault.MultisampleEnable = FALSE;
    RasterizerDefault.AntialiasedLineEnable = FALSE;
    RasterizerDefault.ForcedSampleCount = 0;
    RasterizerDefault.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
  }

  D3D12_GRAPHICS_PIPELINE_STATE_DESC descPipelineState;
  ZeroMemory(&descPipelineState, sizeof(descPipelineState));

  //ShaderObject* vs = g_ShaderManager.getShaderObj("ScreenQuadVS");
  //ShaderObject* ds = g_ShaderManager.getShaderObj("ScreenQuadDS");
  //ShaderObject* hs = g_ShaderManager.getShaderObj("ScreenQuadHS");
  //ShaderObject* gs = g_ShaderManager.getShaderObj("ScreenQuadGS");
  //ShaderObject* ps = g_ShaderManager.getShaderObj("ScreenQuadPS");

  descPipelineState.VS = { ForwardVSBytecodePtr, ForwardVSbytecodeSz };
  //descPipelineState.DS = { GBufferDSBytecodePtr, GBufferDSbytecodeSz };
  //descPipelineState.HS = { GBufferHSBytecodePtr, GBufferHSbytecodeSz };
  //descPipelineState.GS = { GBufferGSBytecodePtr, GBufferGSbytecodeSz };
  descPipelineState.PS = { ForwardPSBytecodePtr, ForwardPSbytecodeSz };
  descPipelineState.InputLayout.pInputElementDescs = VertexDesc;
  descPipelineState.InputLayout.NumElements = _countof(VertexDesc);
  descPipelineState.pRootSignature = m_ForwardRootSignature;
  descPipelineState.DepthStencilState = DepthStencilDefault;
  descPipelineState.DepthStencilState.DepthEnable = false;
  descPipelineState.BlendState = BlendStateDefault;
  descPipelineState.RasterizerState = RasterizerDefault;
  descPipelineState.RasterizerState.DepthClipEnable = false;
  descPipelineState.SampleMask = UINT_MAX;
  descPipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  descPipelineState.NumRenderTargets = 1;
  descPipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //Swapchain desc
  descPipelineState.SampleDesc.Count = 1;

  HRESULT HRPSO = m_device->CreateGraphicsPipelineState(&descPipelineState,
                                                        __uuidof(**(&m_GPSOForward)),
                                                        (void**)(&m_GPSOForward));
  if (FAILED(HRPSO)) {
    throw std::exception();
  }
}

void
GraphicsAPI::CreateDSV() {
  //m_dsvHeap.Create(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);
  D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
  
  HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
  HeapDesc.NumDescriptors = 1;
  HeapDesc.NodeMask = 0;
  HeapDesc.Flags = (D3D12_DESCRIPTOR_HEAP_FLAGS)0;

  HRESULT hr = m_device->CreateDescriptorHeap(&HeapDesc,
                                              __uuidof(ID3D12DescriptorHeap),
                                              (void**)&m_DSDHPtr);
  
  if (FAILED(hr)) {
    throw std::exception();
  }

  m_DSCPUHeapStartHandle = m_DSDHPtr->GetCPUDescriptorHandleForHeapStart();
  m_DSGPUHeapStartHandle.ptr = 0;
  m_DSHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(HeapDesc.Type);

  D3D12_HEAP_PROPERTIES heapProperty;
  heapProperty.Type = D3D12_HEAP_TYPE_DEFAULT;
  heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  heapProperty.CreationNodeMask = 1;
  heapProperty.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc;
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Width = m_w;
	resourceDesc.Height = m_h;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clearVal;
	clearVal = { DXGI_FORMAT_D24_UNORM_S8_UINT, 1.0f };

	HRESULT HRCR = m_device->CreateCommittedResource(&heapProperty,
                                                   D3D12_HEAP_FLAG_NONE,
                                                   &resourceDesc,
                                                   //D3D12_RESOURCE_STATE_DEPTH_WRITE,
                                                   D3D12_RESOURCE_STATE_GENERIC_READ,
                                                   &clearVal,
                                                   __uuidof(**(&m_DSTexture)),
                                                   (void**)(&m_DSTexture));

  if (FAILED(HRCR)) {
    throw std::exception();
  }

  m_DSTexture->SetName(_T("DepthStencilTexture"));

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVdesc;
	ZeroMemory(&DSVdesc, sizeof(DSVdesc));
	DSVdesc.Texture2D.MipSlice = 0;
	DSVdesc.Format = resourceDesc.Format;
	DSVdesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DSVdesc.Flags = D3D12_DSV_FLAG_NONE;

  D3D12_CPU_DESCRIPTOR_HANDLE CPUDesHandle;
  CPUDesHandle.ptr = m_DSCPUHeapStartHandle.ptr;

	m_device->CreateDepthStencilView(m_DSTexture, &DSVdesc, CPUDesHandle);
	
	D3D12_SHADER_RESOURCE_VIEW_DESC descSRV;

	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Texture2D.MipLevels = resourceDesc.MipLevels;
	descSRV.Texture2D.MostDetailedMip = 0;
	descSRV.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	descSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	descSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

  D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorhandle;
  CPUDescriptorhandle.ptr = m_ShaderCPUHeapStartHandle.ptr +
                            (static_cast<SizeT>(m_SHandleIncrementSize) * 5);

	m_device->CreateShaderResourceView(m_DSTexture, &descSRV, CPUDescriptorhandle);
}

void
GraphicsAPI::CreateRTV() {
  const Int32 RTSize = 4;
  //Create deferred buffers
  //1.WorldPosition
  //2.Metallic
	//3.Roughness
	//4.Emissive

  D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
  
  HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  HeapDesc.NumDescriptors = RTSize;
  HeapDesc.NodeMask = 0;
  HeapDesc.Flags = (D3D12_DESCRIPTOR_HEAP_FLAGS)0;

  HRESULT HRRTDH = m_device->CreateDescriptorHeap(&HeapDesc,
                                                  __uuidof(ID3D12DescriptorHeap),
                                                  (void**)&m_RTDHPtr);
  
  if (FAILED(HRRTDH)) {
    throw std::exception();
  }

  m_RTCPUHeapStartHandle = m_RTDHPtr->GetCPUDescriptorHandleForHeapStart();
  m_RTGPUHeapStartHandle.ptr = 0;
  m_RTHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(HeapDesc.Type);

  D3D12_HEAP_PROPERTIES RTVHeapProperty;
  RTVHeapProperty.Type = D3D12_HEAP_TYPE_DEFAULT;
  RTVHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  RTVHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  RTVHeapProperty.CreationNodeMask = 1;
  RTVHeapProperty.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC RTVResourceDesc;
	ZeroMemory(&RTVResourceDesc, sizeof(RTVResourceDesc));
	RTVResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	RTVResourceDesc.Alignment = 0;
	RTVResourceDesc.SampleDesc.Count = 1;
	RTVResourceDesc.SampleDesc.Quality = 0;
	RTVResourceDesc.MipLevels = 1;
	
	RTVResourceDesc.DepthOrArraySize = 1;
	RTVResourceDesc.Width = m_w;
	RTVResourceDesc.Height = m_h;
	RTVResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	RTVResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_CLEAR_VALUE clearVal;
	clearVal.Color[0] = 0.0f;
	clearVal.Color[1] = 0.0f;
	clearVal.Color[2] = 0.0f;
	clearVal.Color[3] = 0.0f;

  m_RTTexture.resize(RTSize);
	for (Int32 i = 0; i < RTSize; ++i) {
    RTVResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		clearVal.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    HRESULT HRCR = m_device->CreateCommittedResource(&RTVHeapProperty,
                                                    D3D12_HEAP_FLAG_NONE,
                                                    &RTVResourceDesc,
                                                    //D3D12_RESOURCE_STATE_RENDER_TARGET,
                                                    D3D12_RESOURCE_STATE_GENERIC_READ,
                                                    &clearVal,
                                                    __uuidof(**(&m_RTTexture[i])),
                                                    (void**)(&m_RTTexture[i]));

    WString name = _T("GBuffer RT(") + StringConversion::toTString(i) + _T(")");

    m_RTTexture[i]->SetName(name.c_str());
    if (FAILED(HRCR)) {
      throw std::exception();
    }
	}

	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMemory(&RTVDesc, sizeof(RTVDesc));
	RTVDesc.Texture2D.MipSlice = 0;
	RTVDesc.Texture2D.PlaneSlice = 0;

  RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	
	for (Int32 i = 0; i < RTSize; ++i) {
    RTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorhandle;
    CPUDescriptorhandle.ptr = m_RTCPUHeapStartHandle.ptr +
      (static_cast<SizeT>(m_SHandleIncrementSize) * i);

		m_device->CreateRenderTargetView(m_RTTexture[i],
                                     &RTVDesc,
                                     CPUDescriptorhandle);
	}

	//Create SRV for RTs

	D3D12_SHADER_RESOURCE_VIEW_DESC RTVDescSRV;

	ZeroMemory(&RTVDescSRV, sizeof(RTVDescSRV));
	RTVDescSRV.Texture2D.MipLevels = RTVResourceDesc.MipLevels;
	RTVDescSRV.Texture2D.MostDetailedMip = 0;
	RTVDescSRV.Format = RTVResourceDesc.Format;
	RTVDescSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	RTVDescSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	for (int i = 0; i < RTSize; ++i) {
    RTVDescSRV.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorhandle;
    CPUDescriptorhandle.ptr = m_ShaderCPUHeapStartHandle.ptr +
                              (static_cast<SizeT>(m_SHandleIncrementSize) * (1 + i));

		m_device->CreateShaderResourceView(m_RTTexture[i],
                                       &RTVDescSRV,
                                       CPUDescriptorhandle);
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
    throw std::exception();
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