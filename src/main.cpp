#include <SDL2/SDL.h>
#include <fmt/core.h>

#include "cpu.hpp"
#include "display.hpp"
#include "memory.hpp"

// #define DEBUG_CPU

constexpr int SCREEN_HEIGHT = 256;
constexpr int SCREEN_WIDTH = 224;
constexpr int VRAM_START = 0x2400;

void cpu_cycle(Cpu& cpu, long cyc) {
  long cyc_ran{0};
  while (cyc_ran < cyc) {
#ifdef DEBUG_CPU
    cpu.print_debug();
#endif
    cpu.cycle();
    cyc_ran += cpu.get_cycles();
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fmt::println(stderr, "USAGE:{} <ROM>", argv[0]);
    std::exit(EXIT_FAILURE);
  }
  Memory memory{MEMORY_SIZE};
  try {
    memory.load_data(argv[1]);
  } catch (std::runtime_error err) {
    fmt::println(stderr, "Error: {}", err.what());
    std::exit(EXIT_FAILURE);
  }
  Cpu cpu{memory};
  Display display{"space invaders", SCREEN_WIDTH, SCREEN_HEIGHT, 2};
  SDL_Event e;
  uint32_t last_tic = SDL_GetTicks();
  constexpr double tic = 1000.0 / 60.0;  // ms per tic
  constexpr int cycles_per_ms = 2000;    // 2 Mhz
  constexpr double cycles_per_tic = cycles_per_ms * tic;
  bool quit{false};
  while (!quit && !cpu.is_halted()) {
    if ((SDL_GetTicks() - last_tic) >= tic) {
      last_tic = SDL_GetTicks();
      cpu_cycle(cpu, cycles_per_tic / 2);
      cpu.generate_interrupt(0xcf);
      cpu_cycle(cpu, cycles_per_tic / 2);
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) quit = true;
      }
      display.Update(memory.get() + VRAM_START);
      cpu.generate_interrupt(0xd7);
    }
  }
  return 0;
}
