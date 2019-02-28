#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"

#include <tb_matrix4x4.h>
#include <tb_Vector2.h>
#include <tb_Vector4.h>

#include <tb_module.h>
#include <tb_pointer.h>

//Hardcoded headers
#include <d3d12.h>
#include <dxgi1_4.h>

#include <vector>

namespace toyboxSDK {

//TODO: Implement this classes.
class Device;
class SwapChain;
//class Texture;
//class RenderTarget;
//class BlendState;
//class DepthStencil;
//class DepthStencilState;
//class RasterizerState;

/**
*
*/
class TB_GRAPHICS_EXPORT GraphicsAPI : public Module<GraphicsAPI> {
 public:
  
  /**
  * Default class constructor.
  */
  GraphicsAPI();

  /**
  * Default class destructor.
  */
  ~GraphicsAPI();

  /**
  * Initializes the graphic API if required.
  *
  * Creates a Device and DeviceContext.
  * Holds a reference to the handle window.
  * Creates the swap chain.
  * Creates the depth stencil.
  * Creates the rasterizer.
  * Creates blend states.
  * Creates depth states.
  *
  * @param w
  *   The target width.
  *
  * @param h
  *   The target height.
  *
  * @param hwnd
  *   A pointer to the handle window.
  *
  * @param api
  *   The selected graphic API.
  */
  void
  init(UInt32 w,
       UInt32 h,
       void* hwnd,
       TB_GRAPHICS_API::E api = TB_GRAPHICS_API::E::kD3D12);

  /**
  * Returns which graphic is being used.
  * 
  * @return
  *   m_SelectedGraphicsAPI
  */
  FORCEINLINE const TB_GRAPHICS_API::E
  selectedGraphicAPI() const {
    return m_SelectedGraphicAPI;
  }

 protected:
  //SimpleUnique<Device> m_device;
  //SimpleUnique<SwapChain> m_swapChain;

  /**
  /* Inherited via Module.
  */
  void
  onStartUp();
  
  /***
  * Inherited via Module.
  */
  void
  onShutDown();
  
  UInt32 m_w;
  UInt32 m_h;
  void* m_hwnd;

 private:
  TB_GRAPHICS_API::E m_SelectedGraphicAPI;

  ///////////////////////////////////
  //////////Hardcoded stuff//////////
  ///////////////////////////////////

  //Required Flow
  void CreateDevice();
  void CreateCommandQueue();
  void CreateSwapChainCommanAllocators();
  void CreateFences();
  //Set variables
  void CreateSwapChain();
  void CreateCommandList();
  void CreateShaders();
  void CreateConstantBuffer();
  void CreateQuadVB();
  void CreateQuadIB();

  //Utilities
  bool m_bUseCPU;
  Int32 m_iFrameBuffers;
  Int32 m_iCurrentFrame;

  void
  CompileShader(WString filepath,
                TB_SHADER_TYPE::E type,
                ID3DBlob** shaderBlobOut,
                void** bytecodePtrOut,
                SizeT* bytecodeSzOut);

  IDXGIFactory4* GetFactory(UInt32 flags = 0);
  void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

  ID3D12Device* m_device; //Device
  ID3D12CommandQueue* m_commandQueue; //Command Queue

  std::vector<ID3D12CommandAllocator*> m_commandAllocators;
  ID3D12Fence* m_fence;
  std::vector<UInt64> m_fenceValues;

  IDXGISwapChain1* m_swapChain;
  D3D12_VIEWPORT m_viewport;

  HANDLE m_fenceEvent;

  ID3D12DescriptorHeap* m_rtvHeap;
  UInt32 m_rtvDescriptorSize;
  std::vector<ID3D12Resource*> m_renderTargets;

  ID3D12GraphicsCommandList* m_commandList;

  ID3D12Resource* m_CB;

  ID3D12Resource* m_QuadVB;
  ID3D12Resource* m_QuadIB;

  D3D12_VERTEX_BUFFER_VIEW m_QuadVBView;
  D3D12_INDEX_BUFFER_VIEW m_QuadIBView;


  //GBuffer Shader
  //Vertex
  ID3DBlob* GBufferVSShaderBlob;
  void* GBufferVSBytecodePtr;
  SizeT GBufferVSbytecodeSz;
  //Fragment
  ID3DBlob* GBufferPSShaderBlob;
  void* GBufferPSBytecodePtr;
  SizeT GBufferPSbytecodeSz;

  struct CBuffer {
    Matrix4x4 World;
    Matrix4x4 View;
    Matrix4x4 Projection;
  };

  struct ScreenQuadVertex {
    Vector4 position;
    Vector2 texcoord;
  };
};

}