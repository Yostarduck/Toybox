#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"

namespace toyboxSDK {

class Device;

/**
*
*/
class TB_GRAPHICS_EXPORT Texture {
 public:
  /**
  * Class default constructor.
  */
  Texture() = default;
  
  /**
  * Class default destructor.
  */
  virtual ~Texture() = default;
  
  /**
  * Create a texture with initial data
  *
  * @param device
  *   The device to create the texture
  *
  * @param desc
  *   The descriptor of the texture
  *
  * @param buffer
  *   The buffer with initial data
  */
  virtual void
  CreateFromMemory(const Device& device, 
                   const TextureDesc& desc, 
                   const char* buffer) = 0;

  /**
  * Create a texture
  *
  * @param device
  *   The device to create the texture
  *
  * @param desc
  *   The descriptor of the texture
  */
  virtual void
  CreateEmpty(const Device& device, const TextureDesc& desc) = 0;

 protected:

 private:

};

}