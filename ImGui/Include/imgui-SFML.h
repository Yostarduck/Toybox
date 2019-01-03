#ifndef IMGUI_SFML_H
#define IMGUI_SFML_H

#include "imgui_prerequisites.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Joystick.hpp>

namespace sf
{
  class Event;
  class RenderTarget;
  class RenderWindow;
  class Sprite;
  class Texture;
  class Window;
}

namespace ImGui
{

namespace SFML
{
  TB_IMGUI_EXPORT void Init(sf::RenderWindow& window, bool loadDefaultFont = true);
  TB_IMGUI_EXPORT void Init(sf::Window& window, sf::RenderTarget& target, bool loadDefaultFont = true);

  TB_IMGUI_EXPORT void ProcessEvent(const sf::Event& event);

  TB_IMGUI_EXPORT void Update(sf::RenderWindow& window, float dt);
  TB_IMGUI_EXPORT void Update(sf::Window& window, sf::RenderTarget& target, float dt);
  TB_IMGUI_EXPORT void Update(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize, float dt);

  TB_IMGUI_EXPORT void Render(sf::RenderTarget& target);

  TB_IMGUI_EXPORT void Shutdown();

  TB_IMGUI_EXPORT void UpdateFontTexture();
  TB_IMGUI_EXPORT sf::Texture& GetFontTexture();

  // joystick functions
  TB_IMGUI_EXPORT void SetActiveJoystickId(unsigned int joystickId);
  TB_IMGUI_EXPORT void SetJoytickDPadThreshold(float threshold);
  TB_IMGUI_EXPORT void SetJoytickLStickThreshold(float threshold);

  TB_IMGUI_EXPORT void SetJoystickMapping(int action, unsigned int joystickButton);
  TB_IMGUI_EXPORT void SetDPadXAxis(sf::Joystick::Axis dPadXAxis, bool inverted = false);
  TB_IMGUI_EXPORT void SetDPadYAxis(sf::Joystick::Axis dPadYAxis, bool inverted = false);
  TB_IMGUI_EXPORT void SetLStickXAxis(sf::Joystick::Axis lStickXAxis, bool inverted = false);
  TB_IMGUI_EXPORT void SetLStickYAxis(sf::Joystick::Axis lStickYAxis, bool inverted = false);
} //SFML

// custom ImGui widgets for SFML stuff

// Image overloads
  TB_IMGUI_EXPORT void Image(const sf::Texture& texture,
                             const sf::Color& tintColor = sf::Color::White,
                             const sf::Color& borderColor = sf::Color::Transparent);
  TB_IMGUI_EXPORT void Image(const sf::Texture& texture, const sf::Vector2f& size,
                             const sf::Color& tintColor = sf::Color::White,
                             const sf::Color& borderColor = sf::Color::Transparent);
  TB_IMGUI_EXPORT void Image(const sf::Texture& texture, const sf::FloatRect& textureRect,
                             const sf::Color& tintColor = sf::Color::White,
                             const sf::Color& borderColor = sf::Color::Transparent);
  TB_IMGUI_EXPORT void Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::FloatRect& textureRect,
                             const sf::Color& tintColor = sf::Color::White,
                             const sf::Color& borderColor = sf::Color::Transparent);

  TB_IMGUI_EXPORT void Image(const sf::Sprite& sprite,
                             const sf::Color& tintColor = sf::Color::White,
                             const sf::Color& borderColor = sf::Color::Transparent);
  TB_IMGUI_EXPORT void Image(const sf::Sprite& sprite, const sf::Vector2f& size,
                             const sf::Color& tintColor = sf::Color::White,
                             const sf::Color& borderColor = sf::Color::Transparent);

// ImageButton overloads
  TB_IMGUI_EXPORT bool ImageButton(const sf::Texture& texture, const int framePadding = -1,
                                   const sf::Color& bgColor = sf::Color::Transparent,
                                   const sf::Color& tintColor = sf::Color::White);
  TB_IMGUI_EXPORT bool ImageButton(const sf::Texture& texture, const sf::Vector2f& size, const int framePadding = -1,
                                   const sf::Color& bgColor = sf::Color::Transparent, const sf::Color& tintColor = sf::Color::White);

  TB_IMGUI_EXPORT bool ImageButton(const sf::Sprite& sprite, const int framePadding = -1,
                                   const sf::Color& bgColor = sf::Color::Transparent,
                                   const sf::Color& tintColor = sf::Color::White);
  TB_IMGUI_EXPORT bool ImageButton(const sf::Sprite& sprite, const sf::Vector2f& size, const int framePadding = -1,
                                   const sf::Color& bgColor = sf::Color::Transparent,
                                   const sf::Color& tintColor = sf::Color::White);

// Draw_list overloads. All positions are in relative coordinates (relative to top-left of the current window)
  TB_IMGUI_EXPORT void DrawLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& col, float thickness = 1.0f);
  TB_IMGUI_EXPORT void DrawRect(const sf::FloatRect& rect, const sf::Color& color, float rounding = 0.0f, int rounding_corners = 0x0F, float thickness = 1.0f);
  TB_IMGUI_EXPORT void DrawRectFilled(const sf::FloatRect& rect, const sf::Color& color, float rounding = 0.0f, int rounding_corners = 0x0F);
} //ImGui

#endif //# IMGUI_SFML_H
