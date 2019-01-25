#include "tb_graphics_api.h"

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
  
  //Create device.
  //Create device context.
  if (TB_GRAPHICS_API::E::kD3D11 == api) {
    //m_device = std::make_unique<Device>(D3D11Device);
    //m_deviceContext = std::make_unique<DeviceContext>(new D3D11DeviceContext);
  }
  else if (TB_GRAPHICS_API::E::kD3D12 == api) {
    //m_device = std::make_unique<Device>(D3D12Device);
    //m_deviceContext = std::make_unique<DeviceContext>(new D3D12DeviceContext);
  }
  else if (TB_GRAPHICS_API::E::kGL3 == api) {
    //m_device = std::make_unique<Device>(GLDevice);
    //m_deviceContext = std::make_unique<DeviceContext>(new GLDeviceContext);
  }
  else if (TB_GRAPHICS_API::E::kVK_LG == api) {
    //I don't know vulkan :c
    exit(162);
    //1.62 m/(s^2) is the lunar gravity
    //refering to "Vulkan Lunar G"
  }
  else if (TB_GRAPHICS_API::E::kNone == api) {
    //App doesn't require a graphic API.
    return;
  }
  else {
    //How the heck do you arrive here?
    exit(666);
  }

  m_hwnd = hwnd;

  //Create a device and its context.
  //m_device->createDeviceAndDeviceContext(*m_deviceContext);

  //Create swap chain.

  //Create depth stencil.

  //Create rasterizer.

  //Create blend states.

  //Create depth states.
}

}