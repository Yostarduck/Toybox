#include "tb_d3d12_render_target.h"
#include "tb_d3d12_device.h"

namespace toyboxSDK {

RenderTargetD3D12::RenderTargetD3D12() : m_rtvDescriptorSize(0) {
}

RenderTargetD3D12::~RenderTargetD3D12() {
}

void
RenderTargetD3D12::create(const Device& device,
                          const TextureDesc& desc,
                          UInt32 numRTs) {
  /*
  const DeviceD3D12* devd3d12 = reinterpret_cast<const DeviceD3D12*>(&device);
  ID3D12Device* dev = devd3d12->m_device;

  // Create descriptor heaps for render target.
  {
    // Describe and create a render target view (RTV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = 2;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT HRDescHeap = dev->CreateDescriptorHeap(&rtvHeapDesc,
                                                   __uuidof(**(&m_rtvHeap)),
                                                   (void**)(&m_rtvHeap));

    if (FAILED(HRDescHeap)) {
      std::exception();
    }

    m_rtvDescriptorSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  }

  // Create frame resources.
  {
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    // Create a RTV for each frame.
    for (UInt32 n = 0; n < 2; ++n) {
      HRESULT HRGetBuffer = m_swapChain->GetBuffer(n,
                                                   __uuidof(**(&m_renderTargets[n])),
                                                   (void**)(&m_renderTargets[n]));
      dev->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
      rtvHandle.Offset(1, m_rtvDescriptorSize);
    }
  }
  */
}

void
RenderTargetD3D12::create(const Device& device,
                          const std::vector<TextureDesc>& desc,
                          UInt32 numRTs) {
}

void
RenderTargetD3D12::create(const Device& device,
                          const std::vector<Texture*>& textures) {
}

}
