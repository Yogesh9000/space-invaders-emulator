// #define DEBUG_CPU

#include "cpu.hpp"

#include <fmt/color.h>
#include <fmt/core.h>

#define red(arg) fmt::styled(arg, fmt::fg(fmt::color::red))
#define green(arg) fmt::styled(arg, fmt::fg(fmt::color::green))

Cpu::Cpu(Memory& memory)
    : a{}, b{}, c{}, d{}, e{}, h{}, l{}, cc{}, pc{}, sp{}, cycles_{0},
      memory{memory}, table{std::make_unique<func[]>(NUM_OPCODE + 1)} {
  for (int i{0}; i <= 0xff; ++i) {
    table[i] = &Cpu::unimplemented_instruction;
  }
  table[0x00] = &Cpu::op_nop;
  table[0x01] = &Cpu::op_lxi;
  table[0x02] = &Cpu::op_stax;
  table[0x03] = &Cpu::op_inx;
  table[0x04] = &Cpu::op_inr;
  table[0x05] = &Cpu::op_dcr;
  table[0x06] = &Cpu::op_mvi;
  table[0x07] = &Cpu::op_rlc;
  table[0x08] = &Cpu::op_nop;
  table[0x09] = &Cpu::op_dad;
  table[0x0a] = &Cpu::op_ldax;
  table[0x0b] = &Cpu::op_dcx;
  table[0x0c] = &Cpu::op_inr;
  table[0x0d] = &Cpu::op_dcr;
  table[0x0e] = &Cpu::op_mvi;
  table[0x0f] = &Cpu::op_rrc;
  table[0x10] = &Cpu::op_nop;
  table[0x11] = &Cpu::op_lxi;
  table[0x12] = &Cpu::op_stax;
  table[0x13] = &Cpu::op_inx;
  table[0x14] = &Cpu::op_inr;
  table[0x15] = &Cpu::op_dcr;
  table[0x16] = &Cpu::op_mvi;
  table[0x17] = &Cpu::op_ral;
  table[0x18] = &Cpu::op_nop;
  table[0x19] = &Cpu::op_dad;
  table[0x1a] = &Cpu::op_ldax;
  table[0x1b] = &Cpu::op_dcx;
  table[0x1c] = &Cpu::op_inr;
  table[0x1d] = &Cpu::op_dcr;
  table[0x1e] = &Cpu::op_mvi;
  table[0x1f] = &Cpu::op_rar;
  table[0x20] = &Cpu::op_nop;
  table[0x21] = &Cpu::op_lxi;
  table[0x22] = &Cpu::op_shld;
  table[0x23] = &Cpu::op_inx;
  table[0x24] = &Cpu::op_inr;
  table[0x25] = &Cpu::op_dcr;
  table[0x26] = &Cpu::op_mvi;
  table[0x28] = &Cpu::op_nop;
  table[0x29] = &Cpu::op_dad;
  table[0x2a] = &Cpu::op_lhld;
  table[0x2b] = &Cpu::op_dcx;
  table[0x2c] = &Cpu::op_inr;
  table[0x2d] = &Cpu::op_dcr;
  table[0x2e] = &Cpu::op_mvi;
  table[0x2f] = &Cpu::op_cma;
  table[0x30] = &Cpu::op_nop;
  table[0x31] = &Cpu::op_lxi;
  table[0x32] = &Cpu::op_sta;
  table[0x33] = &Cpu::op_inx;
  table[0x34] = &Cpu::op_inr;
  table[0x35] = &Cpu::op_dcr;
  table[0x36] = &Cpu::op_mvi;
  table[0x37] = &Cpu::op_stc;
  table[0x38] = &Cpu::op_nop;
  table[0x39] = &Cpu::op_dad;
  table[0x3a] = &Cpu::op_lda;
  table[0x3b] = &Cpu::op_dcx;
  table[0x3c] = &Cpu::op_inr;
  table[0x3d] = &Cpu::op_dcr;
  table[0x3e] = &Cpu::op_mvi;
  table[0x3f] = &Cpu::op_cmc;
  table[0x40] = &Cpu::op_mov;
  table[0x41] = &Cpu::op_mov;
  table[0x42] = &Cpu::op_mov;
  table[0x43] = &Cpu::op_mov;
  table[0x44] = &Cpu::op_mov;
  table[0x45] = &Cpu::op_mov;
  table[0x46] = &Cpu::op_mov;
  table[0x47] = &Cpu::op_mov;
  table[0x48] = &Cpu::op_mov;
  table[0x49] = &Cpu::op_mov;
  table[0x4a] = &Cpu::op_mov;
  table[0x4b] = &Cpu::op_mov;
  table[0x4c] = &Cpu::op_mov;
  table[0x4d] = &Cpu::op_mov;
  table[0x4e] = &Cpu::op_mov;
  table[0x4f] = &Cpu::op_mov;
  table[0x50] = &Cpu::op_mov;
  table[0x51] = &Cpu::op_mov;
  table[0x52] = &Cpu::op_mov;
  table[0x53] = &Cpu::op_mov;
  table[0x54] = &Cpu::op_mov;
  table[0x55] = &Cpu::op_mov;
  table[0x56] = &Cpu::op_mov;
  table[0x57] = &Cpu::op_mov;
  table[0x58] = &Cpu::op_mov;
  table[0x59] = &Cpu::op_mov;
  table[0x5a] = &Cpu::op_mov;
  table[0x5b] = &Cpu::op_mov;
  table[0x5c] = &Cpu::op_mov;
  table[0x5d] = &Cpu::op_mov;
  table[0x5e] = &Cpu::op_mov;
  table[0x5f] = &Cpu::op_mov;
  table[0x60] = &Cpu::op_mov;
  table[0x61] = &Cpu::op_mov;
  table[0x62] = &Cpu::op_mov;
  table[0x63] = &Cpu::op_mov;
  table[0x64] = &Cpu::op_mov;
  table[0x65] = &Cpu::op_mov;
  table[0x66] = &Cpu::op_mov;
  table[0x67] = &Cpu::op_mov;
  table[0x68] = &Cpu::op_mov;
  table[0x69] = &Cpu::op_mov;
  table[0x6a] = &Cpu::op_mov;
  table[0x6b] = &Cpu::op_mov;
  table[0x6c] = &Cpu::op_mov;
  table[0x6d] = &Cpu::op_mov;
  table[0x6e] = &Cpu::op_mov;
  table[0x6f] = &Cpu::op_mov;
  table[0x70] = &Cpu::op_mov;
  table[0x71] = &Cpu::op_mov;
  table[0x72] = &Cpu::op_mov;
  table[0x73] = &Cpu::op_mov;
  table[0x74] = &Cpu::op_mov;
  table[0x75] = &Cpu::op_mov;
  table[0x77] = &Cpu::op_mov;
  table[0x78] = &Cpu::op_mov;
  table[0x79] = &Cpu::op_mov;
  table[0x7a] = &Cpu::op_mov;
  table[0x7b] = &Cpu::op_mov;
  table[0x7c] = &Cpu::op_mov;
  table[0x7d] = &Cpu::op_mov;
  table[0x7e] = &Cpu::op_mov;
  table[0x7f] = &Cpu::op_mov;
  table[0x80] = &Cpu::op_add;
  table[0x81] = &Cpu::op_add;
  table[0x82] = &Cpu::op_add;
  table[0x83] = &Cpu::op_add;
  table[0x84] = &Cpu::op_add;
  table[0x85] = &Cpu::op_add;
  table[0x86] = &Cpu::op_add;
  table[0x87] = &Cpu::op_add;
  table[0x88] = &Cpu::op_adc;
  table[0x89] = &Cpu::op_adc;
  table[0x8a] = &Cpu::op_adc;
  table[0x8b] = &Cpu::op_adc;
  table[0x8c] = &Cpu::op_adc;
  table[0x8d] = &Cpu::op_adc;
  table[0x8e] = &Cpu::op_adc;
  table[0x8f] = &Cpu::op_adc;
  table[0x90] = &Cpu::op_sub;
  table[0x91] = &Cpu::op_sub;
  table[0x92] = &Cpu::op_sub;
  table[0x93] = &Cpu::op_sub;
  table[0x94] = &Cpu::op_sub;
  table[0x95] = &Cpu::op_sub;
  table[0x96] = &Cpu::op_sub;
  table[0x97] = &Cpu::op_sub;
  table[0x98] = &Cpu::op_sbb;
  table[0x99] = &Cpu::op_sbb;
  table[0x9a] = &Cpu::op_sbb;
  table[0x9b] = &Cpu::op_sbb;
  table[0x9c] = &Cpu::op_sbb;
  table[0x9d] = &Cpu::op_sbb;
  table[0x9e] = &Cpu::op_sbb;
  table[0x9f] = &Cpu::op_sbb;
  table[0xa0] = &Cpu::op_ana;
  table[0xa1] = &Cpu::op_ana;
  table[0xa2] = &Cpu::op_ana;
  table[0xa3] = &Cpu::op_ana;
  table[0xa4] = &Cpu::op_ana;
  table[0xa5] = &Cpu::op_ana;
  table[0xa6] = &Cpu::op_ana;
  table[0xa7] = &Cpu::op_ana;
  table[0xa8] = &Cpu::op_xra;
  table[0xa9] = &Cpu::op_xra;
  table[0xaa] = &Cpu::op_xra;
  table[0xab] = &Cpu::op_xra;
  table[0xac] = &Cpu::op_xra;
  table[0xad] = &Cpu::op_xra;
  table[0xae] = &Cpu::op_xra;
  table[0xaf] = &Cpu::op_xra;
  table[0xb0] = &Cpu::op_ora;
  table[0xb1] = &Cpu::op_ora;
  table[0xb2] = &Cpu::op_ora;
  table[0xb3] = &Cpu::op_ora;
  table[0xb4] = &Cpu::op_ora;
  table[0xb5] = &Cpu::op_ora;
  table[0xb6] = &Cpu::op_ora;
  table[0xb7] = &Cpu::op_ora;
  table[0xb8] = &Cpu::op_cmp;
  table[0xb9] = &Cpu::op_cmp;
  table[0xba] = &Cpu::op_cmp;
  table[0xbb] = &Cpu::op_cmp;
  table[0xbc] = &Cpu::op_cmp;
  table[0xbd] = &Cpu::op_cmp;
  table[0xbe] = &Cpu::op_cmp;
  table[0xbf] = &Cpu::op_cmp;
  table[0xc0] = &Cpu::op_r_ccc;
  table[0xc1] = &Cpu::op_pop;
  table[0xc2] = &Cpu::op_jmp_ccc;
  table[0xc3] = &Cpu::op_jmp;
  table[0xc4] = &Cpu::op_call_ccc;
  table[0xc5] = &Cpu::op_push;
  table[0xc6] = &Cpu::op_adi;
  table[0xc8] = &Cpu::op_r_ccc;
  table[0xc9] = &Cpu::op_ret;
  table[0xca] = &Cpu::op_jmp_ccc;
  table[0xcc] = &Cpu::op_call_ccc;
  table[0xcd] = &Cpu::op_call;
  table[0xce] = &Cpu::op_aci;
  table[0xd0] = &Cpu::op_r_ccc;
  table[0xd1] = &Cpu::op_pop;
  table[0xd2] = &Cpu::op_jmp_ccc;
  table[0xd3] = &Cpu::op_out;
  table[0xd4] = &Cpu::op_call_ccc;
  table[0xd5] = &Cpu::op_push;
  table[0xd6] = &Cpu::op_sui;
  table[0xd8] = &Cpu::op_r_ccc;
  table[0xda] = &Cpu::op_jmp_ccc;
  table[0xdc] = &Cpu::op_call_ccc;
  table[0xde] = &Cpu::op_sbi;
  table[0xe0] = &Cpu::op_r_ccc;
  table[0xe1] = &Cpu::op_pop;
  table[0xe2] = &Cpu::op_jmp_ccc;
  table[0xe3] = &Cpu::op_xthl;
  table[0xe4] = &Cpu::op_call_ccc;
  table[0xe5] = &Cpu::op_push;
  table[0xe6] = &Cpu::op_ani;
  table[0xe8] = &Cpu::op_r_ccc;
  table[0xe9] = &Cpu::op_pchl;
  table[0xea] = &Cpu::op_jmp_ccc;
  table[0xeb] = &Cpu::op_xchg;
  table[0xec] = &Cpu::op_call_ccc;
  table[0xee] = &Cpu::op_xri;
  table[0xf0] = &Cpu::op_r_ccc;
  table[0xf1] = &Cpu::op_pop;
  table[0xf2] = &Cpu::op_jmp_ccc;
  table[0xf4] = &Cpu::op_call_ccc;
  table[0xf5] = &Cpu::op_push;
  table[0xf6] = &Cpu::op_ori;
  table[0xf8] = &Cpu::op_r_ccc;
  table[0xf9] = &Cpu::op_sphl;
  table[0xfa] = &Cpu::op_jmp_ccc;
  table[0xfb] = &Cpu::op_ei;
  table[0xfc] = &Cpu::op_call_ccc;
  table[0xfe] = &Cpu::op_cpi;
}

void Cpu::print_debug() {
#if defined(DEBUG_CPU)
  fmt::println("{}: {:x}, {}: {:x}, {}: {:x}", red("pc"), pc, red("pc - 100"),
               (pc - 0x100), red("sp"), sp);
#else
  fmt::println("{}: {:x}, {}: {:x}", red("pc"), pc, red("sp"), sp);
#endif  // DEBUG
  fmt::println(
      "{}: {:x}, {}: {:x}, {}: {:x}, {}: {:x}, {}: {:x}, {}: {:x}, {}: {:x}",
      red("a"), a, red("b"), b, red("c"), c, red("d"), d, red("e"), e, red("h"),
      h, red("l"), l);
  fmt::println("{}: {}, {}: {}, {}: {}, {}:{}", red("z"), uint8_t(cc.z),
               red("s"), uint8_t(cc.s), red("p"), uint8_t(cc.p), red("cy"),
               uint8_t(cc.cy));
}

void Cpu::unimplemented_instruction() {
  print_debug();
  fmt::println("{:x}: instruction is unimplemented", green(memory[pc]));
  std::exit(EXIT_FAILURE);
}

bool Cpu::parity_iseven(uint16_t n) {
  return not(__builtin_popcount(n) % 2);
}

void Cpu::cycle() {
  ((*this).*(table[memory[pc]]))();
}

void Cpu::op_nop() {
  pc += 1;
  cycles_ = 4;
}

void Cpu::op_jmp() {
  uint16_t old_pc = pc;
  pc += 3;
  cycles_ = 10;
  pc = memory.read_d16_32(old_pc + 1);
}

void Cpu::op_lxi() {
  uint16_t old_pc = pc;
  pc += 3;
  cycles_ = 10;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00110000;
  rp >>= 4;
  switch (rp) {
    case 0u:
      b = memory.read_d8(old_pc + 2);
      c = memory.read_d8(old_pc + 1);
      break;
    case 1u:
      d = memory.read_d8(old_pc + 2);
      e = memory.read_d8(old_pc + 1);
      break;
    case 2u:
      h = memory.read_d8(old_pc + 2);
      l = memory.read_d8(old_pc + 1);
      break;
    case 3u:
      sp = memory.read_d16_32(old_pc + 1);
      break;
  }
}

void Cpu::op_mvi() {
  uint16_t old_pc = pc;
  pc += 2;
  cycles_ = 7;
  uint8_t r = memory.read_d8(old_pc);
  r &= 0b00111000;
  r >>= 3;
  switch (r) {
    case 0u:
      b = memory.read_d8(old_pc + 1);
      break;
    case 1u:
      c = memory.read_d8(old_pc + 1);
      break;
    case 2u:
      d = memory.read_d8(old_pc + 1);
      break;
    case 3u:
      e = memory.read_d8(old_pc + 1);
      break;
    case 4u:
      h = memory.read_d8(old_pc + 1);
      break;
    case 5u:
      l = memory.read_d8(old_pc + 1);
      break;
    case 6u:
      memory[(h << 8) | l] = memory.read_d8(old_pc + 1);
      cycles_ = 10;
      break;
    case 7u:
      a = memory.read_d8(old_pc + 1);
      break;
  }
}

void Cpu::op_call() {
  uint16_t old_pc = pc;
  pc += 3;
  cycles_ = 17;
  memory[sp - 1] = (pc & 0xff00) >> 8;
  memory[sp - 2] = pc & 0xff;
  sp -= 2;
#if defined(DEBUG_CPU)
  uint16_t jmp_addr = memory.read_d16_32(old_pc + 1);
  if (jmp_addr == 0x5) {
    if (c == 9) {
      uint16_t offset = (d << 8) | e;
      auto* str = &memory[offset + 3];
      while (*str != '$') {
        fmt::print("{:c}", *str++);
      }
      fmt::println("");
      std::exit(EXIT_FAILURE);
    } else if (c == 2) {
      fmt::print("print char routine called\n");
    }
  } else if (jmp_addr == 0) {
    std::exit(0);
  } else {
#endif
    pc = memory.read_d16_32(old_pc + 1);
#ifdef DEBUG_CPU
  }
#endif
}

void Cpu::op_ldax() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 7;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00110000;
  rp >>= 4;
  switch (rp) {
    case 0u:
      a = memory.read_d8((b << 8) | c);
      break;
    case 1u:
      a = memory.read_d8((d << 8) | e);
      break;
  }
}

void Cpu::op_mov() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 5;
  uint8_t r1 = memory.read_d8(old_pc);
  r1 &= 0b00111000;
  r1 >>= 3;
  uint8_t r2 = memory.read_d8(old_pc);
  r2 &= 0b00000111;
  switch (r1) {
    case 0u: {
      switch (r2) {
        case 0u:
          break;
        case 1u:
          b = c;
          break;
        case 2u:
          b = d;
          break;
        case 3u:
          b = e;
          break;
        case 4u:
          b = h;
          break;
        case 5u:
          b = l;
          break;
        case 6u:
          b = memory.read_d8((h << 8) | l);
          cycles_ = 7;
          break;
        case 7u:
          b = a;
          break;
      }
    } break;
    case 1u: {
      switch (r2) {
        case 0u:
          c = b;
          break;
        case 1u:
          break;
        case 2u:
          c = d;
          break;
        case 3u:
          c = e;
          break;
        case 4u:
          c = h;
          break;
        case 5u:
          c = l;
          break;
        case 6u:
          c = memory.read_d8((h << 8) | l);
          cycles_ = 7;
          break;
        case 7u:
          c = a;
          break;
      }
    } break;
    case 2u: {
      switch (r2) {
        case 0u:
          d = b;
          break;
        case 1u:
          d = c;
          break;
        case 2u:
          break;
        case 3u:
          d = e;
          break;
        case 4u:
          d = h;
          break;
        case 5u:
          d = l;
          break;
        case 6u:
          d = memory.read_d8((h << 8) | l);
          cycles_ = 7;
          break;
        case 7u:
          d = a;
          break;
      }
    } break;
    case 3u: {
      switch (r2) {
        case 0u:
          e = b;
          break;
        case 1u:
          e = c;
          break;
        case 2u:
          e = d;
          break;
        case 3u:
          break;
        case 4u:
          e = h;
          break;
        case 5u:
          e = l;
          break;
        case 6u:
          e = memory.read_d8((h << 8) | l);
          cycles_ = 7;
          break;
        case 7u:
          e = a;
          break;
      }
    } break;
    case 4u: {
      switch (r2) {
        case 0u:
          h = b;
          break;
        case 1u:
          h = c;
          break;
        case 2u:
          h = d;
          break;
        case 3u:
          h = e;
          break;
        case 4u:
          break;
        case 5u:
          h = l;
          break;
        case 6u:
          h = memory.read_d8((h << 8) | l);
          cycles_ = 7;
          break;
        case 7u:
          h = a;
          break;
      }
    } break;
    case 5u: {
      switch (r2) {
        case 0u:
          l = b;
          break;
        case 1u:
          l = c;
          break;
        case 2u:
          l = d;
          break;
        case 3u:
          l = e;
          break;
        case 4u:
          l = h;
          break;
        case 5u:
          break;
        case 6u:
          l = memory.read_d8((h << 8) | l);
          cycles_ = 7;
          break;
        case 7u:
          l = a;
          break;
      }
    } break;
    case 6u: {
      switch (r2) {
        case 0u:
          memory[(h << 8) | l] = b;
          cycles_ = 7;
          break;
        case 1u:
          memory[(h << 8) | l] = c;
          cycles_ = 7;
          break;
        case 2u:
          memory[(h << 8) | l] = d;
          cycles_ = 7;
          break;
        case 3u:
          memory[(h << 8) | l] = e;
          cycles_ = 7;
          break;
        case 4u:
          memory[(h << 8) | l] = h;
          cycles_ = 7;
          break;
        case 5u:
          memory[(h << 8) | l] = l;
          cycles_ = 7;
          break;
        case 6u:
          cycles_ = 7;
          break;
        case 7u:
          memory[(h << 8) | l] = a;
          cycles_ = 7;
          break;
      }
    } break;
    case 7u: {
      switch (r2) {
        case 0u:
          a = b;
          break;
        case 1u:
          a = c;
          break;
        case 2u:
          a = d;
          break;
        case 3u:
          a = e;
          break;
        case 4u:
          a = h;
          break;
        case 5u:
          a = l;
          break;
        case 6u:
          a = memory.read_d8((h << 8) | l);
          cycles_ = 7;
          break;
        case 7u:
          break;
      }
    } break;
  }
}

void Cpu::op_inx() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 5;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00110000;
  rp >>= 4;
  uint16_t tmp;
  switch (rp) {
    case 0:
      tmp = (b << 8) | c;
      ++tmp;
      b = (tmp & 0xff00) >> 8;
      c = tmp & 0xff;
      break;
    case 1:
      tmp = (d << 8) | e;
      ++tmp;
      d = (tmp & 0xff00) >> 8;
      e = tmp & 0xff;
      break;
    case 2:
      tmp = (h << 8) | l;
      ++tmp;
      h = (tmp & 0xff00) >> 8;
      l = tmp & 0xff;
      break;
    case 3:
      ++sp;
      break;
  }
}

// z, s, p, ac
void Cpu::op_dcr() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 5;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00111000;
  rp >>= 3;
  switch (rp) {
    case 0:
      b -= 1;
      cc.z = (b == 0);
      cc.s = (b & 0x80) >> 7;
      cc.p = parity_iseven(b);
      break;
    case 1:
      c -= 1;
      cc.z = (c == 0);
      cc.s = (c & 0x80) >> 7;
      cc.p = parity_iseven(c);
      break;
    case 2:
      d -= 1;
      cc.z = (d == 0);
      cc.s = (d & 0x80) >> 7;
      cc.p = parity_iseven(d);
      break;
    case 3:
      e -= 1;
      cc.z = (e == 0);
      cc.s = (e & 0x80) >> 7;
      cc.p = parity_iseven(e);
      break;
    case 4:
      h -= 1;
      cc.z = (h == 0);
      cc.s = (h & 0x80) >> 7;
      cc.p = parity_iseven(h);
      break;
    case 5:
      l -= 1;
      cc.z = (l == 0);
      cc.s = (l & 0x80) >> 7;
      cc.p = parity_iseven(l);
      break;
    case 6:
      memory[(h << 8) | l] = memory.read_d8((h << 8) | l) - 1;
      cc.z = (memory[(h << 8) | l] == 0);
      cc.s = (memory[(h << 8) | l] & 0x80) >> 7;
      cc.p = parity_iseven(memory[(h << 8) | l]);
      cycles_ = 10;
      break;
    case 7:
      a -= 1;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      break;
  }
}

void Cpu::op_jmp_ccc() {
  uint16_t old_pc = pc;
  pc += 3;
  cycles_ = 10;
  uint8_t ccc = memory.read_d8(old_pc);
  ccc &= 0b00111000;
  ccc >>= 3;
  switch (ccc) {
    case 0:
      if (cc.z == 0) {
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 1:
      if (cc.z == 1) {
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 2:
      if (cc.cy == 0) {
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 3:
      if (cc.cy == 1) {
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 4:
      if (cc.p == 0) {
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 5:
      if (cc.p == 1) {
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 6:
      if (cc.s == 0) {
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 7:
      if (cc.s == 1) {
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
  }
}

void Cpu::op_ret() {
  pc = (memory.read_d8(sp + 1) << 8) | memory.read_d8(sp);
  sp += 2;
  cycles_ = 10;
}

void Cpu::op_cpi() {
  uint16_t old_pc = pc;
  pc += 2;
  cycles_ = 7;
  uint8_t byte2 = memory.read_d8(old_pc + 1);
  uint16_t res = static_cast<uint16_t>(a) - byte2;
  cc.z = ((res & 0xff) == 0);
  cc.s = ((res & 0x80) == 0x80);
  cc.p = parity_iseven(res & 0xff);
  cc.cy = (res > 0xff);
}

void Cpu::op_push() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 11;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00110000;
  rp >>= 4;
  switch (rp) {
    case 0:
      memory[sp - 1] = b;
      memory[sp - 2] = c;
      sp -= 2;
      break;
    case 1:
      memory[sp - 1] = d;
      memory[sp - 2] = e;
      sp -= 2;
      break;
    case 2:
      memory[sp - 1] = h;
      memory[sp - 2] = l;
      sp -= 2;
      break;
    case 3: {
      memory[sp - 1] = a;
      uint8_t fw{0};
      fw |= (cc.s << 7);
      fw |= (cc.z << 6);
      fw |= (0 << 5);
      fw |= (cc.acy << 4);
      fw |= (0 << 3);
      fw |= (cc.p << 2);
      fw |= (1 << 1);
      fw |= (cc.cy << 0);
      memory[sp - 2] = fw;
      sp -= 2;
    } break;
  }
}

void Cpu::op_dad() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 10;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00110000;
  rp >>= 4;
  uint32_t hl = (h << 8) | l;
  uint16_t tmp;
  switch (rp) {
    case 0:
      tmp = (b << 8) | c;
      hl = hl + tmp;
      h = (hl & 0xff00) >> 8;
      l = hl & 0xff;
      cc.cy = (hl > 0xffff);
      break;
    case 1:
      tmp = (d << 8) | e;
      hl = hl + tmp;
      h = (hl & 0xff00) >> 8;
      l = hl & 0xff;
      cc.cy = (hl > 0xffff);
      break;
    case 2:
      tmp = (h << 8) | l;
      hl = hl + tmp;
      h = (hl & 0xff00) >> 8;
      l = hl & 0xff;
      cc.cy = (hl > 0xffff);
      break;
    case 3:
      hl = hl + sp;
      h = (hl & 0xff00) >> 8;
      l = hl & 0xff;
      cc.cy = (hl > 0xffff);
      break;
  }
}

void Cpu::op_xchg() {
  pc += 1;
  cycles_ = 5;
  std::swap(h, d);
  std::swap(l, e);
}

void Cpu::op_pop() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 10;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00110000;
  rp >>= 4;
  switch (rp) {
    case 0:
      b = memory.read_d8(sp + 1);
      c = memory.read_d8(sp);
      sp += 2;
      break;
    case 1:
      d = memory.read_d8(sp + 1);
      e = memory.read_d8(sp);
      sp += 2;
      break;
    case 2:
      h = memory.read_d8(sp + 1);
      l = memory.read_d8(sp);
      sp += 2;
      break;
    case 3:
      cc.cy = (memory.read_d8(sp) & 0b00000001) >> 0;
      cc.p = (memory.read_d8(sp) & 0b00000100) >> 2;
      cc.acy = (memory.read_d8(sp) & 0b00010000) >> 4;
      cc.z = (memory.read_d8(sp) & 0b01000000) >> 6;
      cc.s = (memory.read_d8(sp) & 0b10000000) >> 7;
      a = memory.read_d8(sp + 1);
      sp += 2;
      break;
  }
}

void Cpu::op_out() {
  pc += 2;
  cycles_ = 10;
  // TODO
}

void Cpu::op_rrc() {
  pc += 1;
  cycles_ = 4;
  cc.cy = a & 0x1;
  a >>= 1;
  a |= (cc.cy << 7);
}

void Cpu::op_ani() {
  uint16_t old_pc = pc;
  pc += 2;
  cycles_ = 7;
  a = a & memory.read_d8(old_pc + 1);
  cc.z = (a == 0);
  cc.s = ((a & 0x80) == 0x80);
  cc.p = parity_iseven(a);
  cc.cy = 0;
  // cc.acy = 0;
}

void Cpu::op_adi() {
  uint16_t old_pc = pc;
  pc += 2;
  cycles_ = 7;
  uint16_t res = static_cast<uint16_t>(a) + memory.read_d8(old_pc + 1);
  a = res & 0xff;
  cc.z = (a == 0);
  cc.s = ((a & 0x80) == 0x80);
  cc.p = parity_iseven(a);
  cc.cy = (res > 0xff);
}

void Cpu::op_lda() {
  uint16_t old_pc = pc;
  pc += 3;
  cycles_ = 13;
  a = memory.read_d8((memory.read_d8(old_pc + 2) << 8) |
                     memory.read_d8(old_pc + 1));
}

void Cpu::op_sta() {
  uint16_t old_pc = pc;
  pc += 3;
  cycles_ = 13;
  memory[(memory[old_pc + 2] << 8) | memory[old_pc + 1]] = a;
}

void Cpu::op_xra() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 4;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00000111;
  switch (rp) {
    case 0:
      a = a ^ b;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 1:
      a = a ^ c;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 2:
      a = a ^ d;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 3:
      a = a ^ e;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 4:
      a = a ^ h;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 5:
      a = a ^ l;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 6:
      a = a ^ memory.read_d8((h << 8) | l);
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      cycles_ = 7;
      break;
    case 7:
      a = a ^ a;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
  }
}

void Cpu::op_ei() {
  pc += 1;
  cycles_ = 4;
  int_enable = true;
}

void Cpu::op_ana() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 4;
  uint8_t r = memory.read_d8(old_pc);
  r &= 0b00000111;
  switch (r) {
    case 0:
      a = a & b;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 1:
      a = a & c;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 2:
      a = a & d;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 3:
      a = a & e;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 4:
      a = a & h;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 5:
      a = a & l;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 6:
      a = a & memory.read_d8((h << 8) | l);
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      cycles_ = 7;
      break;
    case 7:
      a = a & a;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
  }
}

void Cpu::generate_interrupt(uint8_t int_num) {
  if (!int_enable) return;
  int_enable = false;
  uint16_t old_pc = pc;
  pc += 1;
  memory[sp - 1] = (pc & 0xff00) >> 8;
  memory[sp - 2] = pc & 0xff;
  sp -= 2;
  pc = 8 * ((int_num & 0b00111000) >> 3);
}

uint8_t Cpu::get_cycles() {
  return cycles_;
}

void Cpu::op_r_ccc() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 5;
  uint8_t ccc = memory.read_d8(old_pc);
  ccc &= 0b00111000;
  ccc >>= 3;
  switch (ccc) {
    case 0:
      if (cc.z == 0) {
        op_ret();
      }
      break;
    case 1:
      if (cc.z == 1) {
        op_ret();
      }
      break;
    case 2:
      if (cc.cy == 0) {
        op_ret();
      }
      break;
    case 3:
      if (cc.cy == 1) {
        op_ret();
      }
      break;
    case 4:
      if (cc.p == 0) {
        op_ret();
      }
      break;
    case 5:
      if (cc.p == 1) {
        op_ret();
      }
      break;
    case 6:
      if (cc.s == 0) {
        op_ret();
      }
      break;
    case 7:
      if (cc.s == 1) {
        op_ret();
      }
      break;
  }
}

void Cpu::op_ora() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 4;
  uint8_t r = memory.read_d8(old_pc);
  r &= 0b00000111;
  switch (r) {
    case 0:
      a = a | b;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 1:
      a = a | c;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 2:
      a = a | d;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 3:
      a = a | e;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 4:
      a = a | h;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 5:
      a = a | l;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 6:
      a = a | memory.read_d8((h << 8) | l);
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      cycles_ = 7;
      break;
    case 7:
      a = a | a;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
  }
}

void Cpu::op_xthl() {
  pc += 1;
  cycles_ = 18;
  std::swap(l, memory[sp]);
  std::swap(h, memory[sp + 1]);
}

void Cpu::op_pchl() {
  pc += 1;
  cycles_ = 5;
  pc = (h << 8) | l;
}

void Cpu::op_dcx() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 5;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00110000;
  rp >>= 4;
  uint16_t tmp;
  switch (rp) {
    case 0:
      tmp = (b << 8) | c;
      --tmp;
      b = (tmp & 0xff00) >> 8;
      c = tmp & 0xff;
      break;
    case 1:
      tmp = (d << 8) | e;
      --tmp;
      d = (tmp & 0xff00) >> 8;
      e = tmp & 0xff;
      break;
    case 2:
      tmp = (h << 8) | l;
      --tmp;
      h = (tmp & 0xff00) >> 8;
      l = tmp & 0xff;
      break;
    case 3:
      --sp;
      break;
  }
}

void Cpu::op_inr() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 5;
  uint8_t r = memory.read_d8(old_pc);
  r &= 0b00111000;
  r >>= 3;
  switch (r) {
    case 0:
      ++b;
      cc.z = (b == 0);
      cc.s = (b & 0x80) >> 7;
      cc.p = parity_iseven(b);
      break;
    case 1:
      ++c;
      cc.z = (c == 0);
      cc.s = (c & 0x80) >> 7;
      cc.p = parity_iseven(c);
      break;
    case 2:
      ++d;
      cc.z = (d == 0);
      cc.s = (d & 0x80) >> 7;
      cc.p = parity_iseven(d);
      break;
    case 3:
      ++e;
      cc.z = (e == 0);
      cc.s = (e & 0x80) >> 7;
      cc.p = parity_iseven(e);
      break;
    case 4:
      ++h;
      cc.z = (h == 0);
      cc.s = (h & 0x80) >> 7;
      cc.p = parity_iseven(h);
      break;
    case 5:
      ++l;
      cc.z = (l == 0);
      cc.s = (l & 0x80) >> 7;
      cc.p = parity_iseven(l);
      break;
    case 6:
      memory[(h << 8) | l] = memory.read_d8((h << 8) | l) + 1;
      cc.z = (memory[(h << 8) | l] == 0);
      cc.s = (memory[(h << 8) | l] & 0x80) >> 7;
      cc.p = parity_iseven(memory[(h << 8) | l]);
      cycles_ = 10;
      break;
    case 7:
      ++a;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      break;
  }
}

void Cpu::op_call_ccc() {
  uint16_t old_pc = pc;
  pc += 3;
  cycles_ = 11;
  uint8_t ccc = memory.read_d8(old_pc);
  ccc &= 0b00111000;
  ccc >>= 3;
  switch (ccc) {
    case 0:
      if (cc.z == 0) {
        cycles_ = 17;
        memory[sp - 1] = (pc & 0xff00) >> 8;
        memory[sp - 2] = pc & 0xff;
        sp -= 2;
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 1:
      if (cc.z == 1) {
        cycles_ = 17;
        memory[sp - 1] = (pc & 0xff00) >> 8;
        memory[sp - 2] = pc & 0xff;
        sp -= 2;
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 2:
      if (cc.cy == 0) {
        cycles_ = 17;
        memory[sp - 1] = (pc & 0xff00) >> 8;
        memory[sp - 2] = pc & 0xff;
        sp -= 2;
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 3:
      if (cc.cy == 1) {
        cycles_ = 17;
        memory[sp - 1] = (pc & 0xff00) >> 8;
        memory[sp - 2] = pc & 0xff;
        sp -= 2;
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 4:
      if (cc.p == 0) {
        cycles_ = 17;
        memory[sp - 1] = (pc & 0xff00) >> 8;
        memory[sp - 2] = pc & 0xff;
        sp -= 2;
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 5:
      if (cc.p == 1) {
        cycles_ = 17;
        memory[sp - 1] = (pc & 0xff00) >> 8;
        memory[sp - 2] = pc & 0xff;
        sp -= 2;
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 6:
      if (cc.s == 0) {
        cycles_ = 17;
        memory[sp - 1] = (pc & 0xff00) >> 8;
        memory[sp - 2] = pc & 0xff;
        sp -= 2;
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
    case 7:
      if (cc.s == 1) {
        cycles_ = 17;
        memory[sp - 1] = (pc & 0xff00) >> 8;
        memory[sp - 2] = pc & 0xff;
        sp -= 2;
        pc = memory.read_d16_32(old_pc + 1);
      }
      break;
  }
}

void Cpu::op_aci() {
  uint16_t old_pc = pc;
  pc += 2;
  cycles_ = 7;
  uint16_t res = static_cast<uint16_t>(a) + memory.read_d8(old_pc + 1) + cc.cy;
  a = res & 0xff;
  cc.z = (a == 0);
  cc.s = ((a & 0x80) == 0x80);
  cc.p = parity_iseven(a);
  cc.cy = (res > 0xff);
}

void Cpu::op_sui() {
  uint16_t old_pc = pc;
  pc += 2;
  cycles_ = 7;
  uint16_t res = static_cast<uint16_t>(a) - memory.read_d8(old_pc + 1);
  a = res & 0xff;
  cc.z = (a == 0);
  cc.s = ((a & 0x80) == 0x80);
  cc.p = parity_iseven(a);
  cc.cy = (res > 0xff);
}

void Cpu::op_sbi() {
  uint16_t old_pc = pc;
  pc += 2;
  cycles_ = 7;
  uint16_t res = static_cast<uint16_t>(a) - memory.read_d8(old_pc + 1) - cc.cy;
  a = res & 0xff;
  cc.z = (a == 0);
  cc.s = ((a & 0x80) == 0x80);
  cc.p = parity_iseven(a);
  cc.cy = (res > 0xff);
}

void Cpu::op_ori() {
  uint16_t old_pc = pc;
  pc += 2;
  cycles_ = 7;
  a = a | memory.read_d8(old_pc + 1);
  cc.z = (a == 0);
  cc.s = ((a & 0x80) == 0x80);
  cc.p = parity_iseven(a);
  cc.cy = 0;
}

void Cpu::op_xri() {
  uint16_t old_pc = pc;
  pc += 2;
  cycles_ = 7;
  a = a ^ memory.read_d8(old_pc + 1);
  cc.z = (a == 0);
  cc.s = ((a & 0x80) == 0x80);
  cc.p = parity_iseven(a);
  cc.cy = 0;
}

void Cpu::op_add() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 4;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00000111;
  uint16_t res;
  switch (rp) {
    case 0:
      res = a + b;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 1:
      res = a + c;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 2:
      res = a + d;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 3:
      res = a + e;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 4:
      res = a + h;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 5:
      res = a + l;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 6:
      res = a + memory.read_d8((h << 8) | l);
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      cycles_ = 7;
      break;
    case 7:
      res = a + a;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
  }
}

void Cpu::op_sub() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 4;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00000111;
  uint16_t res;
  switch (rp) {
    case 0:
      res = a - b;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 1:
      res = a - c;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 2:
      res = a - d;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 3:
      res = a - e;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 4:
      res = a - h;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 5:
      res = a - l;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 6:
      res = a - memory.read_d8((h << 8) | l);
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      cycles_ = 7;
      break;
    case 7:
      res = a - a;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
  }
}

void Cpu::op_adc() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 4;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00000111;
  uint16_t res;
  switch (rp) {
    case 0:
      res = a + b + cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 1:
      res = a + c + cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 2:
      res = a + d + cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 3:
      res = a + e + cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 4:
      res = a + h + cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 5:
      res = a + l + cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 6:
      res = a + memory.read_d8((h << 8) | l) + cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      cycles_ = 7;
      break;
    case 7:
      res = a + a + cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
  }
}

void Cpu::op_sbb() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 4;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00000111;
  uint16_t res;
  switch (rp) {
    case 0:
      res = a - b - cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 1:
      res = a - c - cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 2:
      res = a - d - cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 3:
      res = a - e - cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 4:
      res = a - h - cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 5:
      res = a - l - cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
    case 6:
      res = a - memory.read_d8((h << 8) | l) - cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      cycles_ = 7;
      break;
    case 7:
      res = a - a - cc.cy;
      a = res & 0xff;
      cc.z = (a == 0);
      cc.s = ((a & 0x80) == 0x80);
      cc.p = parity_iseven(a);
      cc.cy = (res > 0xff);
      break;
  }
}

void Cpu::op_cmp() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 4;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00000111;
  uint16_t res;
  switch (rp) {
    case 0:
      res = a - b;
      cc.z = ((res & 0xff) == 0);
      cc.s = ((res & 0x80) == 0x80);
      cc.p = parity_iseven(res & 0xff);
      cc.cy = (res > 0xff);
      break;
    case 1:
      res = a - c;
      cc.z = ((res & 0xff) == 0);
      cc.s = ((res & 0x80) == 0x80);
      cc.p = parity_iseven(res & 0xff);
      cc.cy = (res > 0xff);
      break;
    case 2:
      res = a - d;
      cc.z = ((res & 0xff) == 0);
      cc.s = ((res & 0x80) == 0x80);
      cc.p = parity_iseven(res & 0xff);
      cc.cy = (res > 0xff);
      break;
    case 3:
      res = a - e;
      cc.z = ((res & 0xff) == 0);
      cc.s = ((res & 0x80) == 0x80);
      cc.p = parity_iseven(res & 0xff);
      cc.cy = (res > 0xff);
      break;
    case 4:
      res = a - h;
      cc.z = ((res & 0xff) == 0);
      cc.s = ((res & 0x80) == 0x80);
      cc.p = parity_iseven(res & 0xff);
      cc.cy = (res > 0xff);
      break;
    case 5:
      res = a - l;
      cc.z = ((res & 0xff) == 0);
      cc.s = ((res & 0x80) == 0x80);
      cc.p = parity_iseven(res & 0xff);
      cc.cy = (res > 0xff);
      break;
    case 6:
      res = a - memory.read_d8((h << 8) | l);
      cc.z = ((res & 0xff) == 0);
      cc.s = ((res & 0x80) == 0x80);
      cc.p = parity_iseven(res & 0xff);
      cc.cy = (res > 0xff);
      cycles_ = 7;
      break;
    case 7:
      res = a - a;
      cc.z = ((res & 0xff) == 0);
      cc.s = ((res & 0x80) == 0x80);
      cc.p = parity_iseven(res & 0xff);
      cc.cy = (res > 0xff);
      break;
  }
}

void Cpu::op_lhld() {
  uint16_t old_pc = pc;
  pc += 3;
  cycles_ = 16;
  l = memory.read_d8((memory.read_d8(old_pc + 2) << 8) |
                     memory.read_d8(old_pc + 1));
  h = memory.read_d8((memory.read_d8(old_pc + 2) << 8) |
                     memory.read_d8(old_pc + 1) + 1);
}

void Cpu::op_shld() {
  uint16_t old_pc = pc;
  pc += 3;
  cycles_ = 16;
  memory[((memory.read_d8(old_pc + 2) << 8) | memory.read_d8(old_pc + 1))] = l;
  memory[((memory.read_d8(old_pc + 2) << 8) | memory.read_d8(old_pc + 1) + 1)] =
      h;
}

void Cpu::op_stax() {
  uint16_t old_pc = pc;
  pc += 1;
  cycles_ = 7;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00110000;
  rp >>= 4;
  switch (rp) {
    case 0:
      memory[(b << 8) | c] = a;
      break;
    case 1:
      memory[(d << 8) | e] = a;
      break;
  }
}

void Cpu::op_stc() {
  pc += 1;
  cycles_ = 4;
  cc.cy = 1;
}

void Cpu::op_cmc() {
  pc += 1;
  cycles_ = 4;
  cc.cy = cc.cy ? 0 : 1;
}

void Cpu::op_cma() {
  pc += 1;
  cycles_ = 4;
  a = ~a;
}

void Cpu::op_rlc() {
  pc += 1;
  cycles_ = 4;
  cc.cy = (a & 0x80) >> 7;
  a <<= 1;
  a |= cc.cy;
}

void Cpu::op_ral() {
  pc += 1;
  cycles_ = 4;
  uint8_t old_cy = cc.cy;
  cc.cy = (a & 0x80) >> 7;
  a <<= 1;
  a |= old_cy;
}

void Cpu::op_rar() {
  pc += 1;
  cycles_ = 4;
  uint8_t old_cy = cc.cy;
  cc.cy = (a & 0x1);
  a >>= 1;
  a |= (old_cy << 7);
}

void Cpu::op_sphl() {
  pc += 1;
  cycles_ = 5;
  sp = (h << 8) | l;
}
