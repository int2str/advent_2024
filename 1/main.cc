//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/1
//

#include <fmt/core.h>

#include <algorithm>
#include <numeric>
#include <ranges>
#include <unordered_map>

#include "setup.hh"

namespace Day1 {

[[nodiscard]] auto partOne() -> uint32_t {
  auto [first, second] = Setup::readLists("1/inputs.txt");
  std::sort(first.begin(), first.end());
  std::sort(second.begin(), second.end());

  constexpr auto abs_distance = [](const auto first, const auto second) {
    return first > second ? first - second : second - first;
  };

  const auto range = std::views::zip_transform(abs_distance, first, second);
  return std::accumulate(std::begin(range), std::end(range), uint32_t{});
}

[[nodiscard]] auto partTwo() -> uint64_t {
  auto [first, second] = Setup::readLists("1/inputs.txt");

  auto map = std::unordered_map<uint32_t, uint64_t>{};
  for (const auto& right : second) ++map[right];

  auto weighted = [&map](auto lookup) {
    return lookup * (map.contains(lookup) ? map[lookup] : uint64_t{});
  };

  const auto range = first | std::views::transform(weighted);
  return std::accumulate(std::begin(range), std::end(range), uint64_t{});
}

}  // namespace Day1

auto main() -> int {
  fmt::print("Day 1\n-----\n");
  fmt::print("Part 1 | Distance sum        : {}\n", Day1::partOne());
  fmt::print("Part 2 | Weighted difference : {}\n", Day1::partTwo());
}
