#include "tb_d3d12_texture.h"

#include "tb_d3d12_device.h"
#include <vector>

namespace toyboxSDK {

// Returns the number of mip levels given a texture size.
static UInt32
NumMipLevels(UInt32 width, UInt32 height, UInt32 depth = 1) {
  UInt32 numMips = 0;

  UInt32 size = (width > height) ? width : height;
  size = (size > depth) ? size : depth;
  
  while (1 << numMips <= size) {
    ++numMips;
  }
  
  if (1 << numMips < size) {
    ++numMips;
  }
  
  return numMips;
}

void
TextureD3D12::CreateFromMemory(const Device& device,
                               const TextureDesc& desc,
                               const char* buffer) {
}

void
TextureD3D12::CreateEmpty(const Device& device, const TextureDesc& desc) {
  /*
  */
  const DeviceD3D12* devd3d12 = reinterpret_cast<const DeviceD3D12*>(&device);
  ID3D12Device* dev = devd3d12->m_device;


  TB_ASSERT(desc.width > 0);
  TB_ASSERT(desc.height > 0);
  //TB_ASSERT(desc.arraySize > 0);

  D3D12_RESOURCE_DESC texDesc = {};
  texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  //texDesc.Alignment;
  texDesc.Width = desc.width;
  texDesc.Height = desc.height;
  texDesc.DepthOrArraySize = 1;
  texDesc.MipLevels = desc.mipLevels;
  texDesc.Format = static_cast<DXGI_FORMAT>(desc.Format);
  texDesc.SampleDesc.Count = 1;
  texDesc.SampleDesc.Quality = 0;
  texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
  texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

  D3D12_HEAP_PROPERTIES HeapProps;
  HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
  HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  HeapProps.CreationNodeMask = 1;
  HeapProps.VisibleNodeMask = 1;

  D3D12_RESOURCE_STATES resStates = D3D12_RESOURCE_STATE_COMMON;
  
  HRESULT HRCreatedResource = dev->CreateCommittedResource(&HeapProps,
                                                           D3D12_HEAP_FLAG_NONE,
                                                           &texDesc,
                                                           resStates,
                                                           nullptr,
                                                           __uuidof(**(&texture2D)),
                                                           (void**)(&texture2D));

  if (FAILED(HRCreatedResource)) {
    std::exception();
  }


  D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
  srvDesc.Format = static_cast<DXGI_FORMAT>(desc.Format);
  srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

  if (desc.arraySize == 1 && desc.cubeMap == false) {
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = UInt32(desc.mipLevels);
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.PlaneSlice = 0;
    srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
  }
  else if (desc.arraySize > 1 && desc.cubeMap == false) {
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MipLevels = UInt32(desc.mipLevels);
    srvDesc.Texture2DArray.MostDetailedMip = 0;
    srvDesc.Texture2DArray.PlaneSlice = 0;
    srvDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
    srvDesc.Texture2DArray.ArraySize = UInt32(desc.arraySize);
    srvDesc.Texture2DArray.FirstArraySlice = 0;
  }
  else if (desc.arraySize == 1 && desc.cubeMap) {
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MipLevels = UInt32(desc.mipLevels);
    srvDesc.TextureCube.MostDetailedMip = 0;
    srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
  }
  else if (desc.arraySize > 1 && desc.cubeMap) {
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCubeArray.MipLevels = UInt32(desc.mipLevels);
    srvDesc.TextureCubeArray.MostDetailedMip = 0;
    srvDesc.TextureCubeArray.ResourceMinLODClamp = 0.0f;
    srvDesc.TextureCubeArray.First2DArrayFace = 0;
    srvDesc.TextureCubeArray.NumCubes = UInt32(desc.arraySize);
  }

  for (UInt32 i = 0; i < DX12::SRVDescriptorHeap.NumHeaps; ++i) {
    dev->CreateShaderResourceView(texture2D,
                                  &srvDesc,
                                  srvAlloc.Handles[i]);
  }
}

}
