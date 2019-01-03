#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int
main() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Level editor");
  window.setVerticalSyncEnabled(true);
  ImGui::SFML::Init(window);
  
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigWindowsMoveFromTitleBarOnly = true;

  sf::Color bgColor;

  float color[3] = { 0.f, 0.f, 0.f };

  // let's use char array as buffer, see next part
  // for instructions on using std::string with ImGui
  char windowTitle[255] = "New name here";

  window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
  sf::Clock deltaClock;
  float deltaTime = 0.0f;
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
    }

    deltaTime = deltaClock.restart().asSeconds();
    ImGui::SFML::Update(window, deltaTime);

    ImGui::Begin("Sample window"); // begin window

                                   // Background color edit
    if (ImGui::ColorEdit3("Background color", color)) {
      // this code gets called if color value changes, so
      // the background color is upgraded automatically!
      bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
      bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
      bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
    }

    // Window title text edit
    ImGui::InputText("Window title", windowTitle, 255);

    if (ImGui::Button("Update window title")) {
      // this code gets if user clicks on the button
      // yes, you could have written if(ImGui::InputText(...))
      // but I do this to show how buttons work :)
      window.setTitle(windowTitle);
    }
    ImGui::End(); // end window

    window.clear(bgColor); // fill background with color
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}