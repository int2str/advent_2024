//
// int2str's Advent of CodeG 2024
// https://adventofcode.com/2024/day/7
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>  // IWYU pragma: keep
#include <ranges>
#include <vector>

#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day7 {

using Problem  = std::vector<uint64_t>;
using Problems = std::vector<Problem>;

template <typename T>
struct Concat {
  [[nodiscard]] constexpr auto operator()(T a, T b) const -> T {
    auto result     = T{};
    const auto both = std::format("{}{}", a, b);
    std::from_chars(both.data(), both.data() + both.size(), result);
    return result;
  }
};

template <typename... TYPES>
auto tryOp(uint64_t solution, uint64_t accumulated, auto operands, auto&& op,
           const std::tuple<TYPES...> ops) -> bool {
  accumulated = op(accumulated, operands.front());
  if (operands.size() == 1) return accumulated == solution;
  return (tryOp(solution, accumulated, operands.next(), std::get<TYPES>(ops),
                ops) or
          ...);
}

template <typename... TYPES>
[[nodiscard]] auto tryOps(const Problem& problem,
                          const std::tuple<TYPES...>& ops) -> bool {
  if (problem.size() < 3) return false;

  const auto solution    = problem.front();
  const auto accumulated = *std::next(problem.begin());
  const auto operands =
      std::ranges::subrange(problem.begin() + 2, problem.end());

  return (tryOp(solution, accumulated, operands, std::get<TYPES>(ops), ops) or
          ...);
}

[[nodiscard]] auto fixPlusOrMultiplies(const Problem& problem) -> uint64_t {
  const auto ops = std::tuple(std::plus{}, std::multiplies{});
  return tryOps(problem, ops) ? problem.front() : 0U;
}

[[nodiscard]] auto alsoFixConcatenate(const Problem& problem) -> uint64_t {
  const auto ops =
      std::tuple(std::plus{}, std::multiplies{}, Concat<uint64_t>{});
  return tryOps(problem, ops) ? problem.front() : 0U;
}

[[nodiscard]] auto calibrationResult(const Problems& problems,
                                     const auto& fn) -> uint64_t {
  return std::ranges::fold_left(  //
      problems | std::views::transform(fn), uint64_t{}, std::plus{});
}

}  // namespace Day7

auto main() -> int {
  const auto problems = Utils::readLines("7/input.txt")  //
                        | std::views::transform([](const auto& line) {
                            return Utils::split<uint64_t>(line, " ");
                          })  //
                        | std::ranges::to<std::vector>();

  fmt::print("Day 7\n-----\n");
  fmt::print("Part 1 | Calibration result : {}\n",
             Day7::calibrationResult(problems, Day7::fixPlusOrMultiplies));
  fmt::print("Part 2 | Calibration with ||: {}\n\n",
             Day7::calibrationResult(problems, Day7::alsoFixConcatenate));
}
