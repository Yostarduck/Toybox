#include "tb_application.h"

#include <iostream>
#include <Windows.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

namespace toyboxSDK {

Application* Application::application;

Application::Application(bool usesWindow)
  :  m_usesWindow(usesWindow) {
}

Application::~Application() {
}

Int32
Application::run(const Viewport& viewport) {
  if (Application::application) {
    std::cout << "Application already running" << std::endl;
    return 0;
  }

  Application::application = this;
  m_viewport = viewport;

  init();

  while (m_running) {
    update();
    render();
  }

  destroy();

  return 0;
}

void
Application::setViewport(const Viewport& viewport) {
  Application& app = getApplication();
  app.m_viewport = viewport;
  SDL_SetWindowSize(app.m_window, app.m_viewport.width, app.m_viewport.height);
}

Viewport
Application::getViewPort() {
  return getApplication().m_viewport;
}

void 
Application::init() {
  m_running = true;
    
  if (m_usesWindow) {
    createWindow();
    m_hwnd = GetActiveWindow();
  }
  
  postInit();
}

void 
Application::createWindow() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Video initialization failed: " << SDL_GetError() << std::endl;
  }
  
  m_window = SDL_CreateWindow("RenderQuacky",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              m_viewport.width,
                              m_viewport.height,
                              SDL_WINDOW_RESIZABLE);

  if (!m_window) {
    std::cout << "Error creating SDL window " << std::endl;
    exit(666);
  }
}

void 
Application::update() {
  if (m_usesWindow) {
    SDL_Event SDLEvent;

    while (SDL_PollEvent(&SDLEvent)) {

      if (SDLEvent.type == SDL_QUIT) {
        m_running = false;
      }
      if (SDLEvent.type == SDL_WINDOWEVENT) {
        if (SDLEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
          std::cout << "Resize" << std::endl;
          m_viewport.width = SDLEvent.window.data1;
          m_viewport.height = SDLEvent.window.data2;
          onResize();
        }
      }
    }
  }
  
  postUpdate();
}

void 
Application::render() {
  postRender();
}

void 
Application::destroy() {
  if (m_usesWindow) {
    SDL_Quit();
  }
  
  postDestroy();
}

Application&
Application::getApplication() {
  return *Application::application;
}

}