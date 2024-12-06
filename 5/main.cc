//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/5
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day5 {

using RuleMap = std::unordered_map<int, std::unordered_set<int>>;
using Pages   = std::vector<int>;
using Manuals = std::vector<Pages>;

[[nodiscard]] auto makeRuleMap(const std::filesystem::path& path) -> RuleMap {
  const auto split_rule = [](const auto& line) constexpr {
    return Utils::split<int, 2>(line, "|");
  };
  const auto rules = Utils::readLines(path)  //
                     | std::views::transform(split_rule);
  auto rule_map = RuleMap{};
  for (const auto [before, after] : rules) rule_map[before].insert(after);
  return rule_map;
}

[[nodiscard]] auto makePages(const std::filesystem::path& path) -> Manuals {
  const auto split_pages = [](const auto& line) constexpr {
    return Utils::split<int>(line, ",");
  };
  return Utils::readLines(path)                //
         | std::views::transform(split_pages)  //
         | std::ranges::to<std::vector>();
}

[[nodiscard]] constexpr auto midpoint(const Pages& pages) -> int {
  return pages.at(pages.size() / 2);
};

[[nodiscard]] auto isValid(const RuleMap& rules, const Pages& pages) -> bool {
  auto seen = std::unordered_set<int>{};
  for (const auto& page : pages) {
    for (const auto& maybe_invalid : rules.at(page)) {
      if (seen.contains(maybe_invalid)) return false;
    }
    seen.insert(page);
  }
  return true;
}

[[nodiscard]] auto validMiddlePageSum(const RuleMap& rules,
                                      const Manuals& manuals) -> int {
  const auto is_valid = [&](const auto& pages) constexpr {
    return isValid(rules, pages);
  };

  auto midpoints = manuals                         //
                   | std::views::filter(is_valid)  //
                   | std::views::transform(midpoint);
  return std::ranges::fold_left(midpoints, 0, std::plus{});
}

[[nodiscard]] auto reorderInvlidPages(const RuleMap& rules,
                                      const Manuals& manuals) -> int {
  const auto is_invalid = [&](const auto& pages) constexpr {
    return !isValid(rules, pages);
  };

  const auto reorder = [&](auto pages) {
    for (auto before = pages.begin(); before != pages.end() - 1; ++before) {
      for (auto after = std::next(before); after != pages.end(); ++after) {
        if (rules.at(*after).contains(*before)) std::swap(*before, *after);
        // We COULD break early here, if the order is now valid.
        // But, at least on my machine, NOT checking and seeing
        // out the loop is actually MUCH faster...
      }
    }

    return pages;
  };

  auto reordered = manuals                           //
                   | std::views::filter(is_invalid)  //
                   | std::views::transform(reorder)  //
                   | std::views::transform(midpoint);
  return std::ranges::fold_left(reordered, 0, std::plus{});
}

}  // namespace Day5

auto main() -> int {
  const auto rules   = Day5::makeRuleMap("5/input_rules.txt");
  const auto manuals = Day5::makePages("5/input_pages.txt");

  fmt::print("Day 5\n-----\n");
  fmt::print("Part 1 | Middle page sum  : {}\n",
             Day5::validMiddlePageSum(rules, manuals));
  fmt::print("Part 2 | Correctly ordered: {}\n\n",
             Day5::reorderInvlidPages(rules, manuals));
}
