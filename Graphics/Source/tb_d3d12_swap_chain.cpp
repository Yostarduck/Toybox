#include"tb_d3d12_swap_chain.h"
#include "tb_d3d12_device.h"
#include "tb_d3d12_render_target.h"

namespace toyboxSDK {

SwapChainD3D12::SwapChainD3D12() {
}

SwapChainD3D12::~SwapChainD3D12() {
}

void
SwapChainD3D12::create(const Device* device, const SwapChainDesc& desc, void* hwnd) {
  const DeviceD3D12* devd3d12 = reinterpret_cast<const DeviceD3D12*>(device);
  ID3D12Device* dev = devd3d12->m_device;

  // Describe and create the swap chain.
  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = desc.bufferCount;
  swapChainDesc.Width = desc.width;
  swapChainDesc.Height = desc.height;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  IDXGIFactory4* factory = devd3d12->GetFactory();

  IDXGISwapChain1* swapChain;
  //Swap chain needs the queue so that it can force a flush on it.
  HRESULT HRSwapChain = factory->CreateSwapChainForHwnd(devd3d12->m_commandQueue,
                                                        static_cast<HWND>(hwnd),
                                                        &swapChainDesc,
                                                        nullptr,
                                                        nullptr,
                                                        &swapChain);

  if (FAILED(HRSwapChain)) {
    throw std::exception();
  }

  HRESULT HRWindowAssociation = factory->MakeWindowAssociation(static_cast<HWND>(hwnd),
                                                               DXGI_MWA_NO_ALT_ENTER);

  factory->Release();

  // This sample does not support fullscreen transitions.
  if (FAILED(HRWindowAssociation)) {
    throw std::exception();
  }

  HRESULT HRQuerySwapChain = swapChain->QueryInterface(__uuidof(IDXGISwapChain3),
                                                       (void**)&m_swapChain);
  if (FAILED(HRQuerySwapChain)) {
    throw std::exception();
  }

  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
  swapChain->Release();

  //////////////////////////////////////////////////////////////////////////////
  
  TextureDesc backDesc;
  backDesc.dimension = TB_DIMENSION::E::k2D;
  backDesc.width = desc.width;
  backDesc.height = desc.height;
  backDesc.Format = TB_FORMAT::kR8G8B8A8_UNORM;
  backDesc.pitch = backDesc.width * 4 * 1;
  backDesc.mipLevels = 0;
  backDesc.genMipMaps = true;
  backDesc.bindFlags = TB_BIND_FLAGS::E::kSHADER_RESOURCE |
                       TB_BIND_FLAGS::E::kRENDER_TARGET;

  m_renderTargets[0] = tb_simple_unique<RenderTarget>(new RenderTargetD3D12);
  m_renderTargets[1] = tb_simple_unique<RenderTarget>(new RenderTargetD3D12);

  m_renderTargets[0]->create(device, backDesc, 1);


  //////////////////////////////////////////////////////////////////////////////
  //Create Front
  /*
  // Create descriptor heaps.
  {
    // Describe and create a render target view (RTV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = 2;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT HRDescriptorHeap = dev->CreateDescriptorHeap(&rtvHeapDesc,
                                                         __uuidof(**(&m_rtvHeap)),
                                                         (void**)&m_rtvHeap);

    if (FAILED(HRDescriptorHeap)) {
      std::exception();
    }

    m_rtvDescriptorSz = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  }

  // Create frame resources.
  {
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    // Create a RTV for each frame.
    for (UInt32 n = 0; n < 2; ++n)
    {
      HRESULT HRGetBuffer = m_swapChain->GetBuffer(n,
                                                   __uuidof(**(&m_renderTargets[n])),
                                                   (void**)&m_renderTargets[n]);
      if (FAILED(HRGetBuffer)) {
        std::exception();
      }

      dev->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
      rtvHandle.Offset(1, m_rtvDescriptorSz);
    }
  }

  HRESULT HRCommandAlloc = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                       __uuidof(**(&m_commandAllocator)),
                                                       (void**)&m_commandAllocator);

  if (FAILED(HRCommandAlloc)) {
    std::exception();
  }
  */
}

}