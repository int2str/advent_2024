#ifndef WINDOW_HH
#define WINDOW_HH

#include <SFML/Graphics.hpp>

#include "state.hh"

constexpr auto DEFAULT_FRAMERATE = 30U;

class Window {
  const State* state_;
  sf::RenderWindow window_;

  sf::CircleShape guard_;
  sf::CircleShape candidate_;

  sf::RectangleShape block_;
  sf::RectangleShape visited_;

  sf::Font font_{};
  sf::Text text_{};

  uint32_t frame_limit_{DEFAULT_FRAMERATE};

 public:
  explicit Window(const State* state);

  auto isOpen() const -> bool;

  void handleEvents();
  void draw();
};

#endif  // WINDOW_HH
