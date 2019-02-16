#pragma once

#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"
#include <tb_types.h>

namespace toyboxSDK {

/**
*
*/
class TB_GRAPHICS_EXPORT Shader {
 public:
  /**
  * Class default constructor.
  */
  Shader() = default;
  
  /**
  * Class default destructor.
  */
  virtual ~Shader() = default;
  
  /**
  * Creates a shader from file.
  *
  * @param filepath
  *   The device to create the texture
  *
  * @return
  *   Returns true if shader was
  *   successfully compiled.
  */
  virtual bool
  create(WString filepath,
         TB_SHADER_TYPE::E type) = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  FORCEINLINE TB_SHADER_TYPE::E
  getShaderType() const {
    return m_shaderType;
  };

 protected:
  TB_SHADER_TYPE::E m_shaderType;

  void* binaryPtr;
  SizeT size;
  String name;

 private:

};

}