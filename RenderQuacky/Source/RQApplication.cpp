#include "RQApplication.h"
#include <tb_graphics_api.h>

#include <iostream>

namespace toyboxSDK {

RenderQuackyApp::RenderQuackyApp() {
}

RenderQuackyApp::~RenderQuackyApp() {
}

void
RenderQuackyApp::postInit() {
  GraphicsAPI::startUp();
  GraphicsAPI::instance().init(m_viewport.width, m_viewport.height, m_hwnd);
}

void
RenderQuackyApp::postUpdate() {
  auto commandQueue = GraphicsAPI::instance().GetCommandQueue();

  const UInt32 PIX_EVENT_UNICODE_VERSION = 0;
  commandQueue->BeginEvent(PIX_EVENT_UNICODE_VERSION,
                           _T("Update"),
                           (wcslen(_T("Update")) + 1) * sizeof(_T("Update")));
  {
    //mCamera.Update();
    //mCamData.MVP = mCamera.ProjView();
    //mCamData.InvPV = mCamera.InvScreenProjView();
    //mCamData.CamPos = mCamera.Position();
    //mDeferredTech.UpdateConstantBuffer(mCamData, mLightData);
  }
  commandQueue->EndEvent();
}

void
RenderQuackyApp::postRender() {
  auto& GraphicAPI = GraphicsAPI::instance();
  auto commandQueue = GraphicAPI.GetCommandQueue();
  auto commandAllocator = GraphicAPI.GetCommandAllocator();
  auto commandList = GraphicAPI.GetCommandList();

  const UInt32 PIX_EVENT_UNICODE_VERSION = 0;
  commandQueue->BeginEvent(PIX_EVENT_UNICODE_VERSION,
                           _T("Render"),
                           (wcslen(_T("Render")) + 1) * sizeof(_T("Render")));
  {
    HRESULT HRCAReset = commandAllocator->Reset();
    if (FAILED(HRCAReset)) {
      std::exception();
    }

    HRESULT HRCLReset = commandList->Reset(commandAllocator, GraphicAPI.GetGPSOGBuffer());
    if (FAILED(HRCLReset)) {
      std::exception();
    }
   
    AddResourceBarrier(commandList,
                       GraphicAPI.GetBackBuffer(),
                       D3D12_RESOURCE_STATE_PRESENT,
                       D3D12_RESOURCE_STATE_RENDER_TARGET);

    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    commandList->ClearRenderTargetView(GraphicAPI.GetRenderTargetView(), clearColor, 0, nullptr);
    D3D12_RECT rect = { 0, 0, m_viewport.width , m_viewport.height };

    commandList->RSSetScissorRects(1, &rect);
    commandList->RSSetViewports(1, &GraphicAPI.GetScreenViewport());

    GraphicAPI.ApplyGBuffer();
    GraphicAPI.ApplyForward();

    AddResourceBarrier(commandList, GraphicAPI.GetBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

    HRESULT HRCLClose = commandList->Close();
    if (FAILED(HRCLClose)) {
      std::exception();
    }
    ID3D12CommandList* ppCommandLists[1] = { commandList };

    commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    GraphicAPI.WaitForGPU();

    GraphicAPI.PresentFrame();

  }
  commandQueue->EndEvent();
}

void
RenderQuackyApp::postDestroy() {
}

void
RenderQuackyApp::onResize() {
}

}