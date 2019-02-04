#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"
#include <tb_pointer.h>

namespace toyboxSDK {

class Device;
class RenderTarget;

class TB_GRAPHICS_EXPORT SwapChain
{
 public:
  /**
  * Class default constructor.
  */
  SwapChain() {};

  /**
  * Class default destructor.
  */
  virtual ~SwapChain() {};
  
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
  virtual void
  create(const Device& device, const SwapChainDesc& desc, void* hwnd) = 0;
  
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
  //getBackBufferRT() = 0;

  ///**
  //* Release the allocated memory.
  //*/
  //virtual void
  //release() = 0;

  ///**
  //* Resize back buffer.
  //* This should be called when the application window is resized.
  //*/
  //virtual void
  //resize(const Device& device, UInt32 _w, UInt32 _h) = 0;

  ///**
  //* Swaps Back and Front buffer.
  //*/
  //virtual void
  //swapBuffers() = 0;
  
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

  SimpleUnique<RenderTarget> m_renderTargets[2];

 private:
};

}