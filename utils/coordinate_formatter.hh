#ifndef COORDINATE_FORMATTER_HH
#define COORDINATE_FORMATTER_HH

#include <fmt/format.h>

#include "coordinate.hh"

template <>
struct fmt::formatter<Utils::Coordinate> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Utils::Coordinate& coordinate, FormatContext& ctx) const {
    return fmt::format_to(ctx.out(), "{}/{}", coordinate.y + 1,
                          coordinate.x + 1);
  }
};

#endif  // COORDINATE_FORMATTER_HH
