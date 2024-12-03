#ifndef ONE_SETUP_HH
#define ONE_SETUP_HH

#include <filesystem>
#include <utility>
#include <vector>

namespace One::Setup {

[[nodiscard]] auto readLists(const std::filesystem::path& path)
    -> std::pair<std::vector<uint32_t>, std::vector<uint32_t>>;

}  // namespace One::Setup

#endif  // ONE_SETUP_HH
