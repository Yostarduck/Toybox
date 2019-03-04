#pragma once

#include <tb_application.h>

//#include <assimp/Importer.hpp>
//#include <FreeImage/FreeImage.h>

namespace toyboxSDK {


enum Inputs
{
  kMouse_Left = 0,
  //kMouse_Right,
  //kMouse_X,
  //kMouse_Y
  kKey_Q
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

};

}