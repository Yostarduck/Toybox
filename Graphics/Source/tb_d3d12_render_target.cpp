#include "tb_d3d12_render_target.h"
#include "tb_d3d12_device.h"

namespace toyboxSDK {

RenderTargetD3D12::RenderTargetD3D12() : m_rtvDescriptorSize(0) {
}

RenderTargetD3D12::~RenderTargetD3D12() {
}

void
RenderTargetD3D12::create(const Device* device,
                          const TextureDesc& desc,
                          UInt32 numRTs) {
  const DeviceD3D12* devd3d12 = reinterpret_cast<const DeviceD3D12*>(device);
  ID3D12Device* dev = devd3d12->m_device;

  // Create descriptor heaps for Render target.
  {
    // Describe and create a render target view (RTV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.NumDescriptors = numRTs;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;

    HRESULT HRDescHeap = dev->CreateDescriptorHeap(&rtvHeapDesc,
                                                   __uuidof(**(&m_rtvHeap)),
                                                   (void**)(&m_rtvHeap));

    if (FAILED(HRDescHeap)) {
      std::exception();
    }

    m_RTHCPUHeapStart = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    m_RTHGPUHeapStart.ptr = 0;

    m_rtvDescriptorSize = dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
  }

  // Create descriptor heaps for Shader resource.
  {
    // Describe and create a shader resource view (SRV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.NumDescriptors = numRTs;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    srvHeapDesc.NodeMask = 0;

    HRESULT HRSRV = dev->CreateDescriptorHeap(&srvHeapDesc,
                                              __uuidof(**(&m_srvHeap)),
                                              (void**)&m_srvHeap);
    if (FAILED(HRSRV)) {
      std::exception();
    }

    m_SRHCPUHeapStart = m_srvHeap->GetCPUDescriptorHandleForHeapStart();
    m_SRHGPUHeapStart = m_srvHeap->GetGPUDescriptorHandleForHeapStart();

    m_srvDescriptorSize = dev->GetDescriptorHandleIncrementSize(srvHeapDesc.Type);
  }

  // Create frame resources.
  {
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

    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.Width = desc.width;
    resourceDesc.Height = desc.height;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    D3D12_CLEAR_VALUE clearVal;
    clearVal.Color[0] = 0.0f;
    clearVal.Color[1] = 0.0f;
    clearVal.Color[2] = 0.0f;
    clearVal.Color[3] = 0.0f;

    m_renderTarget.resize(numRTs);

    //Create comitted resource
    for (UInt32 n = 0; n < numRTs; ++n) {
      resourceDesc.Format = static_cast<DXGI_FORMAT>(desc.Format);
      clearVal.Format = static_cast<DXGI_FORMAT>(desc.Format);

      HRESULT HRCommitted = dev->CreateCommittedResource(&heapProperty,
                                                         D3D12_HEAP_FLAG_NONE,
                                                         &resourceDesc,
                                                         D3D12_RESOURCE_STATE_RENDER_TARGET,
                                                         &clearVal,
                                                         __uuidof(**(&m_renderTarget[n])),
                                                         (void**)(&m_renderTarget[n]));

      if (FAILED(HRCommitted)) {
        std::exception();
      }
    }

    //Create Render target
    D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));
    RTVDesc.Texture2D.MipSlice = 0;
    RTVDesc.Texture2D.PlaneSlice = 0;

    RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
    rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (UInt32 n = 0; n < numRTs; ++n) {
      RTVDesc.Format = static_cast<DXGI_FORMAT>(desc.Format);
      dev->CreateRenderTargetView(m_renderTarget[n], &RTVDesc, rtvHandle);

      rtvHandle.ptr += m_rtvDescriptorSize;
    }


    ////Create  
    //D3D12_SHADER_RESOURCE_VIEW_DESC descSRV;
    //
    //ZeroMemory(&descSRV, sizeof(descSRV));
    //descSRV.Texture2D.MipLevels = resourceDesc.MipLevels;
    //descSRV.Texture2D.MostDetailedMip = 0;
    //descSRV.Format = resourceDesc.Format;
    //descSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    //descSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    //
    //for (UInt32 n = 0; n < numRTs; ++n) {
    //  descSRV.Format = static_cast<DXGI_FORMAT>(desc.Format);
    //  dev->CreateShaderResourceView(m_renderTarget[n], &descSRV, m_cbvsrvHeap.hCPU(i + 2));
    //}
  }
}

void
RenderTargetD3D12::create(const Device* device,
                          const std::vector<TextureDesc>& desc,
                          UInt32 numRTs) {
}

//void
//RenderTargetD3D12::create(const Device& device,
//                          const std::vector<Texture*>& textures) {
//}

}
