#pragma once

#include "tb_core_prerequisites.h"
#include "tb_viewport.h"

#include <atomic>

class SDL_Window;

namespace toyboxSDK {

/**
*
*/
class TB_CORE_EXPORT Application
{
 public:

  /**
  * 
  */
  Application(bool usesWindow = true);
  
  /**
  *
  */
  virtual ~Application();
  
  /**
  *
  */
  Int32
  run(const Viewport& viewport);

  /**
  *
  */
  static void
  setViewport(const Viewport& viewport);

  /**
  *
  */
  static Viewport
  getViewPort();

 protected:
  
  /**
  *
  */
  virtual void
  postInit() = 0;

  /**
  *
  */
  virtual void
  postUpdate() = 0;

  /**
  *
  */
  virtual void
  postRender() = 0;

  /**
  *
  */
  virtual void
  postDestroy() = 0;

  /**
  *
  */
  virtual void
  onResize() = 0;

 private:
  
  /**
  *
  */
  void
  init();

  /**
  *
  */
  void
  createWindow();

  /**
  *
  */
  void
  update();

  /**
  *
  */
  void
  render();

  /**
  *
  */
  void
  destroy();

 protected:
  void* m_hwnd;
  Viewport m_viewport;
  std::atomic<bool> m_running;

 private:
  
  /**
  *
  */
  static Application&
  getApplication();

  static Application* application;
  TString m_AppName;
  TString m_AppPath;

  bool m_usesWindow;
  SDL_Window* m_window;
};

}