#include <fmt/core.h>

#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

#include "utils/curry.hh"
#include "utils/read_file.hh"

namespace Day4 {

struct Coordinate {
  int x;
  int y;

  [[nodiscard]] constexpr auto operator<=>(const Coordinate&) const = default;

  [[nodiscard]] constexpr auto operator+(const Coordinate& rhs) const
      -> Coordinate {
    return {.x = x + rhs.x, .y = y + rhs.y};
  }
};

struct CoordinateHash {
  [[nodiscard]] constexpr auto operator()(const Coordinate& coord) const
      -> int {
    return static_cast<int>(std::hash<int>()(coord.x) ^
                            std::hash<int>()(coord.y));
  }
};

using LookupTable =
    std::unordered_map<char, std::unordered_set<Coordinate, CoordinateHash>>;

[[nodiscard]] auto makeLookupTable(std::string_view chars) -> LookupTable {
  auto lookup = LookupTable{};

  auto x = int{};
  auto y = int{};
  for (const auto chr : chars) {
    if (chr != '\n') {
      lookup[chr].insert(Coordinate{x++, y});

    } else {
      ++y;
      x = 0;
    }
  }
  return lookup;
}

[[nodiscard]] constexpr auto search(const LookupTable& lookup,
                                    std::string_view what, Coordinate from,
                                    Coordinate direction) -> bool {
  if (what.empty()) return true;
  return lookup.at(what.front()).contains(from + direction) and
         search(lookup, what.substr(1), from + direction, direction);
}

[[nodiscard]] constexpr auto count(const LookupTable& lookup,
                                   std::string_view what) -> int64_t {
  const auto search_in_direction = [&](Coordinate from, int x_direction,
                                       int y_direction) {
    return (x_direction != 0 || y_direction != 0) and
           search(lookup, what.substr(1), from, {x_direction, y_direction});
  };

  const auto complete =
      std::views::cartesian_product(lookup.at(what.front()),
                                    std::views::iota(-1, 2),
                                    std::views::iota(-1, 2)) |
      std::views::transform(Utils::uncurry(search_in_direction));
  return std::ranges::count(complete, true);
}

[[nodiscard]] auto xmas(const LookupTable& lookup) -> int64_t {
  const auto check_diagonal = [&](Coordinate c1, Coordinate c2) {
    return (lookup.at('M').contains(c1) and lookup.at('S').contains(c2)) or
           (lookup.at('S').contains(c1) and lookup.at('M').contains(c2));
  };

  const auto& As = lookup.at('A');
  return std::ranges::count_if(std::begin(As), std::end(As), [&](Coordinate A) {
    return check_diagonal(A + Coordinate{-1, -1}, A + Coordinate{1, 1}) and
           check_diagonal(A + Coordinate{1, -1}, A + Coordinate{-1, 1});
  });
}

}  // namespace Day4

auto main() -> int {
  const auto file = Utils::readFile("4/input.txt");
  if (!file) return 1;

  [[maybe_unused]] const auto lookup =
      Day4::makeLookupTable(std::string_view{*file});

  fmt::print("Day 4\n-----\n");
  fmt::print("Part 1 | XMAS count : {}\n", Day4::count(lookup, "XMAS"));
  fmt::print("Part 2 | X-MAS count: {}\n\n", Day4::xmas(lookup));
}
