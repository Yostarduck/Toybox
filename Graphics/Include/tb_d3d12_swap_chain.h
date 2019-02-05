#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"

#include "tb_swap_chain.h"
#include <dxgi1_4.h>
#include <dxgi.h>
#include <d3d12.h>

namespace toyboxSDK {

class TB_GRAPHICS_EXPORT SwapChainD3D12 : public SwapChain
{
 public:
  /**
  * Class default constructor.
  */
  SwapChainD3D12();

  /**
  * Class default destructor.
  */
  ~SwapChainD3D12();
  
  /**
  * Create a swap chain
  *
  * @param device
  *   The device to create the swap chain
  *
  * @param desc
  *   Describes the swap chain params.
  *
  * @return
  *   Return a TB_GRAPHICS_ERROR code, ERROR_NONE means all went well.
  */
  void
  create(const Device& device, const SwapChainDesc& desc, void* hwnd) override;

  ///**
  //* Get the back buffer.
  //*
  //* @param out texture
  //*   The texture to fill out.
  //*
  //* @return
  //*   Return a TB_GRAPHICS_ERROR code, ERROR_NONE means all went well.
  //*/
  //virtual RenderTarget&
  //getBackBufferRT() override;
  //
  ///**
  //* Release the allocated memory.
  //*/
  //virtual void
  //release() override;
  //
  ///**
  //* Resize back buffer.
  //* This should be called when the application window is resized.
  //*/
  //virtual void
  //resize(const Device& device, UInt32 _w, UInt32 _h) override;
  //
  ///**
  //* Swaps Back and Front buffer.
  //*/
  //virtual void
  //swapBuffers() override;
  
  /**
  * Returns a copy of the descriptor.
  *
  * @return
  *   The descriptor which was used to
  *   created this swap chain.
  */
  FORCEINLINE const SwapChainDesc&
  getDescriptor() const {
    return m_descriptor;
  }

 protected:
  SwapChainDesc m_descriptor;

 private:
   IDXGISwapChain3* m_swapChain;
   UInt32 m_frameIndex;
};

}