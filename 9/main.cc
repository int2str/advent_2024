//
// int2str's Advent of CodeG 2024
// https://adventofcode.com/2024/day/9
//

#include <fmt/core.h>

#include <algorithm>
#include <ranges>

#include "utils/read_file.hh"

namespace Day9 {

struct Block {
  bool empty{};
  int size{};
  uint64_t id{};

  constexpr void clear() {
    empty = true;
    id    = {};
  }
};

struct BlockMaker {
  bool empty{true};
  uint64_t id{};

  [[nodiscard]] constexpr auto operator()(char chr) -> Block {
    empty = !empty;
    return {.empty = empty, .size = (chr - '0'), .id = (empty ? 0 : id++)};
  }
};

[[nodiscard]] auto fragment(std::vector<Block> blocks) -> std::vector<Block> {
  auto out = std::vector<Block>{};
  out.reserve(blocks.size());

  auto front = blocks.begin();
  auto back  = blocks.end() - 1;

  while (true) {
    while (!front->empty) out.push_back(*front++);
    while (back->empty) --back;
    if (front >= back) break;

    out.push_back(*back);
    if (front->size >= back->size) {
      front->size -= back->size;
      --back;

    } else {
      out.back().size = front->size;
      back->size -= front->size;
      ++front;
    }
  }

  return out;
}

[[nodiscard]] auto defrag(std::vector<Block> blocks) -> std::vector<Block> {
  auto front = size_t{};
  auto back  = blocks.size() - 1;

  while (back > front) {
    for (auto search = front; search < back; ++search) {
      if (!blocks.at(search).empty) continue;

      if (blocks.at(search).size == blocks.at(back).size) {
        std::swap(blocks.at(search), blocks.at(back));
        break;
      }

      if (blocks.at(search).size > blocks.at(back).size) {
        blocks.at(search).size -= blocks.at(back).size;
        blocks.insert(blocks.begin() + static_cast<int64_t>(search),
                      blocks.at(back));
        blocks.at(++back).clear();
        break;
      }
    }

    while (blocks.at(--back).empty);
    while (!blocks.at(front).empty) ++front;
  }
  return blocks;
}

[[nodiscard]] auto checksum(const std::vector<Block>& blocks) {
  return std::ranges::fold_left(
      blocks.begin(), blocks.end(), 0U,
      [pos = 0U](uint64_t accumulated, Block block) mutable {
        while (block.size-- != 0) accumulated += pos++ * block.id;
        return accumulated;
      });
}

}  // namespace Day9

auto main() -> int {
  const auto blocks =
      Utils::readFile("9/input.txt")                              //
      | std::views::filter([](auto chr) { return chr != '\n'; })  //
      | std::views::transform(Day9::BlockMaker{})                 //
      | std::ranges::to<std::vector>();

  fmt::print("Day 9\n-----\n");
  fmt::print("Part 1 | Filesystem checksum  : {}\n",
             Day9::checksum(Day9::fragment(blocks)));
  fmt::print("Part 2 | Defragmented checksum: {}\n\n",
             Day9::checksum(Day9::defrag(blocks)));
}
