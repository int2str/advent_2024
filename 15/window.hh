#ifndef DAY_14_WINDOW_HH
#define DAY_14_WINDOW_HH

#include <SFML/Graphics.hpp>

#include "utils/coordinate.hh"
#include "utils/grid.hh"

namespace Day15 {

class Window {
  Utils::Coordinate max_;

  sf::Vector2f tile_;
  sf::RenderWindow window_;

  sf::Texture texture_blocks_{};
  sf::Sprite block_{};

  sf::Texture texture_robot_{};
  sf::Sprite robot_{};

 public:
  explicit Window(Utils::Coordinate max);

  void fps(unsigned int limit);

  auto isOpen() const -> bool;

  void handleEvents();
  void draw(const Utils::Grid<char>& map, Utils::Coordinate robot_direction);
};

}  // namespace Day15

#endif  // DAY_14_WINDOW_HH
