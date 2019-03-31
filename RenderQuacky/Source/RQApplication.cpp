#include "RQApplication.h"

#include <tb_file_system.h>
#include <tb_graphics_api.h>
#include <tb_string_conversions.h>

#include <iostream>

#include <FreeImage/FreeImage.h>

#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace toyboxSDK {

RenderQuackyApp::RenderQuackyApp() {
}

RenderQuackyApp::~RenderQuackyApp() {
}

void
RenderQuackyApp::postInit() {
  GraphicsAPI::startUp();
  GraphicsAPI::instance().init(m_viewport.width, m_viewport.height, m_hwnd);

  FileSystem FSys;
  String workingPath = StringConversion::toString(FSys.GetWorkingPath());

  //Camera setting
  std::vector<byte> CBData;
  {
    m_World.identity();

    m_View.identity();
    m_View.LookAt(Vector3(100.0f, 100.0f, 150.0f),
                  Vector3(0.0f, 0.0f, 0.0f),
                  Vector3(0.0f, 1.0f, 0.0f));

    m_Projection.identity();
    m_Projection.ProjectionFov(60.0f * Math::DEGREE_TO_RADIAN,
                                static_cast<float>(m_viewport.width) /
                                                   m_viewport.height,
                               0.0f,
                               10000.0f);

    SizeT matrix4x4Size = sizeof(Matrix4x4);

    CBData.resize(matrix4x4Size * 3);

    std::memcpy(&CBData[matrix4x4Size * 0], &m_World, matrix4x4Size);
    std::memcpy(&CBData[matrix4x4Size * 1], &m_View, matrix4x4Size);
    std::memcpy(&CBData[matrix4x4Size * 2], &m_Projection, matrix4x4Size);
  }

  String GeoRes = workingPath + "Resources\\Geometry\\";
  std::vector<byte> byteIndexList;
  std::vector<byte> byteVertexList;
  SizeT totalIndex;
  //Model loading
  {
    Assimp::Importer importer;

    UInt32 flags = 0;

    flags |= aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
    flags |= aiProcess_RemoveRedundantMaterials;
    flags |= aiProcess_LimitBoneWeights;
    flags |= aiProcess_Triangulate;
    flags |= aiProcess_OptimizeGraph;
    flags |= aiProcess_OptimizeMeshes;
    flags |= aiProcess_JoinIdenticalVertices;
    flags |= aiProcess_FindInvalidData;
    flags |= aiProcess_GenUVCoords;
  
    const aiScene* scene = importer.ReadFile(GeoRes + "Helmet1.fbx",
                                             flags);
    if (scene) {
      //For each mesh
      for (SizeT iMesh = 0; iMesh < scene->mNumMeshes; ++iMesh) {
        aiMesh* mesh = scene->mMeshes[iMesh];

        bool hasNormals = mesh->HasNormals();
        bool hasBinTangs = mesh->HasTangentsAndBitangents();
        bool hasUVs = mesh->HasTextureCoords(0);

        SizeT verticesSize = static_cast<SizeT>(mesh->mNumVertices);
        SizeT totalFaces = static_cast<SizeT>(mesh->mNumFaces);

        std::vector<UInt32> indexList;
        std::vector<VertexInfo> vertexList;

        //Load vertex list
        vertexList.resize(verticesSize);
        for (SizeT vertexIndex = 0; vertexIndex < verticesSize; ++vertexIndex) {
        
          vertexList[vertexIndex].position.x = mesh->mVertices[vertexIndex].x;
          vertexList[vertexIndex].position.y = mesh->mVertices[vertexIndex].y;
          vertexList[vertexIndex].position.z = mesh->mVertices[vertexIndex].z;
          vertexList[vertexIndex].position.w = 1.f;
    
          if (hasNormals) {
            vertexList[vertexIndex].normal.x = mesh->mNormals[vertexIndex].x;
            vertexList[vertexIndex].normal.y = mesh->mNormals[vertexIndex].y;
            vertexList[vertexIndex].normal.z = mesh->mNormals[vertexIndex].z;
            vertexList[vertexIndex].normal.w = 0.f;
          }
    
          if (hasBinTangs) {
            vertexList[vertexIndex].binormal.x = mesh->mBitangents[vertexIndex].x;
            vertexList[vertexIndex].binormal.y = mesh->mBitangents[vertexIndex].y;
            vertexList[vertexIndex].binormal.z = mesh->mBitangents[vertexIndex].z;
            vertexList[vertexIndex].binormal.w = 0.f;

            vertexList[vertexIndex].tangent.x = mesh->mTangents[vertexIndex].x;
            vertexList[vertexIndex].tangent.y = mesh->mTangents[vertexIndex].y;
            vertexList[vertexIndex].tangent.z = mesh->mTangents[vertexIndex].z;
            vertexList[vertexIndex].tangent.w = 0.f;
          }

          if (hasUVs) {
            vertexList[vertexIndex].uv.x = mesh->mTextureCoords[0][vertexIndex].x;
            vertexList[vertexIndex].uv.y = mesh->mTextureCoords[0][vertexIndex].y;
          }
        }

        //Load index list
        for (SizeT iFace = 0; iFace < totalFaces; ++iFace) {
          aiFace& face = mesh->mFaces[iFace];
          TB_ASSERT(face.mNumIndices == 3);
          indexList.push_back(static_cast<UInt32>(face.mIndices[0]));
          indexList.push_back(static_cast<UInt32>(face.mIndices[1]));
          indexList.push_back(static_cast<UInt32>(face.mIndices[2]));
        }

        byteVertexList.resize(vertexList.size() * sizeof(VertexInfo));
        std::memcpy(&byteVertexList[0], &vertexList[0], byteVertexList.size());

        byteIndexList.resize(indexList.size() * sizeof(UInt32));
        std::memcpy(&byteIndexList[0], &indexList[0], byteIndexList.size());

        //totalVertex = vertexList.size();
        totalIndex = indexList.size();
      }
    }
  }
  
  String texRes = workingPath + "Resources\\Textures\\";
  //Texture loading
  FIBITMAP *tgaImg = FreeImage_Load(FIF_TARGA,
                                    (texRes + "Casco1_Normal.tga").c_str(),
                                    TARGA_DEFAULT);
  tgaWidth = 0;
  tgaHeight = 0;
  tgaData = nullptr;
  if (tgaImg) {
    BITMAPINFOHEADER* tgaInfoHeader = FreeImage_GetInfoHeader(tgaImg);
    FREE_IMAGE_TYPE tgaType = FreeImage_GetImageType(tgaImg);
    FREE_IMAGE_COLOR_TYPE tgaColor = FreeImage_GetColorType(tgaImg);
    UInt32 tgaBytes = FreeImage_GetColorsUsed(tgaImg);
    UInt32 tgaBPP = FreeImage_GetBPP(tgaImg);
    UInt32 tgaLine = FreeImage_GetLine(tgaImg); //Returns the width of the bitmap in bytes.
    UInt32 tgaPitch = FreeImage_GetPitch(tgaImg); //Returns the width of the bitmap in bytes, rounded to the next 32-bit boundary, also known as pitch or stride or scan width.
    SizeT tgaSize = FreeImage_GetMemorySize(tgaImg);
    tgaWidth = FreeImage_GetWidth(tgaImg);
    tgaHeight = FreeImage_GetHeight(tgaImg);
    tgaData = FreeImage_GetBits(tgaImg);
    /*
    FIBITMAP *ddsImg = FreeImage_Load(FIF_DDS,
                                      (texRes + "SeaEnviroment.dds").c_str(),
                                      DDS_DEFAULT);
    if (ddsImg) {
      BITMAPINFOHEADER* ddsInfoHeader = FreeImage_GetInfoHeader(ddsImg);
      FREE_IMAGE_TYPE ddsType = FreeImage_GetImageType(ddsImg);
      FREE_IMAGE_COLOR_TYPE ddsColor = FreeImage_GetColorType(ddsImg);
      UInt32 ddsBytes = FreeImage_GetColorsUsed(ddsImg);
      UInt32 ddsBPP = FreeImage_GetBPP(ddsImg);
      UInt32 ddsWidth = FreeImage_GetWidth(ddsImg);
      UInt32 ddsHeight = FreeImage_GetHeight(ddsImg);
      UInt32 ddsLine = FreeImage_GetLine(ddsImg); //Returns the width of the bitmap in bytes.
      UInt32 ddsPitch = FreeImage_GetPitch(ddsImg); //Returns the width of the bitmap in bytes, rounded to the next 32-bit boundary, also known as pitch or stride or scan width.
      SizeT ddsSize = FreeImage_GetMemorySize(ddsImg);

      FreeImage_Unload(ddsImg);
    }
    */
  }

  auto& GraphicAPI = GraphicsAPI::instance();
  auto commandQueue = GraphicAPI.GetCommandQueue();
  auto commandAllocator = GraphicAPI.GetCommandAllocator();
  auto commandList = GraphicAPI.GetCommandList();

  const UInt32 PIX_EVENT_UNICODE_VERSION = 0;
  commandQueue->BeginEvent(PIX_EVENT_UNICODE_VERSION,
                           _T("Init"),
                           (wcslen(_T("Init")) + 1) * sizeof(_T("Init")));
  {
    //Start list
    HRESULT HRCAReset = commandAllocator->Reset();
    if (FAILED(HRCAReset)) {
      std::exception();
    }

    HRESULT HRCLReset = commandList->Reset(commandAllocator, GraphicAPI.GetGPSOForward());
    if (FAILED(HRCLReset)) {
      std::exception();
    }

    //Commands
    GraphicsAPI::instance().UpdateCB(CBData);
    GraphicsAPI::instance().CreateModel(byteVertexList, byteIndexList, totalIndex);
    if (nullptr != tgaData) {
      GraphicsAPI::instance().CreateTexture(tgaWidth,
                                            tgaHeight,
                                            TB_FORMAT::kR32G32B32_FLOAT,
                                            tgaData);

      //FreeImage_Unload(tgaImg);
    }

    //End list
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
    GraphicAPI.ApplyInverter();
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