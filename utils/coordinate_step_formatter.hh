#ifndef COORDINATE_STEP_FORMATTER_HH
#define COORDINATE_STEP_FORMATTER_HH

#include <fmt/format.h>

#include "coordinate_formatter.hh"  // IWYU pragma: keep
#include "coordinate_step.hh"

template <>
struct fmt::formatter<Utils::Step> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Utils::Step& step, FormatContext& ctx) const {
    return fmt::format_to(ctx.out(), "{}->{}", step.position, step.direction);
  }
};

#endif  // COORDINATE_STEP_FORMATTER_HH
