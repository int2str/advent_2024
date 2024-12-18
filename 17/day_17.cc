//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/17
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <array>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include "external/ctre.hpp"
#include "testrunner/testrunner.h"
#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day17 {

struct CPU {
  enum OpCode : uint8_t { ADV = 0x00, BXL, BST, JNZ, BXC, OUT, BDV, CDV };

  size_t pc{};
  std::array<uint64_t, 3> registers{};
  std::vector<uint8_t> program{};
  std::vector<uint8_t> out{};

  constexpr auto RA() -> uint64_t& { return registers[0]; }
  constexpr auto RB() -> uint64_t& { return registers[1]; }
  constexpr auto RC() -> uint64_t& { return registers[2]; }

  [[nodiscard]] constexpr auto COMBO(uint8_t operand) -> uint64_t {
    if (operand < 0x4) return static_cast<uint64_t>(operand);
    return registers.at(operand - 4);
  }

  void OUT8(uint64_t value) {
    out.push_back(static_cast<uint8_t>((value & 0x7)));
  }

  void reset(uint64_t ra) {
    registers = {ra, 0, 0};
    pc        = {};
    out.clear();
  }

  void run() {
    while (pc < program.size()) {
      const auto op  = program.at(pc++);
      const auto arg = program.at(pc++);
      // clang-format off
      switch (op) {
        case OpCode::ADV: RA() = RA() / (1 << COMBO(arg));  break;
        case OpCode::BXL: RB() ^= arg;                      break;
        case OpCode::BST: RB() = COMBO(arg) & 0x7;          break;
        case OpCode::JNZ: pc = RA() != 0 ? arg : pc+1;      break;
        case OpCode::BXC: RB() ^= RC();                     break;
        case OpCode::OUT: OUT8(COMBO(arg));                 break;
        case OpCode::BDV: RB() = RA() / (1 << COMBO(arg));  break;
        case OpCode::CDV: RC() = RA() / (1 << COMBO(arg));  break;
        default: return;
      }
      // clang-format on
    }
  }

  [[nodiscard]] constexpr auto outputMatch() const -> bool {
    return !out.empty() &&
           std::equal(out.rbegin(), out.rend(), program.rbegin());
  }
};

[[nodiscard]] auto makeCPU(const std::filesystem::path& path) -> CPU {
  using namespace ctre::literals;           // NOLINT
  const auto file = Utils::readLines(path)  //
                    | std::views::join      //
                    | std::ranges::to<std::string>();
  auto [_, ra, program] = ctre::match<R"(Register A: (\d+).*)"
                                      R"(Program: ([\d,]+))">(file);
  return CPU{.registers = {static_cast<uint64_t>(ra.to_number()), 0, 0},
             .program   = Utils::split<uint8_t>(program, ",")};
}

[[nodiscard]] auto runProgram(CPU cpu) -> std::string {
  cpu.run();
  return fmt::format("{}", fmt::join(cpu.out, ","));
}

[[nodiscard]] auto findQuine(CPU cpu) -> uint64_t {
  auto try_ra = uint64_t{1};

  while (true) {
    cpu.reset(try_ra);
    cpu.run();

    if (cpu.outputMatch()) {
      if (cpu.out.size() == cpu.program.size()) return try_ra;
      try_ra *= 8;
    } else {
      ++try_ra;
    }

    if (cpu.out.size() > cpu.program.size()) return 0;
  }

  std::unreachable();
}

}  // namespace Day17

TEST(Day_17_Chronospatial_Computer_SAMPLE) {
  const auto cpu = Day17::makeCPU("17/sample.txt");
  EXPECT_EQ(Day17::runProgram(cpu), "4,6,3,5,6,3,5,2,1,0");
  EXPECT_EQ(Day17::findQuine(cpu), 0);
}

TEST(Day_17_Chronospatial_Computer_QUINE) {
  const auto cpu = Day17::makeCPU("17/quine.txt");
  EXPECT_EQ(Day17::runProgram(cpu), "0,3,5,4,3,0");
  EXPECT_EQ(Day17::findQuine(cpu), 117440);
}

TEST(Day_17_Chronospatial_Computer_FINAL) {
  const auto cpu = Day17::makeCPU("17/input.txt");
  EXPECT_EQ(Day17::runProgram(cpu), "1,5,3,0,2,5,2,5,3");
  EXPECT_EQ(Day17::findQuine(cpu), 108107566389757ULL);
}
