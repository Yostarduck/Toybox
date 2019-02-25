#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"

#include <tb_module.h>
#include <tb_pointer.h>

//Hardcoded headers
#include <d3d12.h>
#include <dxgi1_4.h>

namespace toyboxSDK {

//TODO: Implement this classes.
class Device;
class SwapChain;
//class Texture;
//class RenderTarget;
//class BlendState;
//class DepthStencil;
//class DepthStencilState;
//class RasterizerState;

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
  void
  init(UInt32 w,
       UInt32 h,
       void* hwnd,
       TB_GRAPHICS_API::E api = TB_GRAPHICS_API::E::kD3D12);

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
  //SimpleUnique<Device> m_device;
  //SimpleUnique<SwapChain> m_swapChain;

  /**
  /* Inherited via Module.
  */
  void
  onStartUp();
  
  /***
  * Inherited via Module.
  */
  void
  onShutDown();
  
  void* m_hwnd;

 private:
  TB_GRAPHICS_API::E m_SelectedGraphicAPI;

  //Hardcoded stuff

  //Required Flow
  void CreateDevice();
  void CreateCommandQueue();

  //Utilities
  IDXGIFactory4* GetFactory(UInt32 flags = 0);
  void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

  bool m_bUseCPU;
  ID3D12Device* m_device; //Device
  ID3D12CommandQueue* m_commandQueue; //Command Queue
};

}