#ifndef UTILS_GRID_FORMATTER_HH
#define UTILS_GRID_FORMATTER_HH

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <ranges>
#include <string>

#include "grid.hh"

template <typename GRID_STORE_AS, typename GRID_OOB_POLICY>
struct fmt::formatter<Utils::Grid<GRID_STORE_AS, GRID_OOB_POLICY>> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Utils::Grid<GRID_STORE_AS, GRID_OOB_POLICY>& grid,
              FormatContext& ctx) const {
    fmt::format_to(ctx.out(), "       {:^3}\n",
                   fmt::join(std::views::iota(1U, grid.width() + 1), " "));
    fmt::format_to(ctx.out(), "      {}\n", std::string(grid.width() * 4, '-'));
    for (size_t y = 0; y != grid.height(); ++y) {
      fmt::format_to(ctx.out(), " {:3} |", y + 1);
      for (size_t x = 0; x != grid.width(); ++x) {
        fmt::format_to(ctx.out(), " {:^3}", grid[x, y]);
      }
      fmt::format_to(ctx.out(), "\n");
    }

    return ctx.out();
  }
};

#endif  // UTILS_GRID_FORMATTER_HH
