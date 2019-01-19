#include <tb_types.h>
#include "TestApplication.h"

toyboxSDK::Int32
main() {
  toyboxSDK::RenderQuackyApp app;

  toyboxSDK::Viewport vp;
  vp.width = 1280;
  vp.height = 720;
  vp.minDepth = 0.0f;
  vp.maxDepth = 1.0f;
  vp.topLeftX = 0.0f;
  vp.topLeftY = 0.0f;

  return app.run(vp);
}