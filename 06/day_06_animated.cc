//
// int2str's Advent of CodeG 2024
// https://adventofcode.com/2024/day/6
//

#include "map.hh"
#include "state.hh"
#include "utils/coordinate.hh"
#include "utils/read_file.hh"
#include "window.hh"

namespace Day6 {

[[nodiscard]] constexpr auto guardInBounds(const State& state) -> bool {
  return state.guard_at.x >= 0 and state.guard_at.x < state.map.size.x and  //
         state.guard_at.y >= 0 and state.guard_at.y < state.map.size.y;
}

void moveGuard(State& state) {
  state.visited.insert(state.guard_at);
  auto new_position = state.guard_at + state.guard_direction;
  while (state.map.blocked.contains(new_position)) {
    state.guard_direction.rotateClockwise();
    new_position = state.guard_at + state.guard_direction;
  }
  state.guard_at = new_position;
}

[[nodiscard]] auto hasLooped(State& state) -> bool {
  if (!state.visited.contains(state.guard_at)) {
    state.travelled = 0;
  } else {
    ++state.travelled;
  }
  return (state.travelled == state.visited.size());
}

void animate(State& state) {
  Window window{&state};
  state.resetGuard();

  while (window.isOpen()) {
    window.handleEvents();

    switch (state.mode) {
      case Mode::Tracing: {
        if (guardInBounds(state)) {
          moveGuard(state);
        } else {
          state.switchToProbing();
        }
        break;
      }

      case Mode::Probing: {
        if (!state.noMoreCandidates()) {
          const auto last = state.candidates_attempted;
          while (!state.noMoreCandidates() and
                 state.candidates_attempted == last) {
            moveGuard(state);
            if (!guardInBounds(state)) {
              state.nextCandidate();

            } else if (hasLooped(state)) {
              ++state.obstruction_positions;
              state.nextCandidate();
            }
          }
        } else {
          state.mode = Mode::Done;
        }

        break;
      }

      case Mode::Done:
        break;
    }

    window.draw();
  }
}

}  // namespace Day6

auto main() -> int {
  auto state =
      Day6::State{.map = Utils::readFileXY("06/input.txt", Day6::Map{})};
  Day6::animate(state);
}
