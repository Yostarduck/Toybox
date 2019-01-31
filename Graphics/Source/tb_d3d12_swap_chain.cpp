#include"tb_d3d12_swap_chain.h"
#include "tb_d3d12_device.h"

namespace toyboxSDK {

SwapChainD3D12::SwapChainD3D12() {
}

SwapChainD3D12::~SwapChainD3D12() {
}

void
SwapChainD3D12::create(const Device& device, const SwapChainDesc& desc) {
  const DeviceD3D12* dev = reinterpret_cast<const DeviceD3D12*>(&device);
  m_descriptor = desc;

  //Create command queue. (Only for the swap chain)
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

  HRESULT HRCommand = dev->m_device->CreateCommandQueue(&queueDesc,
                                                        __uuidof(**(&m_commandQueue)),
                                                        reinterpret_cast<void**>(&m_commandQueue));

  if (FAILED(HRCommand)) {
    throw std::exception();
  }

	//Swap chain API descriptor.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = desc.bufferCount;
	swapChainDesc.BufferDesc.Width = desc.width;
	swapChainDesc.BufferDesc.Height = desc.height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

  //////////////////////////////////////////////////////////////////////////////
  IDXGIAdapter1* dxgiDevice = nullptr;
  dev->m_device->QueryInterface(__uuidof(**(&dxgiDevice)), (void**)&dxgiDevice);

  IDXGIAdapter1* dxgiAdapter = nullptr;
  dxgiDevice->GetParent(__uuidof(**(&dxgiAdapter)), (void**)&dxgiAdapter);

  IDXGIFactory4* factory = nullptr;
  dxgiAdapter->GetParent(__uuidof(**(&factory)), (void**)&factory);
  //////////////////////////////////////////////////////////////////////////////

  std::unique_ptr<IDXGISwapChain1> swapChain;
  HRESULT HRSwapChain = factory->CreateSwapChain(dev->m_device.get(),
                                                 &swapChainDesc,
                                                 &m_swapChain);

  if (FAILED(HRSwapChain)) {
    std::exception();
  }
  //////////////////////////////////////////////////////////////////////////////

  HRESULT HRQueryInterface = swapChain->QueryInterface(__uuidof(**(&m_swapChain)),
                                                       reinterpret_cast<void**>(&m_swapChain));
  if (FAILED(HRQueryInterface)) {
    std::exception();
  }

  m_frameIndex = m_swapChain.GetCurrentBackBufferIndex();

  dxgiDevice->Release();
  dxgiAdapter->Release();
  factory->Release();
}

}