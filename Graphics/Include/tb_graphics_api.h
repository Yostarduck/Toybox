#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"
#include <tb_module.h>

namespace toyboxSDK {

//TODO: Implement this classes.
class Device;
class DeviceContext;
class SwapChain;
class Texture;
class RenderTarget;
class BlendState;
class DepthStencil;
class DepthStencilState;
class RasterizerState;

/**
*
*/
class TB_GRAPHICS_EXPORT GraphicsAPI : public Module<GraphicsAPI> {
 public:
  
  /**
  * Default class constructor.
  */
  GraphicsAPI();

  /**
  * Default class destructor.
  */
  ~GraphicsAPI();

  /***
  * Inherited via Module.
  */
  virtual void
  onStartUp() override;

  /***
  * Inherited via Module.
  */
  virtual void
  onShutDown() override;

  /**
  * Initializes the graphic API if required.
  *
  * Creates a Device and DeviceContext.
  * Holds a reference to the handle window.
  * Creates the swap chain.
  * Creates the depth stencil.
  * Creates the rasterizer.
  * Creates blend states.
  * Creates depth states.
  *
  * @param w
  *   The target width.
  *
  * @param h
  *   The target height.
  *
  * @param hwnd
  *   A pointer to the handle window.
  *
  * @param api
  *   The selected graphic API.
  */
  virtual void
  init(UInt32 w,
       UInt32 h,
       void* hwnd,
       TB_GRAPHICS_API::E api = TB_GRAPHICS_API::E::kD3D11);

  /**
  * Returns which graphic is being used.
  * 
  * @return
  *   m_SelectedGraphicsAPI
  */
  FORCEINLINE TB_GRAPHICS_API::E
  selectedGraphicAPI() {
    return m_SelectedGraphicAPI;
  }

 protected:
   std::unique_ptr<Device> m_device;
   std::unique_ptr<DeviceContext> m_deviceContext;
   
   void* m_hwnd;

 private:
  TB_GRAPHICS_API::E m_SelectedGraphicAPI;
};

}