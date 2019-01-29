#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"
#include "tb_device.h"

#include <memory>

namespace toyboxSDK {

/**
*
*/
class TB_GRAPHICS_EXPORT DeviceD3D12 : Device {
 public:
  
  /**
  * Default class constructor.
  */
  DeviceD3D12();

  /**
  * Default class destructor.
  */
  ~DeviceD3D12();

  /**
  * Inherited from Device
  */
  void
  CreateDevice() override;

 protected:
  /**
  * Search a hardware where the
  * device is gonna be running.
  *
  * @param pFactory.
  *   
  *
  * @param ppAdapter.
  *   
  */
  void
  GetHardwareAdapter(IDXGIFactory2* pFactory, std::unique_ptr<IDXGIAdapter1>* ppAdapter);

 private:
   std::unique_ptr<ID3D12Device> m_device;
   std::unique_ptr<ID3D12CommandQueue> m_commandQueue;
   std::unique_ptr<IDXGISwapChain3> m_swapChain;

};

}