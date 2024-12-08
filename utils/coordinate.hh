#ifndef COORDINATE_HH
#define COORDINATE_HH

#include <bitset>
#include <cstdint>
#include <cwchar>
#include <limits>

struct Coordinate {
  int x{};
  int y{};

  [[nodiscard]] constexpr auto operator<=>(const Coordinate&) const = default;

  constexpr auto operator+=(const Coordinate& other) -> Coordinate& {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr auto operator-=(const Coordinate& other) -> Coordinate& {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  void rotateCW() { *this = Coordinate{-y, x}; }
};

[[nodiscard]] constexpr auto operator+(const Coordinate& lhs,
                                       const Coordinate& rhs) -> Coordinate {
  return {.x = lhs.x + rhs.x, .y = lhs.y + rhs.y};
}

[[nodiscard]] constexpr auto operator-(const Coordinate& lhs,
                                       const Coordinate& rhs) -> Coordinate {
  return {.x = lhs.x - rhs.x, .y = lhs.y - rhs.y};
}

struct CoordinateHash {
  [[nodiscard]] constexpr auto operator()(const Coordinate& coord) const
      -> int {
    return static_cast<int>(std::hash<int>()(coord.x) ^
                            std::hash<int>()(coord.y));
  }
};

class CoordinateSet {
  using Bits = std::bitset<std::numeric_limits<uint16_t>::max()>;

  Bits bits_{};

  [[nodiscard]] static constexpr auto idxFor(const Coordinate& coordinate)
      -> uint16_t {
    return static_cast<uint16_t>(
        (static_cast<uint16_t>(coordinate.y) << 8) +    // NOLINT
        (static_cast<uint16_t>(coordinate.x) & 0xFF));  // NOLINT
  }

 public:
  class Iterator {
    const Bits* bits_p_{nullptr};
    size_t idx_{};

    friend CoordinateSet;
    constexpr explicit Iterator(const Bits* bits, size_t idx)
        : bits_p_{bits}, idx_{idx} {
      if (idx_ < bits_p_->size() and !bits_p_->test(idx)) advance();
    }

    void advance() {
      do {
        ++idx_;
      } while (idx_ < bits_p_->size() and !bits_p_->test(idx_));
    }

   public:
    using difference_type = int32_t;
    using value_type      = Coordinate;

    Iterator() = default;

    [[nodiscard]] constexpr auto operator*() const -> Coordinate {
      return {.x = static_cast<int>(idx_ & 0xFF),  // NOLINT
              .y = static_cast<int>(idx_ >> 8)};   // NOLINT
    }

    constexpr auto operator++() -> Iterator& {
      advance();
      return *this;
    }

    constexpr auto operator++(int) -> Iterator {
      const auto pre = *this;
      advance();
      return pre;
    }

    [[nodiscard]] constexpr auto operator<=>(const Iterator&) const = default;
  };

  using const_iterator = Iterator;
  using iterator       = Iterator;
  using value_type     = Coordinate;

  constexpr void insert(const Coordinate& coordinate) {
    bits_.set(idxFor(coordinate));
  }

  constexpr void erase(const Coordinate& coordinate) {
    bits_.reset(idxFor(coordinate));
  }

  constexpr void clear() { bits_ = 0; }

  [[nodiscard]] constexpr auto contains(const Coordinate& coordinate) const
      -> bool {
    const auto idx = idxFor(coordinate);
    return idx < bits_.size() and bits_.test(idx);
  }

  [[nodiscard]] constexpr auto size() const -> size_t { return bits_.size(); }

  [[nodiscard]] constexpr auto count() const -> size_t { return bits_.count(); }

  [[nodiscard]] constexpr auto empty() const -> bool { return bits_.none(); }

  [[nodiscard]] constexpr auto begin() const -> Iterator {
    return Iterator{&bits_, 0};
  }

  [[nodiscard]] constexpr auto end() const -> Iterator {
    return Iterator{&bits_, bits_.size()};
  }
};

// --------------------------------------------------------------------------------

#include <fmt/format.h>

template <>
struct fmt::formatter<Coordinate> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Coordinate& coordinate, FormatContext& ctx) const {
    return fmt::format_to(ctx.out(), "{}/{}", coordinate.y + 1,
                          coordinate.x + 1);
  }
};

#endif  // COORDINATE_HH
