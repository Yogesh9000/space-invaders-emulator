#include <fmt/core.h>

#include "cpu.hpp"
#include "memory.hpp"

// #define DEBUG_CPU

int main(int argc, char *argv[]) {
  Memory memory{MEMORY_SIZE};
  try {
    memory.load_data(argv[1]);
  } catch (std::runtime_error err) {
    fmt::println(stderr, "Error: {}", err.what());
    std::exit(EXIT_FAILURE);
  }
  Cpu cpu{memory};
  while (true) {
#ifdef DEBUG_CPU
    cpu.print_debug();
#endif

    cpu.cycle();
  }
  return 0;
}
