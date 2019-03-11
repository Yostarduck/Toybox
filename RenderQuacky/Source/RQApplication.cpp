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
  FileSystem FSys;
  String workingPath = StringConversion::toString(FSys.GetWorkingPath());
  String texRes = workingPath + "Resources\\Textures\\";
  String GeoRes = workingPath + "Resources\\Geometry\\";

  std::vector<byte> byteIndexList;
  std::vector<byte> byteVertexList;
  SizeT totalFaces;

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
  
  const aiScene* scene = importer.ReadFile(GeoRes + "Cube.fbx",
                                           flags);
  if (scene) {
    //For each mesh
    for (SizeT iMesh = 0; iMesh < scene->mNumMeshes; ++iMesh) {
      aiMesh* mesh = scene->mMeshes[iMesh];

      bool hasNormals = mesh->HasNormals();
      bool hasBinTangs = mesh->HasTangentsAndBitangents();
      bool hasUVs = mesh->HasTextureCoords(0);

      SizeT verticesSize = static_cast<SizeT>(mesh->mNumVertices);
      totalFaces = static_cast<SizeT>(mesh->mNumFaces);

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

      byteIndexList.resize(indexList.size() * sizeof(UInt32));
      byteVertexList.resize(vertexList.size() * sizeof(VertexInfo));

      std::memcpy(&byteIndexList[0], &indexList[0], byteIndexList.size());
      std::memcpy(&byteVertexList[0], &vertexList[0], byteVertexList.size());

      //totalFaces = vertexList.size();
      totalFaces = indexList.size();
    }
  }
  
  FIBITMAP *tgaImg = FreeImage_Load(FIF_TARGA,
                                    (texRes + "256_Checker_Diffuse.tga").c_str(),
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
                                    (texRes + "EarthCubemap.dds").c_str(),
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
                                    (texRes + "TearsOfSteelBridge_2K.hdr").c_str(),
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

  //Camera stuff
  m_World.identity();

  m_View.identity();
  m_View.LookAt(Vector3(-5.0f, 5.0f, -5.0f),
                Vector3(0.0f, 0.0f, 0.0f),
                Vector3(0.0f, 1.0f, 0.0f));

  m_Projection.identity();
  m_Projection.ProjectionFov(60.0f * Math::DEGREE_TO_RADIAN,
                              static_cast<float>(m_viewport.width) /
                                                 m_viewport.height,
                             0.0f,
                             10000.0f);

  GraphicsAPI::startUp();
  GraphicsAPI::instance().init(m_viewport.width, m_viewport.height, m_hwnd);

  SizeT matrix4x4Size = sizeof(Matrix4x4);

  std::vector<byte> CBData;
  CBData.resize(matrix4x4Size * 3);

  Matrix4x4 WVP = m_World * (m_View * m_Projection);

  std::memcpy(&CBData[matrix4x4Size * 0], &m_World, matrix4x4Size);
  std::memcpy(&CBData[matrix4x4Size * 1], &m_View, matrix4x4Size);
  std::memcpy(&CBData[matrix4x4Size * 2], &m_Projection, matrix4x4Size);

  GraphicsAPI::instance().UpdateCB(CBData);
  GraphicsAPI::instance().CreateModel(byteVertexList, byteIndexList, totalFaces);
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