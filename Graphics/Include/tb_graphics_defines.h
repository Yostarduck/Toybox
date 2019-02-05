#pragma once

#include "tb_graphics_prerequisites.h"

namespace toyboxSDK {

namespace TB_GRAPHICS_API
{
enum class E {
  kD3D11,
  kD3D12,
  kGL3,
  kVK_LG,
  kNone
};
}

namespace TB_FILE_FORMAT
{
enum class E {
  kDDS,
  kTGA,
  kPNG,
  kBMP,
  kJPG
};
}

namespace TB_BUFFER_USAGE
{
enum class E {
  kDefault,
  kImmutable,
  kDynamic,
  kStaging
};
}

namespace TB_BLEND
{
enum class E
{
  kBLEND_ZERO             = 1,
  kBLEND_ONE              = 2,
  kBLEND_SRC_COLOR        = 3,
  kBLEND_INV_SRC_COLOR    = 4,
  kBLEND_SRC_ALPHA        = 5,
  kBLEND_INV_SRC_ALPHA    = 6,
  kBLEND_DEST_ALPHA       = 7,
  kBLEND_INV_DEST_ALPHA   = 8,
  kBLEND_DEST_COLOR       = 9,
  kBLEND_INV_DEST_COLOR   = 10,
  kBLEND_SRC_ALPHA_SAT    = 11,
  kBLEND_BLEND_FACTOR     = 14,
  kBLEND_INV_BLEND_FACTOR = 15,
  kBLEND_SRC1_COLOR       = 16,
  kBLEND_INV_SRC1_COLOR   = 17,
  kBLEND_SRC1_ALPHA       = 18,
  kBLEND_INV_SRC1_ALPHA   = 19
};
}

namespace TB_BLEND_OP {
enum class E {
  kBLEND_OP_ADD           = 1,
  kBLEND_OP_SUBTRACT      = 2,
  kBLEND_OP_REV_SUBTRACT  = 3,
  kBLEND_OP_MIN           = 4,
  kBLEND_OP_MAX           = 5
};
}

namespace TB_FORMAT {
enum E {
  kUNKNOWN = 0,
  kR32G32B32A32_TYPELESS = 1,
  kR32G32B32A32_FLOAT = 2,
  kR32G32B32A32_UINT = 3,
  kR32G32B32A32_SINT = 4,
  kR32G32B32_TYPELESS = 5,
  kR32G32B32_FLOAT = 6,
  kR32G32B32_UINT = 7,
  kR32G32B32_SINT = 8,
  kR16G16B16A16_TYPELESS = 9,
  kR16G16B16A16_FLOAT = 10,
  kR16G16B16A16_UNORM = 11,
  kR16G16B16A16_UINT = 12,
  kR16G16B16A16_SNORM = 13,
  kR16G16B16A16_SINT = 14,
  kR32G32_TYPELESS = 15,
  kR32G32_FLOAT = 16,
  kR32G32_UINT = 17,
  kR32G32_SINT = 18,
  kR32G8X24_TYPELESS = 19,
  kD32_FLOAT_S8X24_UINT = 20,
  kR32_FLOAT_X8X24_TYPELESS = 21,
  kX32_TYPELESS_G8X24_UINT = 22,
  kR10G10B10A2_TYPELESS = 23,
  kR10G10B10A2_UNORM = 24,
  kR10G10B10A2_UINT = 25,
  kR11G11B10_FLOAT = 26,
  kR8G8B8A8_TYPELESS = 27,
  kR8G8B8A8_UNORM = 28,
  kR8G8B8A8_UNORM_SRGB = 29,
  kR8G8B8A8_UINT = 30,
  kR8G8B8A8_SNORM = 31,
  kR8G8B8A8_SINT = 32,
  kR16G16_TYPELESS = 33,
  kR16G16_FLOAT = 34,
  kR16G16_UNORM = 35,
  kR16G16_UINT = 36,
  kR16G16_SNORM = 37,
  kR16G16_SINT = 38,
  kR32_TYPELESS = 39,
  kD32_FLOAT = 40,
  kR32_FLOAT = 41,
  kR32_UINT = 42,
  kR32_SINT = 43,
  kR24G8_TYPELESS = 44,
  kD24_UNORM_S8_UINT = 45,
  kR24_UNORM_X8_TYPELESS = 46,
  kX24_TYPELESS_G8_UINT = 47,
  kR8G8_TYPELESS = 48,
  kR8G8_UNORM = 49,
  kR8G8_UINT = 50,
  kR8G8_SNORM = 51,
  kR8G8_SINT = 52,
  kR16_TYPELESS = 53,
  kR16_FLOAT = 54,
  kD16_UNORM = 55,
  kR16_UNORM = 56,
  kR16_UINT = 57,
  kR16_SNORM = 58,
  kR16_SINT = 59,
  kR8_TYPELESS = 60,
  kR8_UNORM = 61,
  kR8_UINT = 62,
  kR8_SNORM = 63,
  kR8_SINT = 64,
  kA8_UNORM = 65,
  kR1_UNORM = 66,
  kR9G9B9E5_SHAREDEXP = 67,
  kR8G8_B8G8_UNORM = 68,
  kG8R8_G8B8_UNORM = 69,
  kBC1_TYPELESS = 70,
  kBC1_UNORM = 71,
  kBC1_UNORM_SRGB = 72,
  kBC2_TYPELESS = 73,
  kBC2_UNORM = 74,
  kBC2_UNORM_SRGB = 75,
  kBC3_TYPELESS = 76,
  kBC3_UNORM = 77,
  kBC3_UNORM_SRGB = 78,
  kBC4_TYPELESS = 79,
  kBC4_UNORM = 80,
  kBC4_SNORM = 81,
  kBC5_TYPELESS = 82,
  kBC5_UNORM = 83,
  kBC5_SNORM = 84,
  kB5G6R5_UNORM = 85,
  kB5G5R5A1_UNORM = 86,
  kB8G8R8A8_UNORM = 87,
  kB8G8R8X8_UNORM = 88,
  kR10G10B10_XR_BIAS_A2_UNORM = 89,
  kB8G8R8A8_TYPELESS = 90,
  kB8G8R8A8_UNORM_SRGB = 91,
  kB8G8R8X8_TYPELESS = 92,
  kB8G8R8X8_UNORM_SRGB = 93,
  kBC6H_TYPELESS = 94,
  kBC6H_UF16 = 95,
  kBC6H_SF16 = 96,
  kBC7_TYPELESS = 97,
  kBC7_UNORM = 98,
  kBC7_UNORM_SRGB = 99,
  kAYUV = 100,
  kY410 = 101,
  kY416 = 102,
  kNV12 = 103,
  kP010 = 104,
  kP016 = 105,
  k420_OPAQUE = 106,
  kYUY2 = 107,
  kY210 = 108,
  kY216 = 109,
  kNV11 = 110,
  kAI44 = 111,
  kIA44 = 112,
  kP8 = 113,
  kA8P8 = 114,
  kB4G4R4A4_UNORM = 115,

  kP208 = 130,
  kV208 = 131,
  kV408 = 132,


  kFORCE_UINT = 0xffffffff
};
}

namespace TB_TEXTURE_ADDRESS
{
enum class E {
  kWrap       = 1,
  kMirror     = 2,
  kClamp      = 3,
  kBorder     = 4,
  kMirrorOnce = 5
};
}

namespace TB_TEXTURE_FILTER
{
enum class E {
  kMIN_MAG_MIP_POINT                          = 0,
  kMIN_MAG_POINT_MIP_LINEAR                   = 0x1,
  kMIN_POINT_MAG_LINEAR_MIP_POINT             = 0x4,
  kMIN_POINT_MAG_MIP_LINEAR                   = 0x5,
  kMIN_LINEAR_MAG_MIP_POINT                   = 0x10,
  kMIN_LINEAR_MAG_POINT_MIP_LINEAR            = 0x11,
  kMIN_MAG_LINEAR_MIP_POINT                   = 0x14,
  kMIN_MAG_MIP_LINEAR                         = 0x15,
  kANISOTROPIC                                = 0x55,
  kCOMPARISON_MIN_MAG_MIP_POINT               = 0x80,
  kCOMPARISON_MIN_MAG_POINT_MIP_LINEAR        = 0x81,
  kCOMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT  = 0x84,
  kCOMPARISON_MIN_POINT_MAG_MIP_LINEAR        = 0x85,
  kCOMPARISON_MIN_LINEAR_MAG_MIP_POINT        = 0x90,
  kCOMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
  kCOMPARISON_MIN_MAG_LINEAR_MIP_POINT        = 0x94,
  kCOMPARISON_MIN_MAG_MIP_LINEAR              = 0x95,
  kCOMPARISON_ANISOTROPIC                     = 0xd5,
  kMINIMUM_MIN_MAG_MIP_POINT                  = 0x100,
  kMINIMUM_MIN_MAG_POINT_MIP_LINEAR           = 0x101,
  kMINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT     = 0x104,
  kMINIMUM_MIN_POINT_MAG_MIP_LINEAR           = 0x105,
  kMINIMUM_MIN_LINEAR_MAG_MIP_POINT           = 0x110,
  kMINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR    = 0x111,
  kMINIMUM_MIN_MAG_LINEAR_MIP_POINT           = 0x114,
  kMINIMUM_MIN_MAG_MIP_LINEAR                 = 0x115,
  kMINIMUM_ANISOTROPIC                        = 0x155,
  kMAXIMUM_MIN_MAG_MIP_POINT                  = 0x180,
  kMAXIMUM_MIN_MAG_POINT_MIP_LINEAR           = 0x181,
  kMAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT     = 0x184,
  kMAXIMUM_MIN_POINT_MAG_MIP_LINEAR           = 0x185,
  kMAXIMUM_MIN_LINEAR_MAG_MIP_POINT           = 0x190,
  kMAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR    = 0x191,
  kMAXIMUM_MIN_MAG_LINEAR_MIP_POINT           = 0x194,
  kMAXIMUM_MIN_MAG_MIP_LINEAR                 = 0x195,
  kMAXIMUM_ANISOTROPIC                        = 0x1d5
};
}

namespace TB_PRIMITIVE_TOPOLOGY
{
enum class E {
  kTriangleList,
  kTriangleStrip,
  kLineList,
  kLineStrip,
  kPoint
};
}

namespace TB_DEPTH_STENCIL_CLEAR_TYPE
{
enum class E {
  kClearDepth,
  kClearStencil,
  kClearDepthStencil
};
}

namespace TB_SHADER_TYPE_FLAG {
enum class E {
  //Common pipeline
  kVertex     = 1 << 0,
  kHull       = 1 << 1,
  kTeselation = 1 << 2,
  kDomain     = 1 << 3,
  kGeometry   = 1 << 4,
  kFragment   = 1 << 5,
  //New pipeline
  kMesh       = 1 << 6,
  kRaytracing = 1 << 7,
  //Out-pipeline
  kCompute    = 1 << 8,
  //Strange unknown shader
  kTexture    = 1 << 9,
};
}

namespace TB_CPU_ACCESS_FLAG
{
enum E {
  kWrite = 0x10000L,
  kRead  = 0x20000L
};
}

namespace TB_FILL_MODE
{
enum class E {
  kWireframe  = 2,
  kSolid      = 3
};
}

namespace TB_CULL_MODE
{
enum class E {
  kNone   = 1,
  kFront  = 2,
  kBack   = 3
};
}

namespace TB_TEXTURE_TYPE
{
enum class E {
  k1D,
  k2D,
  k3D
};
}

namespace TB_COMPARISON_FUNC
{
enum class E {
  kNEVER          = 1,
  kLESS           = 2,
  kEQUAL          = 3,
  kLESS_EQUAL     = 4,
  kGREATER        = 5,
  kNOT_EQUAL      = 6,
  kGREATER_EQUAL  = 7,
  kALWAYS         = 8
};
}

namespace TB_SWAP_EFFECT
{
enum class E {
  kEFFECT_DISCARD         = 0,
  kEFFECT_SEQUENTIAL      = 1,
  kEFFECT_FLIP_SEQUENTIAL = 3,
  kEFFECT_FLIP_DISCARD    = 4
};
}

namespace TB_BUFFER_TYPE
{
enum class E {
  kVERTEX,
  kINDEX,
  kCONSTANT,
  kCOMMAND,
  kRWSTRUCTURE,
  kSTRUCTURE,
  kINDIRECT_DRAW_INSTANCED_INDEXED,
  kINDIRECT_DISPATCH
};
}

namespace TB_DIMENSION
{
enum class E {
  k1D,
  k2D,
  k3D,
  kCUBE_MAP
};
}

namespace TB_IB_TYPE
{
enum class E {
  k16,
  k32
};
}

namespace TB_BIND_FLAGS
{
enum E {
  kVERTEX_BUFFER    = 0x1L,
  kINDEX_BUFFER     = 0x2L,
  kCONSTANT_BUFFER  = 0x4L,
  kSHADER_RESOURCE  = 0x8L,
  kSTREAM_OUTPUT    = 0x10L,
  kRENDER_TARGET    = 0x20L,
  kDEPTH_STENCIL    = 0x40L,
  kUNORDERED_ACCESS = 0x80L,
  kDECODER          = 0x200L,
  kVIDEO_ENCODER    = 0x400L
};
}

namespace TB_DEPTH_WRITE_MASK
{
enum class E {
  kMASK_ZERO  = 0,
  kMASK_ALL   = 1
};
}

namespace TB_INPUT_CLASSIFICATION
{
enum class E {
  kPerVertex    = 0,
  kPerInstance  = 1
};
}

struct TB_GRAPHICS_EXPORT InputElementDesc {
  String semanticName;
  UInt32 semanticIndex;
  TB_FORMAT::E format;
  UInt32 inputSlot;
  UInt32 offset;
  TB_INPUT_CLASSIFICATION::E slotClass;
  UInt32 stepRate;
  InputElementDesc() {
    semanticName = "";
    semanticIndex = 0;
    format = TB_FORMAT::E::kB4G4R4A4_UNORM;
    inputSlot = 0;
    offset = 0;
    slotClass = TB_INPUT_CLASSIFICATION::E::kPerVertex;
    stepRate = 0;
  }
};

struct TB_GRAPHICS_EXPORT SampleDesc {
  TB_TEXTURE_FILTER::E Filter;
  TB_TEXTURE_ADDRESS::E addressU;
  TB_TEXTURE_ADDRESS::E addressV;
  TB_TEXTURE_ADDRESS::E addressW;
  float mipLODBias;
  UInt32 maxAnisotropy;
  TB_COMPARISON_FUNC::E comparisonFunc;
  float borderColor[4];
  float minLOD;
  float maxLOD;
  SampleDesc() {
    Filter = TB_TEXTURE_FILTER::E::kMIN_MAG_LINEAR_MIP_POINT;
    addressU = TB_TEXTURE_ADDRESS::E::kClamp;
    addressV = TB_TEXTURE_ADDRESS::E::kClamp;
    addressW = TB_TEXTURE_ADDRESS::E::kClamp;
    mipLODBias = 0;
    maxAnisotropy = 1;
    comparisonFunc = TB_COMPARISON_FUNC::E::kNEVER;
    borderColor[0] = 0.0f;
    borderColor[1] = 0.0f;
    borderColor[2] = 0.0f;
    borderColor[3] = 0.0f;
    minLOD = 0.0f;
    maxLOD = 0.0f; //3.402823466e+38F;
  }
};

struct TB_GRAPHICS_EXPORT BufferDesc {
  TB_BUFFER_USAGE::E usage;
  TB_BUFFER_TYPE::E type;
  UInt32 stride;
  UInt32 sizeInBytes;
  BufferDesc() {
    usage = TB_BUFFER_USAGE::E::kDefault;
    type = TB_BUFFER_TYPE::E::kCONSTANT;
    stride = 0;
    sizeInBytes = 0;
  }
};

struct TB_GRAPHICS_EXPORT TextureDesc {
  TB_FORMAT::E Format;
  TB_BUFFER_USAGE::E Usage;
  TB_DIMENSION::E dimension;
  UInt32 width;
  UInt32 height;
  UInt32 pitch;
  UInt32 mipLevels;
  UInt32 CPUAccessFlags;
  UInt32 bindFlags;
  UInt32 arraySize;
  bool genMipMaps;
  bool cubeMap;
  TextureDesc() {
    Format = TB_FORMAT::E::kB4G4R4A4_UNORM;
    Usage = TB_BUFFER_USAGE::E::kDefault;
    dimension = TB_DIMENSION::E::k2D;
    width = 0;
    height = 0;
    pitch = 0;
    mipLevels = 0;
    CPUAccessFlags = 0;
    bindFlags = 0;
    arraySize = 1;
    genMipMaps = false;
    cubeMap = false;
  }
};

struct TB_GRAPHICS_EXPORT DepthStencilDesc {
  TB_FORMAT::E Format;
  UInt32 width;
  UInt32 height;
  UInt32 bindFlags;

  DepthStencilDesc() {
    Format = TB_FORMAT::E::kD24_UNORM_S8_UINT;
    width = 0;
    height = 0;
    bindFlags = TB_BIND_FLAGS::kDEPTH_STENCIL;
  }
};

struct TB_GRAPHICS_EXPORT RasterizerDesc {
  TB_FILL_MODE::E fillMode;
  TB_CULL_MODE::E cullMode;
  bool  frontCounterClockwise;
  Int32 depthBias;
  float depthBiasClamp;
  float slopeScaledDepthBias;
  bool depthClipEnable;
  bool scissorEnable;
  bool multisampleEnable;
  bool antialiasedLineEnable;
  RasterizerDesc() {
    fillMode = TB_FILL_MODE::E::kSolid;
    cullMode = TB_CULL_MODE::E::kBack;
    frontCounterClockwise = true;
    depthBias = 1;
    depthBiasClamp = 0.0f;
    slopeScaledDepthBias = 0.0f;
    depthClipEnable = true;
    scissorEnable = true;
    multisampleEnable = true;
    antialiasedLineEnable = true;
  }
};

struct TB_GRAPHICS_EXPORT BlendStateDesc {
  bool blendEnable;
  TB_BLEND::E srcBlend;
  TB_BLEND::E destBlend;
  TB_BLEND::E srcBlendAlpha;
  TB_BLEND::E destBlendAlpha;
  TB_BLEND_OP::E blendOp;
  TB_BLEND_OP::E blendOpAlpha;
  BlendStateDesc() {
    blendEnable = true;
  }
};

struct TB_GRAPHICS_EXPORT RationalNumber {
  UInt32 numerator;
  UInt32 denominator;
  RationalNumber() {
    numerator = 0;
    denominator = 1;
  }
};

struct TB_GRAPHICS_EXPORT SwapChainDesc  {
  UInt32 width;
  UInt32 height;
  RationalNumber refreshRate;
  TB_FORMAT::E Format;
  UInt32 bufferCount;
  void* windowHandler;
  bool windowed;
  SwapChainDesc() {
    width = 0;
    height = 0;
    RationalNumber ref;
    ref.denominator = 0;
    ref.numerator = 0;
    refreshRate = ref;
    Format = TB_FORMAT::E::kUNKNOWN;
    bufferCount = 0;
    windowHandler = nullptr;
    windowed = true;
  }
};

struct TB_GRAPHICS_EXPORT DepthStencilStateDesc {
  bool depthEnable;
  TB_COMPARISON_FUNC::E depthFunc; 
  bool stencilEnable;
  UInt8 stencilReadMask;
  UInt8 stencilWriteMask;
  TB_DEPTH_WRITE_MASK::E depthWriteMask;
  DepthStencilStateDesc() {
    depthEnable = true;
    depthFunc = TB_COMPARISON_FUNC::E::kLESS_EQUAL;
    stencilEnable = false;
    stencilReadMask = 0;
    stencilWriteMask = 0;
    depthWriteMask = TB_DEPTH_WRITE_MASK::E::kMASK_ALL;
  }
};

}