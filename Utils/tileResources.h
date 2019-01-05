#pragma once

//
#define TILESIZE 10
//

#include <SFML/Graphics.hpp>

namespace toyboxSDK {
 
static sf::Vector2f
GridtoPixel(sf::Vector2f vec) {
  return vec * TILESIZE;
}

static sf::Vector2f
PixeltoGrid(sf::Vector2f vec) {
  sf::Vector2f temp;
  temp.x = (((int)vec.x - (int)vec.x % TILESIZE) / TILESIZE);
  temp.y = (((int)vec.y - (int)vec.y % TILESIZE) / TILESIZE);
  return temp;
}


}