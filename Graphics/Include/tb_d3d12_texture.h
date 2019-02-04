#pragma once

#include "tb_texture.h"

namespace toyboxSDK {

/**
*
*/
class TB_GRAPHICS_EXPORT TextureD3D12 : public Texture {
 public:
  /**
  * Class default constructor.
  */
  TextureD3D12() = default;

  /**
  * Class default constructor.
  */
  ~TextureD3D12() = default;
   
  /**
  * Inherited from Texture
  */
  void
  CreateFromMemory(const Device& device, 
                   const TextureDesc& desc, 
                   const char* buffer) override;

  /**
  * Inherited from Texture
  */
  void
  CreateEmpty(const Device& device, const TextureDesc& desc) override;

 protected:

 private:
   ID3D12Resource* texture2D;
};

}