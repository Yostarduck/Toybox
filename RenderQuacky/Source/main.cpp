#include "TestApplication.h"
#include <tb_types.h>
#include <iostream>

toyboxSDK::Int32
main(toyboxSDK::Int32 argc, toyboxSDK::ANSIChar* argv[]) {
  /*
  var graphic API selected.
  var window size.
  var window position.

  if user config file exists.
    Open user config file.
    if file opened.
      Load graphic api selected.
      Load window size.
      Load window position.
    else
      welp fuck you.
  else
    Create user config file.
    if file created.
      Set default graphic API.
      Set default window size.
      Set default window position.
    else
      welp fuck you
  */

  if (argc < 2) {
    std::cout << "App path: " << argv[0] << std::endl;
  }
  else {
    /*
    TODO: Use & parse parameters.
    Set graphic API parameter.
    Set window size parameter.
    Set window position parameter.
    Save to user config file.
    */
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