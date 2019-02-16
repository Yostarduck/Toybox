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
  create(const Device* device,
         const TextureDesc& desc,
         UInt32 numRTs) override;
  
  /**
  * Inherited from RenderTarget.
  */
  void
  create(const Device* device,
         const std::vector<TextureDesc>& desc,
         UInt32 numRTs) override;

  /**
  * Inherited from RenderTarget.
  */
  //void
  //create(const Device& device,
  //       const std::vector<Texture*>& textures) override;

 protected:

 private:
  //ID3D12DescriptorHeap* m_cbvsrvHeap; //This is a Device variable

  std::vector<ID3D12Resource*> m_renderTarget;

  //Render target heap
  ID3D12DescriptorHeap* m_rtvHeap;
  D3D12_CPU_DESCRIPTOR_HANDLE m_RTHCPUHeapStart;
  D3D12_GPU_DESCRIPTOR_HANDLE m_RTHGPUHeapStart;
  UInt32 m_rtvDescriptorSize;

  //Shader resource heap.
  ID3D12DescriptorHeap* m_srvHeap;
  D3D12_CPU_DESCRIPTOR_HANDLE m_SRHCPUHeapStart;
  D3D12_GPU_DESCRIPTOR_HANDLE m_SRHGPUHeapStart;
  UInt32 m_srvDescriptorSize;
};

}