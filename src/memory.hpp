#ifndef MEMORY_HPP_INCLUDED
#define MEMORY_HPP_INCLUDED

#include <memory>
#include <string_view>

class Memory {
public:
  Memory(int size);
  ~Memory() = default;

  uint8_t& operator[](int idx);
  void load_data(std::string_view file_path, int offset = 0);
  uint8_t read_d8(uint16_t addr);
  uint16_t read_d16_32(uint16_t addr);
  uint8_t* get() {
    return memory.get();
  }

private:
  std::unique_ptr<uint8_t[]> memory{};
};

#endif  // !MEMORY_HPP_INCLUDED
