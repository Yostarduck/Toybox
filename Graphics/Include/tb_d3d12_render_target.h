#pragma once

#include "tb_render_target.h"

#include <tb_pointer.h>

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
  RenderTargetD3D12();

  /**
  * Class default destructor.
  */
  ~RenderTargetD3D12();

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
  ID3D12DescriptorHeap* m_rtvHeap;
  UInt32 m_rtvDescriptorSize;

  //std::vector<std::unique_ptr<ID3D12Resource>> m_renderTarget;

};

}