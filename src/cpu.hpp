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
  void generate_interrupt(uint8_t int_num);
  uint8_t get_cycles();
  bool is_halted();

  // OPCODES
  void op_nop();

  // Data Transfer Group
  void op_mov();
  void op_mvi();
  void op_lxi();
  void op_lda();
  void op_sta();
  void op_lhld();
  void op_shld();
  void op_ldax();
  void op_stax_ma();
  void op_xchg();
  void op_stax();

  // Arithmetic Group
  void op_inr();
  void op_inx();
  void op_dcx();
  void op_dcr();
  void op_dad();
  void op_add();
  void op_adc();
  void op_ani();
  void op_adi();
  void op_aci();
  void op_sub();
  void op_sui();
  void op_sbi();
  void op_sbb();

  // Branch Group
  void op_jmp();
  void op_jmp_ccc();
  void op_call();
  void op_call_ccc();
  void op_ret();
  void op_pchl();

  // Logical Group
  void op_cpi();
  void op_rrc();
  void op_xra();
  void op_ana();
  void op_r_ccc();
  void op_ora();
  void op_ori();
  void op_xri();
  void op_cmp();
  void op_stc();
  void op_cmc();
  void op_cma();
  void op_rlc();
  void op_ral();
  void op_rar();

  // Stack, I/O, Machine Group
  void op_push();
  void op_pop();
  void op_in();
  void op_out();
  void op_ei();
  void op_xthl();
  void op_sphl();
  void op_hlt();

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
  uint8_t cycles_{};
  bool int_enable{false};
  bool halted{false};
  Memory& memory;
  std::unique_ptr<func[]> table{};
};

#endif  // !CPU_
