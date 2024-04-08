#define DEBUG_CPU

#include <fmt/core.h>

#include "cpu.hpp"
#include "memory.hpp"

int main() {
  Memory memory{MEMORY_SIZE};
  memory.load_data("invaders/cpudiag.bin", 0x100);
  memory[0] = 0xc3;
  memory[1] = 0;
  memory[2] = 0x01;
  memory[368] = 0x7;
  memory[0x59c] = 0xc3;
  memory[0x59d] = 0xc2;
  memory[0x59e] = 0x05;
  Cpu cpu{memory};
  while (true) {
#if defined(DEBUG_CPU)
    cpu.print_debug();
#endif

    cpu.cycle();
  }
  return 0;
}
