#include "TestApplication.h"
#include <tb_types.h>
#include <iostream>

toyboxSDK::Int32
main(toyboxSDK::Int32 argc, toyboxSDK::ANSIChar* argv[]) {

  // Check the number of parameters
  if (argc < 2) {
    std::cout << "App path: " << argv[0] << std::endl;
  }
  else {
    //TODO: Use & parse parameters.
    //Uses window parameter.
    //Graphic API parameter.
    //Width parameter.
    //Height parameter.
  }

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