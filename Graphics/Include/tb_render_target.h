#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"

#include <vector>

namespace toyboxSDK {

//class Texture;
class Device;

/**
*
*/
class TB_GRAPHICS_EXPORT RenderTarget {
 public:
  /**
  * Class default constructor.
  */
  RenderTarget() = default;
  
  /**
  * Class default destructor.
  */
  virtual ~RenderTarget() = default;
  
  /**
  *
  */
  virtual void
  create(const Device* device, const TextureDesc& desc, UInt32 numRTs) = 0;
  
  /**
  *
  */
  virtual void
  create(const Device* device, const std::vector<TextureDesc>& desc, UInt32 numRTs) = 0;

  /**
  *
  */
  //virtual void
  //create(const Device& device, const std::vector<Texture*>& textures) = 0;

 protected:

 private:

};

}