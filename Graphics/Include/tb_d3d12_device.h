#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"
#include "tb_device.h"

#include <memory>

#include <d3d12.h>
#include <dxgi1_4.h>

namespace toyboxSDK {

/**
*
*/
class TB_GRAPHICS_EXPORT DeviceD3D12 : public Device {
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

  /**
  *
  */
  SwapChain*
  CreateSwapChain(const SwapChainDesc& desc, void* hwnd) const override;

  /**
  */
  IDXGIFactory4*
  GetFactory(UInt32 flags = 0) const;

  ID3D12Device* m_device;
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
  GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

 private:
   ID3D12CommandQueue* m_commandQueue;
   ID3D12CommandAllocator* m_commandAlloc;

};

}