//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/2
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <functional>
#include <ranges>

#include "utils/read_file.hh"
#include "utils/split.hh"

namespace {

[[nodiscard]] constexpr auto minmaxDifference(const std::vector<int>& record)
    -> std::ranges::minmax_result<int> {
  return std::ranges::minmax(record |
                             std::views::pairwise_transform(std::minus<int>{}));
}

[[nodiscard]] constexpr auto isSafe(std::ranges::minmax_result<int> minmax)
    -> bool {
  return minmax.min != 0 and minmax.max != 0  //
         and std::abs(minmax.min) <= 3        //
         and std::abs(minmax.max) <= 3        //
         and std::signbit(minmax.min) == std::signbit(minmax.max);
}

}  // namespace

namespace Day2 {

[[nodiscard]] auto safeReports(const auto& records) {
  const auto validated = records                                    //
                         | std::views::transform(minmaxDifference)  //
                         | std::views::transform(isSafe);
  return std::ranges::count(validated, true);
}

[[nodiscard]] auto safeReportsMinusOne(const auto& records) {
  constexpr auto validate = [](auto record) -> bool {
    for (int64_t skip_idx = 0; skip_idx != static_cast<int64_t>(record.size());
         ++skip_idx) {
      auto copy = record;
      copy.erase(copy.begin() + skip_idx);
      const auto valid = isSafe(minmaxDifference(copy));
      if (valid) return true;
    }
    return false;
  };

  const auto validated = records | std::views::transform(validate);
  return std::ranges::count(validated, true);
}

}  // namespace Day2

auto main() -> int {
  const auto records = Utils::readLines("2/input.txt")  //
                       | std::views::transform([](const auto& line) {
                           return Utils::split<int>(line, " ");
                         })  //
                       | std::ranges::to<std::vector>();

  fmt::print("Day 2\n-----\n");
  fmt::print("Part 1 | Safe reports  : {}\n", Day2::safeReports(records));
  fmt::print("Part 2 | Safe minus one: {}\n\n",
             Day2::safeReportsMinusOne(records));
}
