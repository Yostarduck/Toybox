#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"

namespace toyboxSDK {

/**
*
*/
class TB_GRAPHICS_EXPORT Device {
 public:
  
  /**
  * Default class constructor.
  */
  Device();

  /**
  * Default class destructor.
  */
  virtual ~Device();

  /**
  * 
  */
  virtual void
  CreateDevice() = 0;

 protected:
   bool m_useCPU;

 private:

};

}