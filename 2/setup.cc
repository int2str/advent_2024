#include "setup.hh"

#include <charconv>
#include <fstream>

namespace {

template <typename T>
[[nodiscard]] constexpr auto toInt(std::string_view str) -> T {
  auto value = T{};
  std::from_chars<T>(str.data(), str.data() + str.size(), value);
  return value;
}

}  // namespace

namespace Day2::Setup {

[[nodiscard]] auto readLists(const std::filesystem::path& path)
    -> std::vector<std::vector<int>> {
  auto istream  = std::ifstream{path};
  auto readings = std::vector<std::vector<int>>{};

  auto line = std::string{};
  while (std::getline(istream, line)) {
    auto sstream   = std::stringstream{line};
    auto value_str = std::string{};
    auto values    = std::vector<int>{};
    while (std::getline(sstream, value_str, ' '))
      values.emplace_back(toInt<int>(value_str));
    readings.emplace_back(values);
  }

  return readings;
}

}  // namespace Day2::Setup
