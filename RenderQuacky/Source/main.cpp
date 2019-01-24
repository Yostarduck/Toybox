#include <tb_types.h>
#include <iostream>
#include "TestApplication.h"

toyboxSDK::Int32
main(toyboxSDK::Int32 argc, toyboxSDK::ANSIChar* argv[]) {

  // Check the number of parameters
  if (argc < 2) {
    std::cout << "App path: " << argv[0] << std::endl;
  }
  else {
    //Parse parameters.
    //TODO: use parameters to select graphic api.
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