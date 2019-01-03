#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <array>

#include <SFML/Graphics.hpp>

#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"

float
RandomFloat(float a, float b) {
  const float random = ((float)rand()) / (float)RAND_MAX;
  const float diff = b - a;
  const float r = random * diff;
  return a + r;
}

int
main(int argc, char* argv[]) {
  if (argc == 1) {
  }
  else if (argc > 1) {
  }

  srand(time(NULL));

  // SFML window
  sf::RenderWindow window(sf::VideoMode(800, 600), "Ducky Delaunay triangulation");

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
      }
    }

    window.clear();



    window.display();
  }

  return 0;
}
