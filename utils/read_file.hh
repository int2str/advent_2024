#ifndef READ_FILE_HH
#define READ_FILE_HH

#include <filesystem>
#include <optional>
#include <vector>

namespace Utils {

[[nodiscard]] auto readFile(const std::filesystem::path& path)
    -> std::optional<std::vector<char>>;

}  // namespace Utils

#endif  // READ_FILE_HH
