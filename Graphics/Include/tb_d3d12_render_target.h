#pragma once

#include "tb_render_target.h"

#include <dxgi1_4.h>
#include <dxgi.h>
#include <d3d12.h>

namespace toyboxSDK {

class TB_GRAPHICS_EXPORT RenderTargetD3D12 : public RenderTarget
{
 public:
  /**
  * Class default constructor.
  */
  RenderTargetD3D12() = default;

  /**
  * Class default destructor.
  */
  ~RenderTargetD3D12() = default;

  /**
  * Inherited from RenderTarget.
  */
  void
  create(const Device& device,
         const TextureDesc& desc,
         UInt32 numRTs) override;
  
  /**
  * Inherited from RenderTarget.
  */
  void
  create(const Device& device,
         const std::vector<TextureDesc>& desc,
         UInt32 numRTs) override;

  /**
  * Inherited from RenderTarget.
  */
  void
  create(const Device& device,
         const std::vector<Texture*>& textures) override;

 protected:

 private:
   std::vector<std::unique_ptr<ID3D12Resource>> m_renderTarget;

};

}