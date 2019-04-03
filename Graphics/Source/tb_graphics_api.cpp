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
  //EnableRTX(); //RTX
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
    //CreateDXRDeviceAndCommandList(); //RTX
    CreateConstantBuffer();
    CreateQuadVB();
    CreateQuadIB();
    CreateShaderHeap();
    CreateShaders();
    CreateGBufferRootSignature();
    CreateForwardRootSignature();
    CreateInverterRootSignature();
    CreateGPSOGBuffer();
    CreateGPSOForward();
    CreateCPSOInverter();
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
GraphicsAPI::CreateModel(std::vector<byte>& VB, std::vector<byte>& IB, SizeT totalVertex) {
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
	resourceDesc.Height = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  //VB
  {
	  resourceDesc.Width = VB.size();

	  HRESULT HRVBCR = m_device->CreateCommittedResource(&heapProperty,
                                                       D3D12_HEAP_FLAG_NONE,
                                                       &resourceDesc,
                                                       D3D12_RESOURCE_STATE_GENERIC_READ,
                                                       nullptr,
                                                       __uuidof(**(&m_ModelVB)),
                                                       (void**)(&m_ModelVB));
	
    if (FAILED(HRVBCR)) {
      throw std::exception();
    }

	  byte* VBDataBegin;
	  HRESULT HRMapVB = m_ModelVB->Map(0, nullptr, reinterpret_cast<void**>(&VBDataBegin));
    if (FAILED(HRMapVB)) {
      throw std::exception();
    }
    std::memcpy(VBDataBegin, &VB[0], VB.size());
	  m_ModelVB->Unmap(0, nullptr);

	  m_ModelVBView.BufferLocation = m_ModelVB->GetGPUVirtualAddress();
	  m_ModelVBView.StrideInBytes = sizeof(ModelVertex);
	  m_ModelVBView.SizeInBytes = VB.size();
  }
  //IB
  {
    resourceDesc.Width = IB.size();

    HRESULT HRIDCR = m_device->CreateCommittedResource(&heapProperty,
                                                       D3D12_HEAP_FLAG_NONE,
                                                       &resourceDesc,
                                                       D3D12_RESOURCE_STATE_GENERIC_READ,
                                                       nullptr,
                                                       __uuidof(**(&m_ModelIB)),
                                                       (void**)(&m_ModelIB));

    if (FAILED(HRIDCR)) {
      throw std::exception();
    }

    byte* IBDataBegin;
    HRESULT HRIB = m_ModelIB->Map(0, nullptr, reinterpret_cast<void**>(&IBDataBegin));
    if (FAILED(HRIB)) {
      throw std::exception();
    }
    std::memcpy(IBDataBegin, &IB[0], IB.size());
    m_ModelIB->Unmap(0, nullptr);

    m_ModelIBView.BufferLocation = m_ModelIB->GetGPUVirtualAddress();
    m_ModelIBView.Format = DXGI_FORMAT_R32_UINT;
    m_ModelIBView.SizeInBytes = IB.size();
  }

  m_ModelIndexes = totalVertex;
}
////////////////////////////////////////////////////////////////////////////////
void
MemcpySubresource(const D3D12_MEMCPY_DEST* pDest,
                  const D3D12_SUBRESOURCE_DATA* pSrc,
                  SizeT RowSizeInBytes,
                  UInt32 NumRows,
                  UInt32 NumSlices) {
  for (UInt32 z = 0; z < NumSlices; ++z) {
    byte* pDestSlice = reinterpret_cast<byte*>(pDest->pData) +
                       (pDest->SlicePitch * z);
    const byte* pSrcSlice = reinterpret_cast<const byte*>(pSrc->pData) +
                            (pSrc->SlicePitch * z);

    for (UInt32 y = 0; y < NumRows; ++y) {
      memcpy(pDestSlice + (pDest->RowPitch * y),
             pSrcSlice + (pSrc->RowPitch * y),
             RowSizeInBytes);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
UInt64
UpdateSubresources(ID3D12GraphicsCommandList* pCmdList,
                   ID3D12Resource* pDestinationResource,
                   ID3D12Resource* pIntermediate,
                   UInt32 FirstSubresource,
                   UInt32 NumSubresources,
                   UInt64 RequiredSize,
                   const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
                   const UInt32* pNumRows,
                   const UInt64* pRowSizesInBytes,
                   const D3D12_SUBRESOURCE_DATA* pSrcData) {
  // Minor validation
  auto IntermediateDesc = pIntermediate->GetDesc();
  auto DestinationDesc = pDestinationResource->GetDesc();
  if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER ||
      IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset ||
      RequiredSize > SizeT(-1) ||
      (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&
      (FirstSubresource != 0 || NumSubresources != 1))) {
    return 0;
  }

  byte* pData;
  HRESULT hr = pIntermediate->Map(0, nullptr, reinterpret_cast<void**>(&pData));
  if (FAILED(hr)) {
    return 0;
  }

  for (UInt32 i = 0; i < NumSubresources; ++i) {
    if (pRowSizesInBytes[i] > SizeT(-1)) return 0;
    D3D12_MEMCPY_DEST DestData = { pData + pLayouts[i].Offset, pLayouts[i].Footprint.RowPitch, SizeT(pLayouts[i].Footprint.RowPitch) * SizeT(pNumRows[i]) };
    MemcpySubresource(&DestData, &pSrcData[i], static_cast<SizeT>(pRowSizesInBytes[i]), pNumRows[i], pLayouts[i].Footprint.Depth);
  }
  pIntermediate->Unmap(0, nullptr);

  if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER) {
    pCmdList->CopyBufferRegion(pDestinationResource, 0, pIntermediate, pLayouts[0].Offset, pLayouts[0].Footprint.Width);
  }
  else {
    for (UInt32 i = 0; i < NumSubresources; ++i) {
      D3D12_TEXTURE_COPY_LOCATION Dst;
      Dst.pResource = pDestinationResource;
      Dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
      Dst.SubresourceIndex = i + FirstSubresource;

      D3D12_TEXTURE_COPY_LOCATION Src;
      Src.pResource = pIntermediate;
      Src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
      Src.PlacedFootprint = pLayouts[i];

      pCmdList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
    }
  }
  return RequiredSize;
}
////////////////////////////////////////////////////////////////////////////////
UInt64
UpdateSubresources(ID3D12GraphicsCommandList* pCmdList,
                   ID3D12Resource* pDestinationResource,
                   ID3D12Resource* pIntermediate,
                   UInt64 IntermediateOffset,
                   UInt32 FirstSubresource,
                   UInt32 NumSubresources,
                   D3D12_SUBRESOURCE_DATA* pSrcData) {
  UInt64 RequiredSize = 0;
  UInt64 MemToAlloc = static_cast<UInt64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) +
                                          sizeof(UInt32) +
                                          sizeof(UInt64)) *
                      NumSubresources;

  if (MemToAlloc > SIZE_MAX) {
    return 0;
  }

  void* pMem = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
  if (nullptr == pMem) {
    return 0;
  }

  auto pLayouts = reinterpret_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
  UInt64* pRowSizesInBytes = reinterpret_cast<UInt64*>(pLayouts + NumSubresources);
  UInt32* pNumRows = reinterpret_cast<UInt32*>(pRowSizesInBytes + NumSubresources);

  auto Desc = pDestinationResource->GetDesc();
  ID3D12Device* pDevice = nullptr;
  pDestinationResource->GetDevice(__uuidof(*pDevice), reinterpret_cast<void**>(&pDevice));
  pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
  pDevice->Release();

  UInt64 Result = UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes, pSrcData);
  HeapFree(GetProcessHeap(), 0, pMem);
  return Result;
}
////////////////////////////////////////////////////////////////////////////////
std::vector<byte>
GenerateTextureData() {
  const UInt32 rowPitch = 2048 * 4;
  const UInt32 cellPitch = rowPitch >> 3; // The width of a cell in the checkboard texture.
  const UInt32 cellHeight = 2048 >> 3;    // The height of a cell in the checkerboard texture.
  const UInt32 textureSize = rowPitch * 2048;

  std::vector<byte> data(textureSize);
  byte* pData = &data[0];

  for (UInt32 n = 0; n < textureSize; n += 4) {
    UInt32 x = n % rowPitch;
    UInt32 y = n / rowPitch;
    UInt32 i = x / cellPitch;
    UInt32 j = y / cellHeight;

    if (i % 2 == j % 2) {
      pData[n] = 0x00;        // R
      pData[n + 1] = 0x00;    // G
      pData[n + 2] = 0x00;    // B
      pData[n + 3] = 0xff;    // A
    }
    else {
      pData[n] = 0xff;        // R
      pData[n + 1] = 0xff;    // G
      pData[n + 2] = 0xff;    // B
      pData[n + 3] = 0xff;    // A
    }
  }

  return data;
}
////////////////////////////////////////////////////////////////////////////////

void
GraphicsAPI::CreateTexture(UInt32 textureWidth,
                           UInt32 textureHeight,
                           TB_FORMAT::E pixelFormat,
                           byte* initData) {
  /////Texture resource creation/////

  //Default Heap properties
  D3D12_HEAP_PROPERTIES defaultHeapProperty;
  defaultHeapProperty.Type = D3D12_HEAP_TYPE_DEFAULT;
  defaultHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  defaultHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  defaultHeapProperty.CreationNodeMask = 1;
  defaultHeapProperty.VisibleNodeMask = 1;
  
  //Describe a Texture2D.
  D3D12_RESOURCE_DESC textureDesc = {};
  textureDesc.MipLevels = 1;
  //textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.Width = textureWidth;
  textureDesc.Height = textureHeight;
  textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
  textureDesc.DepthOrArraySize = 1;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  
  //Create a Texture2D resource.
  HRESULT HRTexRes = m_device->CreateCommittedResource(&defaultHeapProperty,
                                                       D3D12_HEAP_FLAG_NONE,
                                                       &textureDesc,
                                                       D3D12_RESOURCE_STATE_COPY_DEST,
                                                       nullptr,
                                                       __uuidof(**(&m_texture)),
                                                       (void**)(&m_texture));

  if (FAILED(HRTexRes)) {
    throw std::exception();
  }

  m_texture->SetName(_T("Albedo texture"));

  //Get required buffer size
  UInt64 uploadBufferSize = 0;
  m_device->GetCopyableFootprints(&textureDesc,
                                  0,
                                  1,
                                  0,
                                  nullptr,
                                  nullptr,
                                  nullptr,
                                  &uploadBufferSize);

  //Upload heap properties
  D3D12_HEAP_PROPERTIES uploadHeapProperty;
  uploadHeapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
  uploadHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  uploadHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  uploadHeapProperty.CreationNodeMask = 1;
  uploadHeapProperty.VisibleNodeMask = 1;

  //Upload buffer description
  D3D12_RESOURCE_DESC uploadBufferDesc;
  uploadBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  uploadBufferDesc.Alignment = 0;
  uploadBufferDesc.Width = uploadBufferSize;
  uploadBufferDesc.Height = 1;
  uploadBufferDesc.DepthOrArraySize = 1;
  uploadBufferDesc.MipLevels = 1;
  uploadBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
  uploadBufferDesc.SampleDesc.Count = 1;
  uploadBufferDesc.SampleDesc.Quality = 0;
  uploadBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  uploadBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

  // Create the upload buffer.
  ID3D12Resource* textureUploadHeap;
  HRESULT HRUpload = m_device->CreateCommittedResource(&uploadHeapProperty,
                                                       D3D12_HEAP_FLAG_NONE,
                                                       &uploadBufferDesc,
                                                       D3D12_RESOURCE_STATE_GENERIC_READ,
                                                       nullptr,
                                                       __uuidof(**(&textureUploadHeap)),
                                                       (void**)(&textureUploadHeap));
  
  if (FAILED(HRUpload)) {
    throw std::exception();
  }

  std::vector<byte> texture = GenerateTextureData();

  //Copy data to the intermediate upload heap and then schedule a copy
  //from the upload heap to the Texture2D.
  D3D12_SUBRESOURCE_DATA textureData = {};
  textureData.pData = initData;
  //textureData.pData = &texture[0];
  //textureData.RowPitch = textureWidth * texturePixelSize;
  textureData.RowPitch = textureWidth * ((8 * 4) / 8);
  //textureData.RowPitch = textureWidth * ((32 * 4) / 8);
  //textureData.RowPitch = textureWidth * 4;
  textureData.SlicePitch = textureData.RowPitch * textureHeight;
  
  D3D12_RESOURCE_BARRIER barrier = {};
  barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barrier.Transition.pResource = m_texture;
  barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
  barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
  barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

  UpdateSubresources(m_commandList, m_texture, textureUploadHeap, 0, 0, 1, &textureData);
  m_commandList->ResourceBarrier(1, &barrier);

  //Describe a SRV for the texture.
  D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srvDesc.Format = textureDesc.Format;
  srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;

  //Create a SRV for the texture.
  D3D12_CPU_DESCRIPTOR_HANDLE SHCPUDescriptorhandle;
  SHCPUDescriptorhandle.ptr = m_ShaderCPUHeapStartHandle.ptr +
                              (static_cast<SizeT>(m_SHandleIncrementSize) * 8);

  m_device->CreateShaderResourceView(m_texture, &srvDesc, SHCPUDescriptorhandle);
}

void
GraphicsAPI::UpdateCB(std::vector<byte>& data) {
  //Gbuffer CB
  {
    byte* mapped = nullptr;
    m_GbufferCB->Map(0, nullptr, reinterpret_cast<void**>(&mapped));
    std::memcpy(mapped, &data[0], data.size());
    m_GbufferCB->Unmap(0, nullptr);
  }
  //Compute CB
  {
    Vector2 screen = Vector2(1280.0f, 720.0f);
    byte* screenData = reinterpret_cast<byte*>(&screen[0]);

    byte* mapped = nullptr;
    m_ComputeCB->Map(0, nullptr, reinterpret_cast<void**>(&mapped));
    std::memcpy(mapped, screenData, sizeof(Vector2));
    m_ComputeCB->Unmap(0, nullptr);
  }
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
                              (static_cast<SizeT>(m_RTHandleIncrementSize) * i);

    m_commandList->ClearRenderTargetView(CPUDescriptorhandle, mClearColor, 0, nullptr);
  }

  m_commandList->ClearDepthStencilView(m_DSCPUHeapStartHandle, D3D12_CLEAR_FLAG_DEPTH, 0.0f, 0xff, 0, nullptr);

  D3D12_GPU_DESCRIPTOR_HANDLE handleOffset;

  m_commandList->OMSetRenderTargets(RTSize, &m_RTCPUHeapStartHandle, true, &m_DSCPUHeapStartHandle);
  m_commandList->SetDescriptorHeaps(1, ppHeaps);
  m_commandList->SetGraphicsRootSignature(m_GBufferRootSignature);

  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 8); //Tex
  m_commandList->SetGraphicsRootDescriptorTable(0, handleOffset);
  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 0); //CB G
  m_commandList->SetGraphicsRootDescriptorTable(1, handleOffset);

  m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  m_commandList->IASetIndexBuffer(&m_ModelIBView);
  m_commandList->IASetVertexBuffers(0, 1, &m_ModelVBView);

  m_commandList->DrawIndexedInstanced(m_ModelIndexes, 1, 0, 0, 0);
}

void
GraphicsAPI::ApplyInverter() {
  ID3D12DescriptorHeap* ppHeaps[1] = { m_ShaderDHPtr };

  m_commandList->SetPipelineState(m_CPSOInverter);

  D3D12_GPU_DESCRIPTOR_HANDLE handleOffset;
  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr;

  m_commandList->SetDescriptorHeaps(1, ppHeaps);
  m_commandList->SetComputeRootSignature(m_InverterRootSignature);

  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 8); //Tex
  m_commandList->SetComputeRootDescriptorTable(0, handleOffset);
  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 2); //UAV
  m_commandList->SetComputeRootDescriptorTable(1, handleOffset);
  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 1); //CB
  m_commandList->SetComputeRootDescriptorTable(2, handleOffset);

  m_commandList->Dispatch(20, 20, 1);
}

void
GraphicsAPI::ApplyForward() {
  const Int32 RTSize = 4;
  float mClearColor[4] = { 0.0, 0.0f, 0.0f, 1.0f };

  for (Int32 i = 0; i < RTSize; ++i) {
    AddResourceBarrier(m_commandList,
                       m_RTTexture[i],
                       D3D12_RESOURCE_STATE_RENDER_TARGET,
                       D3D12_RESOURCE_STATE_GENERIC_READ);
  }

  AddResourceBarrier(m_commandList,
                     m_DSTexture,
                     D3D12_RESOURCE_STATE_DEPTH_WRITE,
                     D3D12_RESOURCE_STATE_GENERIC_READ);

  ID3D12DescriptorHeap* ppHeaps[1] = { m_ShaderDHPtr };

  m_commandList->SetPipelineState(m_GPSOForward);

  D3D12_GPU_DESCRIPTOR_HANDLE handleOffset;

  m_commandList->OMSetRenderTargets(1, &GetRenderTargetView(), true, nullptr);
  m_commandList->SetDescriptorHeaps(1, ppHeaps);
  m_commandList->SetGraphicsRootSignature(m_ForwardRootSignature);

  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 2); //RTs
  m_commandList->SetGraphicsRootDescriptorTable(0, handleOffset);
  handleOffset.ptr = m_ShaderGPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 7); //Ds
  m_commandList->SetGraphicsRootDescriptorTable(1, handleOffset);

  m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
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
  ++m_fenceValues[m_iCurrentFrame];
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
GraphicsAPI::EnableRTX() {
  ID3D12Device* testDevice = nullptr;

  bool computeRTXAvaible = false;
  {
    IDXGIFactory4* factory = GetFactory();

    GetHardwareAdapter(factory, &m_hardwareAdapter);

    UUID experimentalFeatures[] = { D3D12ExperimentalShaderModels };

    HRESULT HRExperimental = D3D12EnableExperimentalFeatures(1, experimentalFeatures, nullptr, nullptr);
    HRESULT HRDevice = D3D12CreateDevice(m_hardwareAdapter,
                                         D3D_FEATURE_LEVEL_11_0,
                                         __uuidof(**(&testDevice)),
                                         (void**)(&testDevice));
    if (FAILED(HRExperimental)) {
      std::cout << HRExperimental << std::endl;
    }
    if (FAILED(HRDevice)) {
      std::cout << HRDevice << std::endl;
    }

    bool b1 = SUCCEEDED(HRExperimental);
    bool b2 = SUCCEEDED(HRDevice);

    computeRTXAvaible = b1 && b2;
    computeRTXAvaible = b1;
  }

  if (!computeRTXAvaible) {
    //std::cout << _T("Warning: Could not enable Compute Raytracing Fallback (D3D12EnableExperimentalFeatures() failed).\n Possible reasons: your OS is not in developer mode.\n\n") << std::endl;
  }

  bool m_isDxrSupported = false;
  {
    //ID3D12Device* testDevice = nullptr;
    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureSupportData = {};

    //bool b1 = SUCCEEDED(D3D12CreateDevice(m_device,
    //                                      D3D_FEATURE_LEVEL_11_0,
    //                                      __uuidof(**(&testDevice)),
    //                                      (void**)(&testDevice)));
    bool b2 = SUCCEEDED(testDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureSupportData, sizeof(featureSupportData)));
    bool b3 = featureSupportData.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;

    //m_isDxrSupported = b1 && b2 && b3;
    m_isDxrSupported = true && b2 && b3;
  }

  if (!m_isDxrSupported) {
    //std::cout << _T"Warning: DirectX Raytracing is not supported by your GPU and driver.\n\n") << std::endl;

    if (!computeRTXAvaible) {
      //std::cout << _T("Could not enable compute based fallback raytracing support (D3D12EnableExperimentalFeatures() failed).\n Possible reasons: your OS is not in developer mode.\n\n") << std::endl;
      m_raytracingMode = RTXMode::None;
      return;
    }

    m_raytracingMode = RTXMode::FallbackLayer;
    return;
  }

  m_raytracingMode = RTXMode::DirectXRaytracing;
  return;
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
		GetHardwareAdapter(factory, &m_hardwareAdapter);

    HRESULT HRCreateDevice = D3D12CreateDevice(m_hardwareAdapter,
			                                         D3D_FEATURE_LEVEL_11_0,
                                               __uuidof(**(&m_device)),
                                               (void**)(&m_device));
    if (FAILED(HRCreateDevice)) {
      throw std::exception();
    }
    //hardwareAdapter->Release();
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

  for (Int32 n = 0; n < m_iFrameBuffers; ++n) {
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
                                                      //m_GPSOForward,
                                                      __uuidof(**(&m_commandList)),
                                                      (void**)(&m_commandList));

  if (FAILED(HRCommandList)) {
    throw std::exception();
  }
}

void
GraphicsAPI::CreateDXRDeviceAndCommandList() {
  if (RTXMode::DirectXRaytracing == m_raytracingMode) {
    HRESULT HRDXRDevice = m_device->QueryInterface(__uuidof(**(&m_dxrDevice)),
                                                   (void**)(&m_dxrDevice));
    if (FAILED(HRDXRDevice)) {
      //std::cout << _T("Couldn't get DirectX Raytracing interface for the device.\n") << std::endl;
      throw std::exception();
      return;
    }

    HRESULT HRDXRCommandList = m_commandList->QueryInterface(__uuidof(**(&m_dxrCommandList)),
                                                             (void**)(&m_dxrCommandList));
    if (FAILED(HRDXRDevice)) {
      //std::cout << _T("Couldn't get DirectX Raytracing interface for the command list.\n") << std::endl;
      throw std::exception();
      return;
    }
  }
  else if (RTXMode::FallbackLayer == m_raytracingMode) {
  }
  else if (RTXMode::None == m_raytracingMode) {
    m_dxrDevice = nullptr;
    m_dxrCommandList = nullptr;
  }

}

void
GraphicsAPI::CreateShaders() {
  FileSystem FSys;
  TString path = FSys.GetWorkingPath();

  CompileShaderFromFile(path + _T("Resources\\Shaders\\GBuffer_vs.hlsl"),
                        TB_SHADER_TYPE::E::kVertex,
                        &GBufferVSShaderBlob,
                        &GBufferVSBytecodePtr,
                        &GBufferVSbytecodeSz);
  CompileShaderFromFile(path + _T("Resources\\Shaders\\GBuffer_ps.hlsl"),
                        TB_SHADER_TYPE::E::kFragment,
                        &GBufferPSShaderBlob,
                        &GBufferPSBytecodePtr,
                        &GBufferPSbytecodeSz);

  CompileShaderFromFile(path + _T("Resources\\Shaders\\Forward_vs.hlsl"),
                        TB_SHADER_TYPE::E::kVertex,
                        &ForwardVSShaderBlob,
                        &ForwardVSBytecodePtr,
                        &ForwardVSbytecodeSz);
  CompileShaderFromFile(path + _T("Resources\\Shaders\\Forward_ps.hlsl"),
                        TB_SHADER_TYPE::E::kFragment,
                        &ForwardPSShaderBlob,
                        &ForwardPSBytecodePtr,
                        &ForwardPSbytecodeSz);

  CompileShaderFromFile(path + _T("Resources\\Shaders\\Basic_cs.hlsl"),
                        TB_SHADER_TYPE::E::kCompute,
                        &InverterCSShaderBlob,
                        &InverterCSBytecodePtr,
                        &InverterCSbytecodeSz);
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

  HRESULT HRCBCRGB = m_device->CreateCommittedResource(&heapProperty,
                                                       D3D12_HEAP_FLAG_NONE,
                                                       &resourceDesc,
                                                       D3D12_RESOURCE_STATE_GENERIC_READ,
                                                       nullptr,
                                                       __uuidof(**(&m_GbufferCB)),
                                                       (void**)(&m_GbufferCB));

  if (FAILED(HRCBCRGB)) {
    throw std::exception();
  }

  resourceDesc.Width = 256;
  HRESULT HRCBCRC = m_device->CreateCommittedResource(&heapProperty,
                                                      D3D12_HEAP_FLAG_NONE,
                                                      &resourceDesc,
                                                      D3D12_RESOURCE_STATE_GENERIC_READ,
                                                      nullptr,
                                                      __uuidof(**(&m_ComputeCB)),
                                                      (void**)(&m_ComputeCB));

  if (FAILED(HRCBCRC)) {
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

	byte* VBDataBegin;
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

  byte* IBDataBegin;
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

  // Thing           Array  offset
  //
  // CBV
  //  GBuffer      = 1      (0)
  //  Compute      = 1      (1)
  // SRV
  //   GBuffer RTv = 4      (2 - 5)
  //   Compute RTv = 1      (6)
  //   GBuffer DSv = 1      (7)
  //   Model Tex   = 1      (8)
  // UAV = 0

  //Start of create GPU Heap
  D3D12_DESCRIPTOR_HEAP_DESC GPUHDesc;
  GPUHDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  GPUHDesc.NodeMask = 0;
  GPUHDesc.NumDescriptors = 9;
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
  D3D12_CONSTANT_BUFFER_VIEW_DESC	descBuffer;

  //Camera CBV
  descBuffer.BufferLocation = m_GbufferCB->GetGPUVirtualAddress();
  //Constant buffer must be larger than 256 bytes
  //descBuffer.SizeInBytes = sizeof(CBuffer); //Memory should be aligned
  //descBuffer.SizeInBytes = (sizeof(CBuffer) + 255) & ~255; //Aligns memory to 256
  descBuffer.SizeInBytes = 256; //Aligns memory to 256

  CPUHandle.ptr = m_ShaderCPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 0);
  m_device->CreateConstantBufferView(&descBuffer, CPUHandle);

  descBuffer.BufferLocation = m_ComputeCB->GetGPUVirtualAddress();
  //Constant buffer must be larger than 256 bytes
  //descBuffer.SizeInBytes = sizeof(CBuffer); //Memory should be aligned
  //descBuffer.SizeInBytes = (sizeof(CBuffer) + 255) & ~255; //Aligns memory to 256
  descBuffer.SizeInBytes = 256; //Aligns memory to 256
  CPUHandle.ptr = m_ShaderCPUHeapStartHandle.ptr + (m_SHandleIncrementSize * 1);
  m_device->CreateConstantBufferView(&descBuffer, CPUHandle);
}

void
GraphicsAPI::CreateGBufferRootSignature() {
  //Init descriptor tables

  D3D12_DESCRIPTOR_RANGE range[2];
  {
    //SRV (The albedo texture)
    range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    range[0].NumDescriptors = 1;
    range[0].BaseShaderRegister = 0;
    range[0].RegisterSpace = 0;
    range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    //CBV (The camera one)
    range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    range[1].NumDescriptors = 1;
    range[1].BaseShaderRegister = 0;
    range[1].RegisterSpace = 0;
    range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
  }

  D3D12_ROOT_PARAMETER rootParameters[2];
  {
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
  }

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
                                                          __uuidof(**(&m_GBufferRootSignature)),
                                                          (void**)(&m_GBufferRootSignature));
  if (FAILED(HRRootSignature)) {
    throw std::exception();
  }
}

void
GraphicsAPI::CreateInverterRootSignature() {
  //Init descriptor tables

  D3D12_DESCRIPTOR_RANGE range[3];
  {
    //SRV (The depth stencil)
    range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    range[0].NumDescriptors = 1;
    range[0].BaseShaderRegister = 0;
    range[0].RegisterSpace = 0;
    range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    //SRV (The depth stencil)
    range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
    range[1].NumDescriptors = 1;
    range[1].BaseShaderRegister = 0;
    range[1].RegisterSpace = 0;
    range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    //SRV (GBuffer RTs)
    range[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    range[2].NumDescriptors = 1;
    range[2].BaseShaderRegister = 0;
    range[2].RegisterSpace = 0;
    range[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
  }

  D3D12_ROOT_PARAMETER rootParameters[3];
  {
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
    //
    rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    rootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
    rootParameters[2].DescriptorTable.pDescriptorRanges = &range[2];
  }

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
    StaticSamplers.MinLOD = 0.0f;
    StaticSamplers.MaxLOD = D3D12_FLOAT32_MAX;
    StaticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    StaticSamplers.RegisterSpace = 0;
  }

  D3D12_ROOT_SIGNATURE_DESC descRootSignature;
  descRootSignature.NumParameters = 3;
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
                                                          __uuidof(**(&m_InverterRootSignature)),
                                                          (void**)(&m_InverterRootSignature));
  if (FAILED(HRRootSignature)) {
    throw std::exception();
  }

}

void
GraphicsAPI::CreateForwardRootSignature() {
  //Init descriptor tables

  D3D12_DESCRIPTOR_RANGE range[2];
  {
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
  }

  D3D12_ROOT_PARAMETER rootParameters[2];
  {
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
  }

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

static D3D12_INPUT_ELEMENT_DESC ModelVertexDesc[] = {
  { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
  { "NORMAL",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
  { "BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
  { "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 64, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
};

static D3D12_INPUT_ELEMENT_DESC QuadVertexDesc[] = {
  { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
};

void
GraphicsAPI::CreateGPSOGBuffer() {
  D3D12_DEPTH_STENCIL_DESC DepthStencilDefault;
  {
    DepthStencilDefault.DepthEnable = true;
    DepthStencilDefault.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    DepthStencilDefault.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
    DepthStencilDefault.StencilEnable = false;
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

  D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSODescPipelineState;
  ZeroMemory(&GPSODescPipelineState, sizeof(GPSODescPipelineState));

  GPSODescPipelineState.VS = { GBufferVSBytecodePtr, GBufferVSbytecodeSz };
  //GPSODescPipelineState.DS = { GBufferDSBytecodePtr, GBufferDSbytecodeSz };
  //GPSODescPipelineState.HS = { GBufferHSBytecodePtr, GBufferHSbytecodeSz };
  //GPSODescPipelineState.GS = { GBufferGSBytecodePtr, GBufferGSbytecodeSz };
  GPSODescPipelineState.PS = { GBufferPSBytecodePtr, GBufferPSbytecodeSz };
  GPSODescPipelineState.InputLayout.pInputElementDescs = ModelVertexDesc;
  GPSODescPipelineState.InputLayout.NumElements = _countof(ModelVertexDesc);
  GPSODescPipelineState.pRootSignature = m_GBufferRootSignature;
  GPSODescPipelineState.DepthStencilState = DepthStencilDefault;
  GPSODescPipelineState.BlendState = BlendStateDefault;
  GPSODescPipelineState.RasterizerState = RasterizerDefault;
  GPSODescPipelineState.SampleMask = UINT_MAX;
  GPSODescPipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  GPSODescPipelineState.NumRenderTargets = 4;
  GPSODescPipelineState.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
  GPSODescPipelineState.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
  GPSODescPipelineState.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
  GPSODescPipelineState.RTVFormats[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;
  GPSODescPipelineState.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
  GPSODescPipelineState.SampleDesc.Count = 1;

  HRESULT HRPSO = m_device->CreateGraphicsPipelineState(&GPSODescPipelineState,
                                                        __uuidof(**(&m_GPSOGBuffer)),
                                                        (void**)(&m_GPSOGBuffer));
  if (FAILED(HRPSO)) {
    throw std::exception();
  }
}

void
GraphicsAPI::CreateCPSOInverter() {
  D3D12_COMPUTE_PIPELINE_STATE_DESC CPSODescPipelineState;
  ZeroMemory(&CPSODescPipelineState, sizeof(CPSODescPipelineState));

  CPSODescPipelineState.pRootSignature = m_InverterRootSignature;
  CPSODescPipelineState.CS = { InverterCSBytecodePtr, InverterCSbytecodeSz };

  HRESULT HRPSO = m_device->CreateComputePipelineState(&CPSODescPipelineState,
                                                       __uuidof(**(&m_CPSOInverter)),
                                                       (void**)(&m_CPSOInverter));
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
  descPipelineState.InputLayout.pInputElementDescs = QuadVertexDesc;
  descPipelineState.InputLayout.NumElements = _countof(QuadVertexDesc);
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
	clearVal = { DXGI_FORMAT_D24_UNORM_S8_UINT, 0.0f };

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
                            (static_cast<SizeT>(m_SHandleIncrementSize) * 7);

	m_device->CreateShaderResourceView(m_DSTexture, &descSRV, CPUDescriptorhandle);
}

void
GraphicsAPI::CreateRTV() {
  const Int32 GBufferRTSize = 5;
  //Create deferred buffers
  //1.WorldPosition
  //2.Metallic
	//3.Roughness
  //4.Emissive
  //5.ComputeRT

  D3D12_DESCRIPTOR_HEAP_DESC RTHeapDesc;
  
  RTHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  RTHeapDesc.NumDescriptors = GBufferRTSize;
  RTHeapDesc.NodeMask = 0;
  RTHeapDesc.Flags = (D3D12_DESCRIPTOR_HEAP_FLAGS)0;

  HRESULT HRRTDH = m_device->CreateDescriptorHeap(&RTHeapDesc,
                                                  __uuidof(ID3D12DescriptorHeap),
                                                  (void**)&m_RTDHPtr);
  
  if (FAILED(HRRTDH)) {
    throw std::exception();
  }

  m_RTCPUHeapStartHandle = m_RTDHPtr->GetCPUDescriptorHandleForHeapStart();
  m_RTGPUHeapStartHandle.ptr = 0;
  m_RTHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(RTHeapDesc.Type);

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

  m_RTTexture.resize(GBufferRTSize);
	for (Int32 i = 0; i < GBufferRTSize; ++i) {
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

    WString name = _T("Gbuffer RT(") + StringConversion::toTString(i) + _T(")");

    m_RTTexture[i]->SetName(name.c_str());
    if (FAILED(HRCR)) {
      throw std::exception();
    }
	}
  m_RTTexture[4]->SetName(_T("Compute RT"));

	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMemory(&RTVDesc, sizeof(RTVDesc));
	RTVDesc.Texture2D.MipSlice = 0;
	RTVDesc.Texture2D.PlaneSlice = 0;

  RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	
	for (Int32 i = 0; i < GBufferRTSize; ++i) {
    RTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    D3D12_CPU_DESCRIPTOR_HANDLE RTCPUDescriptorhandle;
    RTCPUDescriptorhandle.ptr = m_RTCPUHeapStartHandle.ptr +
                                (m_RTHandleIncrementSize * static_cast<SizeT>(i));

		m_device->CreateRenderTargetView(m_RTTexture[i],
                                     &RTVDesc,
                                     RTCPUDescriptorhandle);
	}

	//Create SRV for RTs

	D3D12_SHADER_RESOURCE_VIEW_DESC RTVDescSRV;

	ZeroMemory(&RTVDescSRV, sizeof(RTVDescSRV));
	RTVDescSRV.Texture2D.MipLevels = RTVResourceDesc.MipLevels;
	RTVDescSRV.Texture2D.MostDetailedMip = 0;
	RTVDescSRV.Format = RTVResourceDesc.Format;
	RTVDescSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	RTVDescSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	for (Int32 i = 0; i < GBufferRTSize; ++i) {
    RTVDescSRV.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    D3D12_CPU_DESCRIPTOR_HANDLE SHCPUDescriptorhandle;
    SHCPUDescriptorhandle.ptr = m_ShaderCPUHeapStartHandle.ptr +
                                (static_cast<SizeT>(m_SHandleIncrementSize) * (2 + i));

		m_device->CreateShaderResourceView(m_RTTexture[i],
                                       &RTVDescSRV,
                                       SHCPUDescriptorhandle);
	}
}

void
GraphicsAPI::CompileShaderFromFile(WString filepath,
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
    OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    throw std::exception();
  }

  (*bytecodePtrOut) = (*shaderBlobOut)->GetBufferPointer();
  (*bytecodeSzOut) = (*shaderBlobOut)->GetBufferSize();
}

void
GraphicsAPI::CompileShaderFromBuffer(WString code,
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

  HRESULT HRShader = D3DCompile(code.c_str(),
                                code.size(),
                                nullptr,
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