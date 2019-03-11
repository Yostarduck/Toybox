#pragma once

#include <tb_application.h>
#include <tb_matrix4x4.h>
#include <tb_vector2.h>

//#include <assimp/Importer.hpp>
//#include <FreeImage/FreeImage.h>

namespace toyboxSDK {

struct VertexInfo
{
  Vector4 position;
  Vector4 normal;
  Vector4 binormal;
  Vector4 tangent;
  Vector2 uv;
};

/*
*
*/
class RenderQuackyApp : public Application
{
 public:
  /*
  * Default class constructor.
  */
  RenderQuackyApp();

  /*
  * Default class destructor.
  */
  ~RenderQuackyApp();
 private:
  
  /*
  * Inherited via Application
  */
  virtual void 
  postInit() override;

  /*
  * Inherited via Application
  */
  virtual void 
  postUpdate() override;

  /*
  * Inherited via Application
  */
  virtual void 
  postRender() override;

  /*
  * Inherited via Application
  */
  virtual void 
  postDestroy() override;

  /*
  * Inherited via Application
  */
  virtual void
  onResize() override;

  Matrix4x4 m_World;
  Matrix4x4 m_View;
  Matrix4x4 m_Projection;
};

}