#include "tb_graphics_api.h"

#include "tb_device.h"
#include "tb_swap_chain.h"
#include "tb_render_target.h"

#include "tb_d3d12_device.h"
#include "tb_d3d12_swap_chain.h""
#include "tb_d3d12_render_target.h"


namespace toyboxSDK {

GraphicsAPI::GraphicsAPI() {
}

GraphicsAPI::~GraphicsAPI() {
}

void
GraphicsAPI::onStartUp() {
}

void
GraphicsAPI::onShutDown() {
}

void
GraphicsAPI::init(UInt32 w,
                  UInt32 h,
                  void* hwnd,
                  TB_GRAPHICS_API::E api) {
  //Select device.
  switch (api)
  {
  case toyboxSDK::TB_GRAPHICS_API::E::kD3D12:
    m_device = tb_simple_unique<Device>(new DeviceD3D12);
    break;
  case toyboxSDK::TB_GRAPHICS_API::E::kD3D11:
    //m_device = tb_simple_unique<Device>(new DeviceD3D11);
    //m_deviceContext = tb_simple_unique<DeviceContext>(new D3D11DeviceContext);
    break;
  case toyboxSDK::TB_GRAPHICS_API::E::kGL3:
    //m_device = tb_simple_unique<Device>(DeviceGL);
    //m_deviceContext = tb_simple_unique<DeviceContext>(new GLDeviceContext);
    break;
  case toyboxSDK::TB_GRAPHICS_API::E::kVK_LG:
    //I don't know vulkan :c
    exit(162);
    //1.62 m/(s^2) is the lunar gravity.
    //Because "Vulkan Lunar G"
    break;
  case toyboxSDK::TB_GRAPHICS_API::E::kNone:
    //App doesn't require a graphic API.
    break;
  default:
    //How the heck do you arrive here?
    exit(666);
    break;
  }

  m_device->CreateDevice();
  
  m_hwnd = hwnd;
  
  //Create swap chain.
  SwapChainDesc swapDesc;
  swapDesc.bufferCount = 2;
  swapDesc.windowed = true;
  swapDesc.windowHandler = hwnd;
  swapDesc.height = h;
  swapDesc.width = w;
  
  m_swapChain = tb_simple_unique<SwapChain>(m_device->CreateSwapChain(swapDesc, hwnd));
  
  TextureDesc td;
  td.width = 1280;
  td.height = 720;
  td.Format = TB_FORMAT::kR32G32B32A32_FLOAT;
  
  RenderTarget* RTTest = new RenderTargetD3D12;
  RTTest->create(m_device.get(), td, 3);

  //m_swapChain = m_device->CreateSwapChain(swapDesc, hwnd);

  //Create a device and its context.
  //m_device->createDeviceAndDeviceContext(*m_deviceContext);

  //Create depth stencil.

  //Create rasterizer.

  //Create blend states.

  //Create depth states.
}

}