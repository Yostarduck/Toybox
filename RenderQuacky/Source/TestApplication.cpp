#include "TestApplication.h"
#include <tb_graphics_api.h>

namespace toyboxSDK {

RenderQuackyApp::RenderQuackyApp() {
}

RenderQuackyApp::~RenderQuackyApp() {
}

void
RenderQuackyApp::postInit() {
  GraphicsAPI::startUp();
  GraphicsAPI::instance().init(1280, 720, m_hwnd);
}

void
RenderQuackyApp::postUpdate() {
}

void
RenderQuackyApp::postRender() {
}

void
RenderQuackyApp::postDestroy() {
}

void
RenderQuackyApp::onResize() {
}

}