#include "tb_d3d12_texture.h"

#include "tb_d3d12_device.h"
#include <vector>

namespace toyboxSDK {

void
TextureD3D12::CreateFromMemory(const Device& device,
                               const TextureDesc& desc,
                               const char* buffer) {
}

void
TextureD3D12::CreateEmpty(const Device& device, const TextureDesc& desc) {
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
  texDesc.DepthOrArraySize = 1; //Hardcoded
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
}

}
