#include "tb_d3d12_shader.h"

#include <tb_string_utils.h>

#include <D3Dcompiler.h>

namespace toyboxSDK {

bool
ShaderD3D12::create(WString filepath, TB_SHADER_TYPE::E type) {
  
#if TB_DEBUG_MODE
  // Enable better shader debugging with the graphics debugging tools.
  UInt32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  UInt32 compileFlags = 0;
#endif
  String shaderModel;
  switch (type)
  {
  case toyboxSDK::TB_SHADER_TYPE::E::kVertex:
    shaderModel = "vs_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kHull:
    shaderModel = "hs_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kTeselation:
    exit(666); //This doesn't exist in directx.
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kDomain:
    shaderModel = "ds_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kGeometry:
    shaderModel = "gs_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kFragment:
    shaderModel = "ps_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kMesh:
    exit(666); //This doesn't exists yet in directx.
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kRaytracing:
    exit(666); //This isn't a shader yet.
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kCompute:
    shaderModel = "cs_5_0";
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kTexture:
    exit(666); //This never has existed.
    break;
  case toyboxSDK::TB_SHADER_TYPE::E::kNone:
    break;
  default:
    break;
  }

  HRESULT HRShader = D3DCompileFromFile(filepath.c_str(),
                                        nullptr,
                                        D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                        "main",
                                        shaderModel.c_str(),
                                        compileFlags,
                                        0,
                                        &shaderBlob,
                                        &errorBlob);

  if (FAILED(HRShader)) {
    std::exception();
    return false;
  }

  bytecodePtr = shaderBlob->GetBufferPointer();
  bytecodeSz = shaderBlob->GetBufferSize();

  return true;
}

void
ShaderD3D12::release() {
}

}