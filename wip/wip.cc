//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/16
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <SFML/Window/Keyboard.hpp>
#include <algorithm>  // IWYU pragma: keep
#include <filesystem>
#include <fstream>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_directions.hh"
#include "utils/coordinate_formatter.hh"
#include "utils/coordinate_map.hh"
#include "utils/coordinate_set.hh"
#include "utils/grid.hh"
#include "utils/grid_formatter.hh"

namespace Day16 {

static constexpr auto MAX_DISTANCE = std::numeric_limits<size_t>::max();

using Utils::Coordinate;

using Map      = Utils::Grid<char>;
using Scores   = Utils::CoordinateMap<size_t>;
using Previous = Utils::CoordinateMap<Utils::CoordinateSet>;

struct Step {
  Coordinate position;
  Coordinate direction;

  [[nodiscard]] constexpr auto operator<=>(const Step&) const = default;
};

struct StepHash {
  [[nodiscard]] constexpr auto operator()(const Step& step) const -> int {
    return static_cast<int>(Utils::CoordinateHash()(step.position) ^
                            Utils::CoordinateHash()(step.direction));
  }
};

template <typename T>
using StepMap = std::unordered_map<Step, T, StepHash>;

struct ScoredMap {
  Map map;
  Scores scores{};
  Previous previous{};
};

[[nodiscard]] auto loadMap(const std::filesystem::path& path) -> Map {
  auto map_file = std::ifstream(path);
  return Map::from(map_file);
}

[[nodiscard]] auto initializePathScores(const Map& map)
    -> Utils::CoordinateMap<size_t> {
  const auto not_wall = [&](auto coordinate) { return map[coordinate] != '#'; };
  const auto default_score = [&](auto coordinate) {
    return std::make_pair(coordinate,  //
                          map[coordinate] == 'E' ? 0U : MAX_DISTANCE);
  };
  return map.coordinates()                       //
         | std::views::filter(not_wall)          //
         | std::views::transform(default_score)  //
         | std::ranges::to<Utils::CoordinateMap<size_t>>();
}

[[nodiscard]] auto calculatePathScores(const Map& map) -> ScoredMap {
  auto result = ScoredMap{.map = map, .scores = initializePathScores(map)};

  const auto key_for = [](auto tuple) { return std::get<0>(tuple); };

  const auto keep_lowest = [](auto acc, auto pair) {
    if (pair.second < acc.second) return pair;
    return acc;
  };

  auto unvisited = result.scores                     //
                   | std::views::transform(key_for)  //
                   | std::ranges::to<Utils::CoordinateSet>();
  auto towards = Utils::CoordinateMap<Coordinate>{};

  const auto score_for = [&](auto coordinate) {
    return std::make_pair(coordinate, result.scores[coordinate]);
  };

  while (true) {
    const auto [lowest_node, lowest_score] = std::ranges::fold_left(
        unvisited | std::views::transform(score_for),
        std::make_pair(Coordinate{}, MAX_DISTANCE), keep_lowest);
    if (lowest_score == MAX_DISTANCE) break;
    unvisited.erase(lowest_node);

    for (const auto next : lowest_node.orthogonalNeighbors()) {
      if (unvisited.contains(next)) {
        const auto distance =
            lowest_score == 0
                ? 1U
                : ((next - lowest_node) != towards[lowest_node] ? 1'001U : 1U);
        const auto alt = result.scores[lowest_node] + distance;
        if (alt <= result.scores[next]) {
          towards[next] = next - lowest_node;
          result.previous[next].insert(lowest_node);
          result.scores[next] = alt;
        }
      }
    }
  }

  return result;
}

[[nodiscard]] auto loadScoredMap(const std::filesystem::path& path)
    -> ScoredMap {
  const auto map = loadMap(path);
  return calculatePathScores(map);
}

auto score(const ScoredMap& scored, Coordinate from, Coordinate direction,
           Coordinate end) -> size_t {
  if (from == end) return 1;
  if (!scored.previous.contains(from)) return 0;

  auto next = *scored.previous.at(from).begin();
  if (scored.previous.at(from).count() == 2) {
    if ((from - next) != direction)
      next = *std::next(scored.previous.at(from).begin());
  }

  // const auto next = *scored.previous.at(from).begin();
  const auto mine = (from - next) == direction ? 1U : 1'001U;
  // fmt::print("{:e} -> {:e} = {} | {}\n", from, next, from - next, mine);
  return mine + score(scored, next, (from - next), end);
}

void walk(const ScoredMap& scored, Coordinate from, std::string indent = "") {
  if (!scored.previous.contains(from)) return;
  for (const auto next : scored.previous.at(from)) {
    walk(scored, next, indent + " ");
  }
}

// TODO(AE): constexpr
[[nodiscard]] auto calculateShortestPath(const ScoredMap& scored) -> size_t {
  auto scored_map =
      Utils::Grid<size_t>{scored.map.width(), scored.map.height()};
  for (const auto& [pos, score] : scored.scores) scored_map[pos] = score;

  const auto start = scored.map.find('S').value_or(Coordinate{});
  const auto end   = scored.map.find('E').value_or(Coordinate{});

  auto sss = score(scored, start, Utils::Direction::right(), end);
  fmt::print("{}", scored_map, sss);

  fmt::print("{}", scored.map, sss);

  walk(scored, start);

  return scored.scores.at(scored.map.find('E').value_or(Coordinate{}));
}

}  // namespace Day16

auto main() -> int {
  const auto scored = Day16::loadScoredMap("16/sample_mini.txt");
  fmt::print("--> {}", Day16::calculateShortestPath(scored));
  // EXPECT_EQ(Day16::calculateShortestPath(scored), 83432);
}
