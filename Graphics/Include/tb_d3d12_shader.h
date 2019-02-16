#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"
#include "tb_shader.h"

#include <d3d12.h>

namespace toyboxSDK {

/**
*
*/
class TB_GRAPHICS_EXPORT ShaderD3D12 : public Shader {
 public:
  /**
  * Class default constructor.
  */
  ShaderD3D12() = default;
  
  /**
  * Class default destructor.
  */
  virtual ~ShaderD3D12() = default;
  
  /**
  * Inherited via Shader
  */
  bool
  create(WString filepath,
         TB_SHADER_TYPE::E type) override;

  /**
  * Inherited via Shader
  */
  void
  release() override;

 protected:
   ID3DBlob* shaderBlob;
   ID3DBlob* errorBlob;

   void* bytecodePtr;
   SizeT bytecodeSz;

 private:

};

}