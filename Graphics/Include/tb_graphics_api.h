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

static void
AddResourceBarrier(ID3D12GraphicsCommandList* command,
                   ID3D12Resource* pResource,
                   D3D12_RESOURCE_STATES before,
                   D3D12_RESOURCE_STATES after) {
  D3D12_RESOURCE_BARRIER desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
  desc.Transition.pResource = pResource;
  desc.Transition.StateBefore = before;
  desc.Transition.StateAfter = after;
  command->ResourceBarrier(1, &desc);
};

//TODO: Implement this classes.
//class Device;
//class SwapChain;
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

  void
  ApplyGBuffer();

  void
  ApplyForward();

  void
  PresentFrame();

  void
  WaitForGPU();

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

  FORCEINLINE ID3D12CommandQueue*
  GetCommandQueue() {
    return m_commandQueue;
  }

  FORCEINLINE ID3D12CommandAllocator*
  GetCommandAllocator() {
    return m_commandAllocators[m_iCurrentFrame];
  }

  FORCEINLINE ID3D12GraphicsCommandList*
  GetCommandList() {
    return m_commandList;
  }

  FORCEINLINE ID3D12Resource*
  GetBackBuffer() {
    return m_renderTargets[m_iCurrentFrame];
  }

  FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE
  GetRenderTargetView() const {
    D3D12_CPU_DESCRIPTOR_HANDLE handle;
    handle.ptr = m_rtvHeap->GetCPUDescriptorHandleForHeapStart().ptr + (m_rtvDescriptorSize * m_iCurrentFrame);
    return handle;
  }

  FORCEINLINE D3D12_VIEWPORT
  GetScreenViewport() const {
    return m_viewport;
  }

  FORCEINLINE ID3D12PipelineState*
    GetGPSOGBuffer() const {
    return m_GPSOGBuffer;
  }

  FORCEINLINE ID3D12PipelineState*
  GetGPSOForward() const {
    return m_GPSOForward;
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
  ///Set variables
  void CreateSwapChain();
  void CreateCommandList();
  void CreateShaders();
  void CreateConstantBuffer();
  void CreateQuadVB();
  void CreateQuadIB();
  void CreateShaderHeap();
  void CreateGBufferRootSignature();
  void CreateForwardRootSignature();
  void CreateGPSOGBuffer();
  void CreateGPSOForward();
  void CreateDSV();
  void CreateRTV();

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

  //Shader Heap
  ID3D12DescriptorHeap* m_ShaderDHPtr;
  D3D12_CPU_DESCRIPTOR_HANDLE m_ShaderCPUHeapStartHandle;
  D3D12_GPU_DESCRIPTOR_HANDLE m_ShaderGPUHeapStartHandle;
  UInt32 m_SHandleIncrementSize;

  ID3D12RootSignature* m_GBufferRootSignature;
  ID3D12RootSignature* m_ForwardRootSignature;

  ID3D12PipelineState* m_GPSOGBuffer;

  ID3D12PipelineState* m_GPSOForward;

  //GBuffer Shader
  //Vertex
  ID3DBlob* GBufferVSShaderBlob;
  void* GBufferVSBytecodePtr;
  SizeT GBufferVSbytecodeSz;
  //Fragment
  ID3DBlob* GBufferPSShaderBlob;
  void* GBufferPSBytecodePtr;
  SizeT GBufferPSbytecodeSz;
  //Forward Shader
  //Vertex
  ID3DBlob* ForwardVSShaderBlob;
  void* ForwardVSBytecodePtr;
  SizeT ForwardVSbytecodeSz;
  //Fragment
  ID3DBlob* ForwardPSShaderBlob;
  void* ForwardPSBytecodePtr;
  SizeT ForwardPSbytecodeSz;

  //Depth Stencil Texture
  ID3D12Resource* m_DSTexture;
  ID3D12DescriptorHeap* m_DSDHPtr;
  D3D12_CPU_DESCRIPTOR_HANDLE m_DSCPUHeapStartHandle;
  D3D12_GPU_DESCRIPTOR_HANDLE m_DSGPUHeapStartHandle;
  UINT m_DSHandleIncrementSize;

  //RTV Texture
  std::vector<ID3D12Resource*> m_RTTexture;
  ID3D12DescriptorHeap* m_RTDHPtr;
  D3D12_CPU_DESCRIPTOR_HANDLE m_RTCPUHeapStartHandle;
  D3D12_GPU_DESCRIPTOR_HANDLE m_RTGPUHeapStartHandle;
  UINT m_RTHandleIncrementSize;

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