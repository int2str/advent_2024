#ifndef ONE_SETUP_HH
#define ONE_SETUP_HH

#include <filesystem>
#include <utility>
#include <vector>

namespace Day1::Setup {

[[nodiscard]] auto readLists(const std::filesystem::path& path)
    -> std::pair<std::vector<uint32_t>, std::vector<uint32_t>>;

}  // namespace Day1::Setup

#endif  // ONE_SETUP_HH
