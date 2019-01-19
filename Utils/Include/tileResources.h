#pragma once

//
#define TILESIZE 10
//

#include <SFML/Graphics.hpp>

namespace toyboxSDK {
 
/**
 * @brief Returns the coordinates in pixel format
 * @param grid coordinates
 * @return pixel coordinates
 *
 */
static sf::Vector2f
GridtoPixel(sf::Vector2f vec) {
  return sf::Vector2f(vec.x * TILESIZE, vec.y * TILESIZE);
}

/**
 * @brief Returns the coordinates in tile format
 * @param pixel coordinates
 * @return grid coordinates
 *
 */
static sf::Vector2f
PixeltoGrid(sf::Vector2f vec) {
  sf::Vector2f temp;
  temp.x = (((int)vec.x - (int)vec.x % TILESIZE) / TILESIZE);
  temp.y = (((int)vec.y - (int)vec.y % TILESIZE) / TILESIZE);
  return temp;
}

}