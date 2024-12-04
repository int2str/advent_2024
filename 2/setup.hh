#ifndef TWO_SETUP_HH
#define TWO_SETUP_HH

#include <filesystem>
#include <vector>

namespace Day2::Setup {

[[nodiscard]] auto readLists(const std::filesystem::path& path)
    -> std::vector<std::vector<int>>;

}  // namespace Day2::Setup

#endif  // TWO_SETUP_HH
