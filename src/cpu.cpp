#include "cpu.hpp"

#include <fmt/color.h>
#include <fmt/core.h>

#define red(arg) fmt::styled(arg, fmt::fg(fmt::color::red))
#define green(arg) fmt::styled(arg, fmt::fg(fmt::color::green))

Cpu::Cpu(Memory& memory)
    : a{}, b{}, c{}, d{}, e{}, h{}, l{}, cc{}, pc{}, sp{}, memory{memory},
      table{std::make_unique<func[]>(NUM_OPCODE + 1)} {
  for (int i{0}; i <= 0xff; ++i) {
    table[i] = &Cpu::unimplemented_instruction;
  }
  table[0x00] = &Cpu::op_nop;
  table[0x01] = &Cpu::op_lxi;
  table[0x03] = &Cpu::op_inx;
  table[0x05] = &Cpu::op_dcr;
  table[0x06] = &Cpu::op_mvi;
  table[0x09] = &Cpu::op_dad;
  table[0x0a] = &Cpu::op_ldax;
  table[0x0d] = &Cpu::op_dcr;
  table[0x0e] = &Cpu::op_mvi;
  table[0x0f] = &Cpu::op_rrc;
  table[0x11] = &Cpu::op_lxi;
  table[0x13] = &Cpu::op_inx;
  table[0x15] = &Cpu::op_dcr;
  table[0x16] = &Cpu::op_mvi;
  table[0x19] = &Cpu::op_dad;
  table[0x1d] = &Cpu::op_dcr;
  table[0x1e] = &Cpu::op_mvi;
  table[0x1a] = &Cpu::op_ldax;
  table[0x21] = &Cpu::op_lxi;
  table[0x23] = &Cpu::op_inx;
  table[0x25] = &Cpu::op_dcr;
  table[0x26] = &Cpu::op_mvi;
  table[0x29] = &Cpu::op_dad;
  table[0x2d] = &Cpu::op_dcr;
  table[0x2e] = &Cpu::op_mvi;
  table[0x31] = &Cpu::op_lxi;
  table[0x32] = &Cpu::op_sta;
  table[0x33] = &Cpu::op_inx;
  table[0x35] = &Cpu::op_dcr;
  table[0x36] = &Cpu::op_mvi;
  table[0x39] = &Cpu::op_dad;
  table[0x3a] = &Cpu::op_lda;
  table[0x3d] = &Cpu::op_dcr;
  table[0x3e] = &Cpu::op_mvi;
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
  table[0xc1] = &Cpu::op_pop;
  table[0xc2] = &Cpu::op_jmp_ccc;
  table[0xc3] = &Cpu::op_jmp;
  table[0xc5] = &Cpu::op_push;
  table[0xc6] = &Cpu::op_adi;
  table[0xc9] = &Cpu::op_ret;
  table[0xca] = &Cpu::op_jmp_ccc;
  table[0xcd] = &Cpu::op_call;
  table[0xd1] = &Cpu::op_pop;
  table[0xd2] = &Cpu::op_jmp_ccc;
  table[0xd3] = &Cpu::op_out;
  table[0xd5] = &Cpu::op_push;
  table[0xda] = &Cpu::op_jmp_ccc;
  table[0xe1] = &Cpu::op_pop;
  table[0xe2] = &Cpu::op_jmp_ccc;
  table[0xe5] = &Cpu::op_push;
  table[0xe6] = &Cpu::op_ani;
  table[0xea] = &Cpu::op_jmp_ccc;
  table[0xeb] = &Cpu::op_xchg;
  table[0xf1] = &Cpu::op_pop;
  table[0xf2] = &Cpu::op_jmp_ccc;
  table[0xf5] = &Cpu::op_push;
  table[0xfa] = &Cpu::op_jmp_ccc;
  table[0xfb] = &Cpu::op_ei;
  table[0xfe] = &Cpu::op_cpi;
}

void Cpu::print_debug() {
  fmt::println("{}: {:x}, {}: {:x}", red("pc"), pc, red("sp"), sp);
  fmt::println(
      "{}: {:x}, {}: {:x}, {}: {:x}, {}: {:x}, {}: {:x}, {}: {:x}, {}: {:x}",
      red("a"), a, red("b"), b, red("c"), c, red("d"), d, red("e"), e, red("h"),
      h, red("l"), l);
  fmt::println("{}: {:08b}", red("cc"), *reinterpret_cast<uint8_t*>(&cc));
}

void Cpu::unimplemented_instruction() {
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
}

void Cpu::op_jmp() {
  uint16_t old_pc = pc;
  pc += 3;
  pc = memory.read_d16_32(old_pc + 1);
}

void Cpu::op_lxi() {
  uint16_t old_pc = pc;
  pc += 3;
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
      break;
    case 7u:
      a = memory.read_d8(old_pc + 1);
      break;
  }
}

void Cpu::op_call() {
  uint16_t old_pc = pc;
  pc += 3;
  memory[sp - 1] = (pc & 0xff00) >> 8;
  memory[sp - 2] = pc & 0xff;
  sp -= 2;
  pc = memory.read_d16_32(old_pc + 1);
}

void Cpu::op_ldax() {
  uint16_t old_pc = pc;
  pc += 1;
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
          d = l;
          break;
        case 6u:
          c = memory.read_d8((h << 8) | l);
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
          break;
        case 1u:
          memory[(h << 8) | l] = c;
          break;
        case 2u:
          memory[(h << 8) | l] = d;
          break;
        case 3u:
          memory[(h << 8) | l] = e;
          break;
        case 4u:
          memory[(h << 8) | l] = h;
          break;
        case 5u:
          memory[(h << 8) | l] = l;
          break;
        case 6u:
          break;
        case 7u:
          memory[(h << 8) | l] = a;
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
}

void Cpu::op_cpi() {
  uint16_t old_pc = pc;
  pc += 2;
  uint8_t byte2 = memory.read_d8(old_pc + 1);
  uint16_t res = static_cast<uint16_t>(a) - byte2;
  cc.z = (a == byte2);
  cc.s = (res & 0x80) >> 7;
  cc.p = parity_iseven(res & 0xff);
  cc.cy = (a < byte2);
}

void Cpu::op_push() {
  uint16_t old_pc = pc;
  pc += 1;
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
  std::swap(h, d);
  std::swap(l, e);
}

void Cpu::op_pop() {
  uint16_t old_pc = pc;
  pc += 1;
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
  // TODO
}

void Cpu::op_rrc() {
  pc += 1;
  cc.cy = a & 0x1;
  a >>= 1;
  a |= (cc.cy << 7);
}

void Cpu::op_ani() {
  uint16_t old_pc = pc;
  pc += 2;
  a = a and memory.read_d8(old_pc + 1);
  cc.z = (a == 0);
  cc.s = (a & 0x80) >> 7;
  cc.p = parity_iseven(a);
  cc.cy = 0;
  // cc.acy = 0;
}

void Cpu::op_adi() {
  uint16_t old_pc = pc;
  pc += 2;
  uint16_t res = static_cast<uint16_t>(a) + memory.read_d8(old_pc + 1);
  a = res & 0xff;
  cc.z = (a == 0);
  cc.s = (a & 0x80) >> 7;
  cc.p = parity_iseven(a);
  cc.cy = (res > 0xff);
}

void Cpu::op_lda() {
  uint16_t old_pc = pc;
  pc += 3;
  a = memory.read_d16_32(old_pc + 1);
}

void Cpu::op_sta() {
  uint16_t old_pc = pc;
  pc += 3;
  memory[(memory[old_pc + 2] << 8) | memory[old_pc + 1]] = a;
}

void Cpu::op_xra() {
  uint16_t old_pc = pc;
  pc += 1;
  uint8_t rp = memory.read_d8(old_pc);
  rp &= 0b00000111;
  switch (rp) {
    case 0:
      a = a xor b;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 1:
      a = a xor c;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 2:
      a = a xor d;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 3:
      a = a xor e;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 4:
      a = a xor h;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 5:
      a = a xor l;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 6:
      a = a xor memory.read_d8((h << 8) | l);
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 7:
      a = a xor a;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
  }
}

void Cpu::op_ei() {
  pc += 1;
  // TODO
}

void Cpu::op_ana() {
  uint16_t old_pc = pc;
  pc += 1;
  uint8_t r = memory.read_d8(old_pc);
  r &= 0b00000111;
  switch (r) {
    case 0:
      a = a and b;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 1:
      a = a and c;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 2:
      a = a and d;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 3:
      a = a and e;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 4:
      a = a and h;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 5:
      a = a and l;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 6:
      a = a and memory.read_d8((h << 8) | l);
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
    case 7:
      a = a and a;
      cc.z = (a == 0);
      cc.s = (a & 0x80) >> 7;
      cc.p = parity_iseven(a);
      cc.cy = 0;
      break;
  }
}
