#ifndef CPU_HPP_INCLUDED
#define CPU_HPP_INCLUDED

#include "memory.hpp"

constexpr int MEMORY_SIZE = 0xffff;
constexpr int NUM_OPCODE = 0xff;

struct ConditionalCodes {
  uint8_t z : 1;
  uint8_t s : 1;
  uint8_t p : 1;
  uint8_t cy : 1;
  uint8_t acy : 1;
  uint8_t pad : 3;
};

class Cpu {
public:
  using func = void (Cpu::*)();

  Cpu(Memory& memory);
  ~Cpu() = default;

  void print_debug();
  void unimplemented_instruction();
  bool parity_iseven(uint16_t n);

  void cycle();

  // OPCODES
  void op_nop();

  // Data Transfer Group
  void op_mov();
  void op_mvi();
  void op_lxi();
  void op_lda();
  void op_sta();
  void op_lhld_lhd16();
  void op_shld_d16hl();
  void op_ldax();
  void op_stax_ma();
  void op_xchg();

  // Arithmetic Group
  void op_inx();
  void op_dcr();
  void op_dad();
  void op_ani();
  void op_adi();

  // Branch Group
  void op_jmp();
  void op_jmp_ccc();
  void op_call();
  void op_ret();

  // Logical Group
  void op_cpi();
  void op_rrc();
  void op_xra();
  void op_ana();

  // Stack, I/O, Machine Group
  void op_push();
  void op_pop();
  void op_out();
  void op_ei();

private:
  uint8_t a{};
  uint8_t b{};
  uint8_t c{};
  uint8_t d{};
  uint8_t e{};
  uint8_t h{};
  uint8_t l{};
  ConditionalCodes cc{};
  uint16_t pc{};
  uint16_t sp{};
  Memory& memory;
  std::unique_ptr<func[]> table{};
};

#endif  // !CPU_
