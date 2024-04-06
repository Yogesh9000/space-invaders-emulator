#include "memory.hpp"

#include <fmt/core.h>

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string_view>

Memory::Memory(int size) : memory(std::make_unique<uint8_t[]>(size)) {}

uint8_t& Memory::operator[](int idx) {
  return memory[idx];
}

void Memory::load_data(std::string_view file_path, int offset) {
  std::ifstream file(file_path.data(), std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error(fmt::format("failed to load file {}", file_path));
  }
  std::streampos size = file.tellg();
  file.seekg(0, std::ios::beg);
  file.read(reinterpret_cast<char*>(memory.get()) + offset, size);
  file.close();
}

uint8_t Memory::read_d8(uint16_t addr) {
  return memory[addr];
}

uint16_t Memory::read_d16_32(uint16_t addr) {
  return (memory[addr + 1] << 8) | memory[addr];
}
