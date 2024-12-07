#ifndef STATE_HH
#define STATE_HH

#include <cstdint>
#include <unordered_set>

#include "coordinate.hh"
#include "map.hh"

enum class Mode : uint8_t { Tracing, Probing, Done };

struct State {
  Map map;
  Mode mode = Mode::Tracing;

  Coordinate guard_at{};
  Coordinate guard_direction{};

  std::unordered_set<Coordinate, CoordinateHash> visited{};
  std::unordered_set<Coordinate, CoordinateHash> candidates{};

  Coordinate candidate_at{};

  size_t candidates_attempted{};
  size_t obstruction_positions{};
  size_t travelled{};

  void resetGuard() {
    guard_at        = map.guard;
    guard_direction = Map::start_direction;
    visited         = {};
    travelled       = {};
  }

  void switchToProbing() {
    mode       = Mode::Probing;
    candidates = visited;
    candidates.erase(map.guard);
    candidates_attempted  = {};
    obstruction_positions = {};
    nextCandidate();
  }

  void nextCandidate() {
    if (candidates_attempted++ != 0) map.blocks.erase(candidate_at);
    candidate_at = *candidates.begin();
    candidates.erase(candidates.begin());
    map.blocks.insert(candidate_at);
    resetGuard();
  };
};

#endif  // STATE_HH
