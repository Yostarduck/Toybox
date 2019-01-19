#pragma once
#include <tb_application.h>

namespace toyboxSDK {

class RenderQuackyApp : public Application
{
 public:

 private:

  virtual void 
  postInit() override;

  virtual void 
  postUpdate() override;

  virtual void 
  postRender() override;

  virtual void 
  postDestroy() override;

  virtual void
  onResize() override;
};

}