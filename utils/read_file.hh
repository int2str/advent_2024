#ifndef READ_FILE_HH
#define READ_FILE_HH

#include <filesystem>
#include <string>
#include <vector>

namespace Utils {

[[nodiscard]] auto readFile(const std::filesystem::path& path)
    -> std::vector<char>;

[[nodiscard]] auto readLines(const std::filesystem::path& path)
    -> std::vector<std::string>;

}  // namespace Utils

#endif  // READ_FILE_HH
