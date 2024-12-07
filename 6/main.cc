//
// int2str's Advent of CodeG 2024
// https://adventofcode.com/2024/day/6
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "coordinate.hh"
#include "state.hh"
#include "window.hh"

namespace Day6 {

[[nodiscard]] constexpr auto guardInBounds(const State& state) -> bool {
  return state.guard_at.x >= 0 and state.guard_at.x < state.map.size.x and  //
         state.guard_at.y >= 0 and state.guard_at.y < state.map.size.y;
}

void moveGuard(State& state) {
  state.visited.set(state.guard_at.idx());
  auto new_position = state.guard_at + state.guard_direction;
  while (state.map.blocked.test(new_position.idx())) {
    state.guard_direction.rotateCW();
    new_position = state.guard_at + state.guard_direction;
  }
  state.guard_at = new_position;
}

[[nodiscard]] auto hasLooped(State& state) -> bool {
  if (!state.visited.test(state.guard_at.idx())) {
    state.travelled = 0;
  } else {
    ++state.travelled;
  }
  return (state.travelled == state.visited.size());
}

void animate(State& state) {
  Window window{&state};

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
        if (!state.candidates.empty()) {
          const auto last = state.candidates_attempted;
          while (!state.candidates.empty() and
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

void calculate(State& state) {
  // Part 1
  while (guardInBounds(state)) moveGuard(state);

  // Part 2
  state.switchToProbing();
  while (!state.candidates.empty()) {
    moveGuard(state);
    if (!guardInBounds(state)) {
      state.nextCandidate();

    } else if (hasLooped(state)) {
      ++state.obstruction_positions;
      state.nextCandidate();
    }
  }
}

}  // namespace Day6

auto main(int argc, char** argv) -> int {
  const auto args = std::span(argv, static_cast<size_t>(argc));

  auto state = State{.map = Map::fromFile("6/input.txt")};
  state.resetGuard();

  fmt::print("{}\n", state.map.size.x);

  fmt::print("Day 6\n-----\n");
  if (args.size() < 2) {
    Day6::calculate(state);
  } else {
    Day6::animate(state);
  }

  fmt::print("Part 1 | Guard visited     : {}\n",
             state.candidates_attempted + 1);
  fmt::print("Part 2 | Possible blockades: {}\n\n",
             state.obstruction_positions);
}
