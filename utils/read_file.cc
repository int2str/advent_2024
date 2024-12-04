#include "read_file.hh"

#include <filesystem>
#include <fstream>
#include <optional>
#include <vector>

namespace Utils {

auto readFile(const std::filesystem::path& path)
    -> std::optional<std::vector<char>> {
  auto file_stream = std::ifstream(std::string{path}, std::ios_base::binary);
  if (!file_stream.good()) return std::nullopt;

  file_stream.seekg(0, std::ios_base::end);
  const auto file_size = static_cast<size_t>(file_stream.tellg());

  auto contents = std::vector<char>(file_size);
  file_stream.seekg(0);
  file_stream.read(contents.data(), static_cast<std::streamsize>(file_size));
  return contents;
}

}  // namespace Utils
