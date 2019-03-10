#include "RQApplication.h"

#include <tb_file_system.h>
#include <tb_graphics_api.h>
#include <tb_string_conversions.h>

#include <iostream>
#include <FreeImage/FreeImage.h>

namespace toyboxSDK {

RenderQuackyApp::RenderQuackyApp() {
}

RenderQuackyApp::~RenderQuackyApp() {
}

void
RenderQuackyApp::postInit() {
  FileSystem FSys;
  String workingPath = StringConversion::toString(FSys.GetWorkingPath());
  String path = workingPath + "Resources\\Textures\\";
  
  FIBITMAP *tgaImg = FreeImage_Load(FIF_TARGA,
                                    (path + "256_Checker_Diffuse.tga").c_str(),
                                    TARGA_DEFAULT);
  {
    FREE_IMAGE_TYPE tgaType = FreeImage_GetImageType(tgaImg);
    UInt32 tgaWidth = FreeImage_GetWidth(tgaImg);
    UInt32 tgaHeight = FreeImage_GetHeight(tgaImg);
    UInt32 tgaLine = FreeImage_GetLine(tgaImg);
    UInt32 tgaPitch = FreeImage_GetPitch(tgaImg);
    SizeT tgaSize = FreeImage_GetMemorySize(tgaImg);
  }
  
  if (tgaImg) { FreeImage_Unload(tgaImg); }

  FIBITMAP *ddsImg = FreeImage_Load(FIF_DDS,
                                    (path + "EarthCubemap.dds").c_str(),
                                    DDS_DEFAULT);
  {
    FREE_IMAGE_TYPE ddsType = FreeImage_GetImageType(ddsImg);
    UInt32 ddsWidth = FreeImage_GetWidth(ddsImg);
    UInt32 ddsHeight = FreeImage_GetHeight(ddsImg);
    UInt32 ddsLine = FreeImage_GetLine(ddsImg);
    UInt32 ddsPitch = FreeImage_GetPitch(ddsImg);
    SizeT ddsSize = FreeImage_GetMemorySize(ddsImg);
  }
  if (ddsImg) { FreeImage_Unload(ddsImg); }
  
  FIBITMAP *hdrImg = FreeImage_Load(FIF_HDR,
                                    (path + "TearsOfSteelBridge_2K.hdr").c_str(),
                                    HDR_DEFAULT);
  {
    FREE_IMAGE_TYPE hdrType = FreeImage_GetImageType(hdrImg);
    UInt32 hdrWidth = FreeImage_GetWidth(hdrImg);
    UInt32 hdrHeight = FreeImage_GetHeight(hdrImg);
    UInt32 hdrLine = FreeImage_GetLine(hdrImg);
    UInt32 hdrPitch = FreeImage_GetPitch(hdrImg);
    SizeT hdrSize = FreeImage_GetMemorySize(hdrImg);
  }
  if (hdrImg) { FreeImage_Unload(hdrImg); }

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