#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"

namespace toyboxSDK {

class SwapChain;

/**
*
*/
class TB_GRAPHICS_EXPORT Device {
 public:
  
  /**
  * Default class constructor.
  */
  Device() :m_useCPU(false) {};

  /**
  * Default class destructor.
  */
  virtual ~Device() {};

  /**
  * 
  */
  virtual void
  CreateDevice() = 0;

  /**
  *
  */
  virtual SwapChain*
  CreateSwapChain(const SwapChainDesc& desc, void* hwnd) const = 0;

  
  /**
  *
  */
  FORCEINLINE bool
  UseCPU() const {
    return m_useCPU;
  }

 protected:
   bool m_useCPU;

 private:

};

}