#ifndef STATE_HH
#define STATE_HH

#include <cstdint>
#include <vector>

#include "coordinate.hh"
#include "map.hh"

enum class Mode : uint8_t { Tracing, Probing, Done };

struct State {
  Map map;
  Mode mode = Mode::Tracing;

  Coordinate guard_at{};
  Coordinate guard_direction{};

  MapBits visited{};
  std::vector<Coordinate> candidates{};

  Coordinate candidate_at{};

  size_t candidates_attempted{};
  size_t obstruction_positions{};
  size_t travelled{};

  void makeCandidates() {
    candidates.clear();
    candidates.reserve(visited.size());

    for (int y = 0; y != map.size.y; ++y) {
      for (int x = 0; x != map.size.x; ++x) {
        const auto pos = Coordinate{x, y};
        if (visited.test(pos.idx()) and pos != map.guard)
          candidates.push_back(pos);
      }
    }
  }

  void resetGuard() {
    guard_at        = map.guard;
    guard_direction = Map::start_direction;
    visited         = {};
    travelled       = {};
  }

  void switchToProbing() {
    makeCandidates();
    mode                  = Mode::Probing;
    candidates_attempted  = {};
    obstruction_positions = {};
    nextCandidate();
  }

  void nextCandidate() {
    if (candidates_attempted++ != 0) map.blocked.reset(candidate_at.idx());
    candidate_at = candidates.back();
    candidates.pop_back();
    map.blocked.set(candidate_at.idx());
    resetGuard();
  };
};

#endif  // STATE_HH
