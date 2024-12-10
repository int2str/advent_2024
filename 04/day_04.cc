//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/4
//

#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/curry.hh"
#include "utils/read_file.hh"

namespace Day4 {

using LookupTable = std::unordered_map<char, Utils::CoordinateSet>;

[[nodiscard]] auto makeLookupTable(const std::filesystem::path& path)
    -> LookupTable {
  auto lookup = LookupTable{};
  Utils::readFileXY(path, [&](auto x, auto y, auto chr) {
    lookup[chr].insert(
        Utils::Coordinate{.x = static_cast<int>(x), .y = static_cast<int>(y)});
  });
  return lookup;
}

[[nodiscard]] constexpr auto search(const LookupTable& lookup,
                                    std::string_view what,
                                    Utils::Coordinate from,
                                    Utils::Coordinate direction) -> bool {
  if (what.empty()) return true;
  return lookup.at(what.front()).contains(from + direction) and
         search(lookup, what.substr(1), from + direction, direction);
}

[[nodiscard]] constexpr auto xmas(const LookupTable& lookup,
                                  std::string_view what) -> int64_t {
  const auto search_in_direction = [&](Utils::Coordinate from, int x_direction,
                                       int y_direction) {
    return (x_direction != 0 || y_direction != 0) and
           search(lookup, what.substr(1), from, {x_direction, y_direction});
  };

  const auto complete =
      std::views::cartesian_product(lookup.at(*what.begin()),
                                    std::views::iota(-1, 2),
                                    std::views::iota(-1, 2)) |
      std::views::transform(Utils::uncurry(search_in_direction));
  return std::ranges::count(complete, true);
}

[[nodiscard]] auto x_mas(const LookupTable& lookup) -> int64_t {
  const auto check_diagonal = [&](Utils::Coordinate c1, Utils::Coordinate c2) {
    return (lookup.at('M').contains(c1) and lookup.at('S').contains(c2)) or
           (lookup.at('S').contains(c1) and lookup.at('M').contains(c2));
  };

  const auto& As = lookup.at('A');
  return std::ranges::count_if(
      std::begin(As), std::end(As), [&](Utils::Coordinate A) {
        return check_diagonal(A + Utils::Coordinate{-1, -1},
                              A + Utils::Coordinate{1, 1}) and
               check_diagonal(A + Utils::Coordinate{1, -1},
                              A + Utils::Coordinate{-1, 1});
      });
}

}  // namespace Day4

TEST(Day_04_Ceres_Search_SAMPLE) {
  const auto lookup = Day4::makeLookupTable("04/sample.txt");
  EXPECT_EQ(Day4::xmas(lookup, "XMAS"), 18);
  EXPECT_EQ(Day4::x_mas(lookup), 9);
}

TEST(Day_04_Ceres_Search_FINAL) {
  const auto lookup = Day4::makeLookupTable("04/input.txt");
  EXPECT_EQ(Day4::xmas(lookup, "XMAS"), 2464);
  EXPECT_EQ(Day4::x_mas(lookup), 1982);
}
