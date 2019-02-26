#include <iostream>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

float
clamp(float value, float min, float max) {
  if (value < min) {
    return min;
  }
  else if (value > max) {
    return max;
  }
  return value;
}

int
main() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Level editor");
  window.setVerticalSyncEnabled(true);
  ImGui::SFML::Init(window);

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigWindowsMoveFromTitleBarOnly = true;

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = true;
  style.WindowBorderSize = 1.0f;

  sf::Color bgColor;
  float color[3] = { 0.f, 0.f, 0.f };

  sf::RenderTexture* RT_SceneViewer = new sf::RenderTexture;
  RT_SceneViewer->create(1280, 720);

  sf::FloatRect textBounds;
  textBounds.left = 0.0f;
  textBounds.top = 0.0f;
  textBounds.width = RT_SceneViewer->getSize().x;
  textBounds.height = RT_SceneViewer->getSize().y;

  sf::CircleShape circle;
  circle.setRadius(50.0f);
  circle.setOrigin(50.0f, 50.0f);
  circle.setPosition(0.0f, 0.0f);
  circle.setFillColor(sf::Color::Magenta);

  sf::Vector2f cameraSz = { 128.0f, 72.0f };
  float cameraScale = 1.0f;
  sf::View debugCamera({ 0.f, 0.f }, cameraSz);
  debugCamera.setCenter(0.0f, 0.0f);

  sf::Vector2f lastWndSz = {0.0f, 0.0f};

  window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
  sf::Clock deltaClock;
  float deltaTime = 0.0f;
  float camSpeed = 250.0f;
  float camZoom = 10.0f;
  sf::Vector2f lastMousePos;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
      }
      if (event.type == sf::Event::MouseWheelMoved) {
        
      }
    }

    deltaTime = deltaClock.restart().asSeconds();
    ImGui::SFML::Update(window, deltaTime);

    RT_SceneViewer->setView(debugCamera);
    RT_SceneViewer->clear(bgColor);
    //Draw gameobjects
    RT_SceneViewer->draw(circle);
    RT_SceneViewer->display();

    ImGui::Begin("Example window");
    {
      if (ImGui::ColorEdit3("Background color", color)) {
        bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
        bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
        bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
      }
    }
    ImGui::End(); //Example window

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Scene viewer");
    {
      const float titleBarHeight = ImGui::GetTextLineHeight() +
                                   (ImGui::GetStyle().FramePadding.y * 2.0f);

      sf::Vector2f contentSz = ImGui::GetWindowContentRegionMax();
      contentSz.y -= titleBarHeight;

      //std::cout << ImGui::GetIO().MouseWheel << std::endl;
      if (ImGui::IsWindowHovered()) {
        if (ImGui::GetIO().MouseWheel != 0) {
          if (-ImGui::GetIO().MouseWheel > 0.5f) {
            cameraScale *= 1.1f;
          }
          else {
            cameraScale *= 0.9f;
          }
          debugCamera.setSize(cameraSz * cameraScale);
        }
      }

      sf::Vector2f mousePos = ImGui::GetMousePos();
      sf::Vector2f windowPos = ImGui::GetWindowPos();
      mousePos -= windowPos;
      mousePos.y -= titleBarHeight;
      //mousePos.x = clamp(mousePos.x, 0.0f, contentSz.x);
      //mousePos.y = clamp(mousePos.y, 0.0f, contentSz.y);

      //if mouse right pressed
      if (ImGui::IsMouseClicked(1, true)) {
        sf::Vector2f mouseDeltaPos = lastMousePos - mousePos;
        mouseDeltaPos.x /= contentSz.x;
        mouseDeltaPos.y /= contentSz.y;

        sf::Vector2f moveOffset = (debugCamera.getSize() * cameraScale);
        moveOffset.x *= mouseDeltaPos.x;
        moveOffset.y *= -mouseDeltaPos.y;
        debugCamera.move(moveOffset * 1.5f);
      }

      //if resized
      if (contentSz != lastWndSz) {
        lastWndSz = contentSz;

        //delete RT_SceneViewer;
        //RT_SceneViewer = new sf::RenderTexture;
        //RT_SceneViewer->create(contentSz.x, contentSz.y);
        cameraSz = contentSz;
        debugCamera.setSize(cameraSz * cameraScale);
      }

      ImGui::Image(RT_SceneViewer->getTexture(), contentSz);
      lastMousePos = mousePos;
    } 
    ImGui::End(); //Scene viewer
    ImGui::PopStyleVar();

    window.clear(sf::Color::Black);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}