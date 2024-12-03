#include "setup.hh"

#include <fstream>

namespace {

[[nodiscard]] auto split(std::istringstream iss)
    -> std::pair<uint32_t, uint32_t> {
  auto first  = uint32_t{};
  auto second = uint32_t{};
  iss >> first >> second;
  return {first, second};
}

}  // namespace

namespace One::Setup {

[[nodiscard]] auto readLists(const std::filesystem::path& path)
    -> std::pair<std::vector<uint32_t>, std::vector<uint32_t>> {
  auto istream = std::ifstream{path};
  auto list_a  = std::vector<uint32_t>{};
  auto list_b  = std::vector<uint32_t>{};

  auto line = std::string{};
  while (std::getline(istream, line)) {
    const auto [first, second] = split(std::istringstream{line});
    list_a.push_back(first);
    list_b.push_back(second);
  }

  return {list_a, list_b};
}

}  // namespace One::Setup
