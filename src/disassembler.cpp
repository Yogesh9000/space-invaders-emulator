#include <fmt/color.h>
#include <fmt/core.h>

#include <cstdint>
#include <fstream>
#include <memory>
#include <string_view>

class Disassembler {
public:
  Disassembler() = default;
  Disassembler(Disassembler &&) = delete;
  Disassembler(const Disassembler &) = delete;
  Disassembler &operator=(Disassembler &&) = delete;
  Disassembler &operator=(const Disassembler &) = delete;
  ~Disassembler() = default;

  void loadexe(std::string_view file);
  void Disassemble();
  void print_instruction(std::string_view opcode, std::string_view arg = "");
  void print_instruction(std::string_view opcode, std::string_view arg1,
                         std::string_view arg2);
  void print_instruction(std::string_view opcode, uint8_t arg);
  void print_instruction(std::string_view opcode, std::string_view arg1,
                         uint8_t arg2);
  void print_instruction(std::string_view opcode, uint8_t arg1, uint8_t arg2);
  void print_instruction(std::string_view opcode, std::string_view arg,
                         uint8_t arg2, uint8_t arg3);

private:
  std::unique_ptr<uint8_t[]> code{};
  int pc{0};
  int size{};
};

void Disassembler::loadexe(std::string_view file) {
  std::ifstream fileHandle(file.data(), std::ios::binary | std::ios::ate);
  if (!fileHandle.is_open()) {
    fmt::println(stderr, "Failed to load executable");
    return;
  }
  size = fileHandle.tellg();
  code = std::make_unique<uint8_t[]>(size);
  fileHandle.seekg(0, std::ios::beg);
  fileHandle.read(reinterpret_cast<char *>(code.get()), size);
  fileHandle.close();
}

void Disassembler::print_instruction(std::string_view opcode,
                                     std::string_view arg) {
  fmt::println("{:<4x}: {:#02x} {:<4} {}", pc, code[pc],
               fmt::styled(opcode, fmt::fg(fmt::color::red)),
               fmt::styled(arg, fmt::fg(fmt::color::green)));
}

void Disassembler::print_instruction(std::string_view opcode,
                                     std::string_view arg1,
                                     std::string_view arg2) {
  fmt::println("{:<4x}: {:#02x} {:<4} {:<4}  {}", pc, code[pc],
               fmt::styled(opcode, fmt::fg(fmt::color::red)),
               fmt::styled(arg1, fmt::fg(fmt::color::green)),
               fmt::styled(arg2, fmt::fg(fmt::color::green)));
}

void Disassembler::print_instruction(std::string_view opcode, uint8_t arg) {
  fmt::println("{:<4x}: {:#02x} {:<4} {:<4}  #${:#02x}", pc, code[pc],
               fmt::styled(opcode, fmt::fg(fmt::color::red)), "",
               fmt::styled(arg, fmt::fg(fmt::color::aqua)));
}

void Disassembler::print_instruction(std::string_view opcode,
                                     std::string_view arg1, uint8_t arg2) {
  fmt::println("{:<4x}: {:#02x} {:<4} {:<4}  #${:#02x}", pc, code[pc],
               fmt::styled(opcode, fmt::fg(fmt::color::red)),
               fmt::styled(arg1, fmt::fg(fmt::color::green)),
               fmt::styled(arg2, fmt::fg(fmt::color::aqua)));
}

void Disassembler::print_instruction(std::string_view opcode, uint8_t arg1,
                                     uint8_t arg2) {
  fmt::println("{:<4x}: {:#02x} {:<4} {:<4}  #${:#02x}{:02x}", pc, code[pc],
               fmt::styled(opcode, fmt::fg(fmt::color::red)), "",
               fmt::styled(arg1, fmt::fg(fmt::color::aqua)),
               fmt::styled(arg2, fmt::fg(fmt::color::aqua)));
}

void Disassembler::print_instruction(std::string_view opcode,
                                     std::string_view arg, uint8_t arg2,
                                     uint8_t arg3) {
  fmt::println("{:<4x}: {:#02x} {:<4} {:<4}  #${:#02x}{:02x}", pc, code[pc],
               fmt::styled(opcode, fmt::fg(fmt::color::red)),
               fmt::styled(arg, fmt::fg(fmt::color::green)),
               fmt::styled(arg2, fmt::fg(fmt::color::aqua)),
               fmt::styled(arg3, fmt::fg(fmt::color::aqua)));
}

void Disassembler::Disassemble() {
  if (!code) {
    fmt::println(stderr, "Load Binary First");
    return;
  }
  while (pc < size) {
    uint8_t opcode = code[pc];
    int opsize{1};

    switch (opcode) {
      case 0x0:
        print_instruction("NOP");
        break;
      case 0x1:
        print_instruction("LXI", "B", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0x2:
        print_instruction("STAX", "B");
        break;
      case 0x3:
        print_instruction("INX", "B");
        break;
      case 0x4:
        print_instruction("INR", "B");
        break;
      case 0x5:
        print_instruction("DCR", "B");
        break;
      case 0x6:
        print_instruction("MVI", "B", code[pc + 1]);
        opsize = 2;
        break;
      case 0x7:
        print_instruction("RLC");
        break;
      case 0x8:
        print_instruction("-");
        break;
      case 0x9:
        print_instruction("DAD", "B");
        break;
      case 0xa:
        print_instruction("LDAX", "B");
        break;
      case 0xb:
        print_instruction("DCX", "B");
        break;
      case 0xc:
        print_instruction("INR", "C");
        break;
      case 0xd:
        print_instruction("DCR", "C");
        break;
      case 0xe:
        print_instruction("MVI", "C", code[pc + 1]);
        opsize = 2;
        break;
      case 0xf:
        print_instruction("RRC");
        break;
      case 0x10:
        print_instruction("-");
        break;
      case 0x11:
        print_instruction("LXI", "D", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0x12:
        print_instruction("STAX", "D");
        break;
      case 0x13:
        print_instruction("INX", "D");
        break;
      case 0x14:
        print_instruction("INR", "D");
        break;
      case 0x15:
        print_instruction("DCR", "D");
        break;
      case 0x16:
        print_instruction("MVI", "D", code[pc + 1]);
        opsize = 2;
        break;
      case 0x17:
        print_instruction("RAL");
        break;
      case 0x18:
        print_instruction("-");
        break;
      case 0x19:
        print_instruction("DAD", "D");
        break;
      case 0x1a:
        print_instruction("LDAX", "D");
        break;
      case 0x1b:
        print_instruction("DCX", "D");
        break;
      case 0x1c:
        print_instruction("INR", "E");
        break;
      case 0x1d:
        print_instruction("DCR", "E");
        break;
      case 0x1e:
        print_instruction("MVI", "E", code[pc + 1]);
        opsize = 2;
        break;
      case 0x1f:
        print_instruction("RAR");
        break;
      case 0x20:
        print_instruction("-");
        break;
      case 0x21:
        print_instruction("LXI", "H", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0x22:
        print_instruction("SHLD", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0x23:
        print_instruction("INX", "H");
        break;
      case 0x24:
        print_instruction("INR", "H");
        break;
      case 0x25:
        print_instruction("DCR", "H");
        break;
      case 0x26:
        print_instruction("MVI", "H", code[pc + 1]);
        opsize = 2;
        break;
      case 0x27:
        print_instruction("DAA");
        break;
      case 0x28:
        print_instruction("-");
        break;
      case 0x29:
        print_instruction("DAD", "H");
        break;
      case 0x2a:
        print_instruction("LHLD", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0x2b:
        print_instruction("DCX", "H");
        break;
      case 0x2c:
        print_instruction("INR", "L");
        break;
      case 0x2d:
        print_instruction("DCR", "L");
        break;
      case 0x2e:
        print_instruction("MVI", "L", code[pc + 1]);
        opsize = 2;
        break;
      case 0x2f:
        print_instruction("CMA");
        break;
      case 0x30:
        print_instruction("-");
        break;
      case 0x31:
        print_instruction("LXI", "SP", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0x32:
        print_instruction("STA", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0x33:
        print_instruction("INX", "SP");
        break;
      case 0x34:
        print_instruction("INR", "M");
        break;
      case 0x35:
        print_instruction("DCR", "M");
        break;
      case 0x36:
        print_instruction("MVI", "M", code[pc + 1]);
        opsize = 2;
        break;
      case 0x37:
        print_instruction("STC");
        break;
      case 0x38:
        print_instruction("-");
        break;
      case 0x39:
        print_instruction("DAD", "SP");
        break;
      case 0x3a:
        print_instruction("LDA", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0x3b:
        print_instruction("DCX", "SP");
        break;
      case 0x3c:
        print_instruction("INR", "A");
        break;
      case 0x3d:
        print_instruction("DCR", "A");
        break;
      case 0x3e:
        print_instruction("MVI", "A", code[pc + 1]);
        opsize = 2;
        break;
      case 0x3f:
        print_instruction("CMC");
        break;
      case 0x40:
        print_instruction("MOV", "B", "B");
        break;
      case 0x41:
        print_instruction("MOV", "B", "C");
        break;
      case 0x42:
        print_instruction("MOV", "B", "D");
        break;
      case 0x43:
        print_instruction("MOV", "B", "E");
        break;
      case 0x44:
        print_instruction("MOV", "B", "H");
        break;
      case 0x45:
        print_instruction("MOV", "B", "L");
        break;
      case 0x46:
        print_instruction("MOV", "B", "M");
        break;
      case 0x47:
        print_instruction("MOV", "B", "A");
        break;
      case 0x48:
        print_instruction("MOV", "C", "B");
        break;
      case 0x49:
        print_instruction("MOV", "C", "C");
        break;
      case 0x4a:
        print_instruction("MOV", "C", "D");
        break;
      case 0x4b:
        print_instruction("MOV", "C", "E");
        break;
      case 0x4c:
        print_instruction("MOV", "C", "H");
        break;
      case 0x4d:
        print_instruction("MOV", "C", "L");
        break;
      case 0x4e:
        print_instruction("MOV", "C", "M");
        break;
      case 0x4f:
        print_instruction("MOV", "C", "A");
        break;
      case 0x50:
        print_instruction("MOV", "D", "B");
        break;
      case 0x51:
        print_instruction("MOV", "D", "C");
        break;
      case 0x52:
        print_instruction("MOV", "D", "D");
        break;
      case 0x53:
        print_instruction("MOV", "D", "E");
        break;
      case 0x54:
        print_instruction("MOV", "D", "H");
        break;
      case 0x55:
        print_instruction("MOV", "D", "L");
        break;
      case 0x56:
        print_instruction("MOV", "D", "M");
        break;
      case 0x57:
        print_instruction("MOV", "D", "A");
        break;
      case 0x58:
        print_instruction("MOV", "E", "B");
        break;
      case 0x59:
        print_instruction("MOV", "E", "C");
        break;
      case 0x5a:
        print_instruction("MOV", "E", "D");
        break;
      case 0x5b:
        print_instruction("MOV", "E", "E");
        break;
      case 0x5c:
        print_instruction("MOV", "E", "H");
        break;
      case 0x5d:
        print_instruction("MOV", "E", "L");
        break;
      case 0x5e:
        print_instruction("MOV", "E", "M");
        break;
      case 0x5f:
        print_instruction("MOV", "E", "A");
        break;
      case 0x60:
        print_instruction("MOV", "H", "B");
        break;
      case 0x61:
        print_instruction("MOV", "H", "C");
        break;
      case 0x62:
        print_instruction("MOV", "H", "D");
        break;
      case 0x63:
        print_instruction("MOV", "H", "E");
        break;
      case 0x64:
        print_instruction("MOV", "H", "H");
        break;
      case 0x65:
        print_instruction("MOV", "H", "L");
        break;
      case 0x66:
        print_instruction("MOV", "H", "M");
        break;
      case 0x67:
        print_instruction("MOV", "H", "A");
        break;
      case 0x68:
        print_instruction("MOV", "L", "B");
        break;
      case 0x69:
        print_instruction("MOV", "L", "C");
        break;
      case 0x6a:
        print_instruction("MOV", "L", "D");
        break;
      case 0x6b:
        print_instruction("MOV", "L", "E");
        break;
      case 0x6c:
        print_instruction("MOV", "L", "H");
        break;
      case 0x6d:
        print_instruction("MOV", "L", "L");
        break;
      case 0x6e:
        print_instruction("MOV", "L", "M");
        break;
      case 0x6f:
        print_instruction("MOV", "L", "A");
        break;
      case 0x70:
        print_instruction("MOV", "M", "B");
        break;
      case 0x71:
        print_instruction("MOV", "M", "C");
        break;
      case 0x72:
        print_instruction("MOV", "M", "D");
        break;
      case 0x73:
        print_instruction("MOV", "M", "E");
        break;
      case 0x74:
        print_instruction("MOV", "M", "H");
        break;
      case 0x75:
        print_instruction("MOV", "M", "L");
        break;
      case 0x76:
        print_instruction("HLT");
        break;
      case 0x77:
        print_instruction("MOV", "M", "A");
        break;
      case 0x78:
        print_instruction("MOV", "A", "B");
        break;
      case 0x79:
        print_instruction("MOV", "A", "C");
        break;
      case 0x7a:
        print_instruction("MOV", "A", "D");
        break;
      case 0x7b:
        print_instruction("MOV", "A", "E");
        break;
      case 0x7c:
        print_instruction("MOV", "A", "H");
        break;
      case 0x7d:
        print_instruction("MOV", "A", "L");
        break;
      case 0x7e:
        print_instruction("MOV", "A", "M");
        break;
      case 0x7f:
        print_instruction("MOV", "A", "A");
        break;
      case 0x80:
        print_instruction("ADD", "B");
        break;
      case 0x81:
        print_instruction("ADD", "C");
        break;
      case 0x82:
        print_instruction("ADD", "D");
        break;
      case 0x83:
        print_instruction("ADD", "E");
        break;
      case 0x84:
        print_instruction("ADD", "H");
        break;
      case 0x85:
        print_instruction("ADD", "L");
        break;
      case 0x86:
        print_instruction("ADD", "M");
        break;
      case 0x87:
        print_instruction("ADD", "A");
        break;
      case 0x88:
        print_instruction("ADC", "B");
        break;
      case 0x89:
        print_instruction("ADC", "C");
        break;
      case 0x8a:
        print_instruction("ADC", "D");
        break;
      case 0x8b:
        print_instruction("ADC", "E");
        break;
      case 0x8c:
        print_instruction("ADC", "H");
        break;
      case 0x8d:
        print_instruction("ADC", "L");
        break;
      case 0x8e:
        print_instruction("ADC", "M");
        break;
      case 0x8f:
        print_instruction("ADC", "A");
        break;
      case 0x90:
        print_instruction("SUB", "B");
        break;
      case 0x91:
        print_instruction("SUB", "C");
        break;
      case 0x92:
        print_instruction("SUB", "D");
        break;
      case 0x93:
        print_instruction("SUB", "E");
        break;
      case 0x94:
        print_instruction("SUB", "H");
        break;
      case 0x95:
        print_instruction("SUB", "L");
        break;
      case 0x96:
        print_instruction("SUB", "M");
        break;
      case 0x97:
        print_instruction("SUB", "A");
        break;
      case 0x98:
        print_instruction("SBB", "B");
        break;
      case 0x99:
        print_instruction("SBB", "C");
        break;
      case 0x9a:
        print_instruction("SBB", "D");
        break;
      case 0x9b:
        print_instruction("SBB", "E");
        break;
      case 0x9c:
        print_instruction("SBB", "H");
        break;
      case 0x9d:
        print_instruction("SBB", "L");
        break;
      case 0x9e:
        print_instruction("SBB", "M");
        break;
      case 0x9f:
        print_instruction("SBB", "A");
        break;
      case 0xa0:
        print_instruction("ANA", "B");
        break;
      case 0xa1:
        print_instruction("ANA", "C");
        break;
      case 0xa2:
        print_instruction("ANA", "D");
        break;
      case 0xa3:
        print_instruction("ANA", "E");
        break;
      case 0xa4:
        print_instruction("ANA", "H");
        break;
      case 0xa5:
        print_instruction("ANA", "L");
        break;
      case 0xa6:
        print_instruction("ANA", "M");
        break;
      case 0xa7:
        print_instruction("ANA", "A");
        break;
      case 0xa8:
        print_instruction("XRA", "B");
        break;
      case 0xa9:
        print_instruction("XRA", "C");
        break;
      case 0xaa:
        print_instruction("XRA", "D");
        break;
      case 0xab:
        print_instruction("XRA", "E");
        break;
      case 0xac:
        print_instruction("XRA", "H");
        break;
      case 0xad:
        print_instruction("XRA", "L");
        break;
      case 0xae:
        print_instruction("XRA", "M");
        break;
      case 0xaf:
        print_instruction("XRA", "A");
        break;
      case 0xb0:
        print_instruction("ORA", "B");
        break;
      case 0xb1:
        print_instruction("ORA", "C");
        break;
      case 0xb2:
        print_instruction("ORA", "D");
        break;
      case 0xb3:
        print_instruction("ORA", "E");
        break;
      case 0xb4:
        print_instruction("ORA", "H");
        break;
      case 0xb5:
        print_instruction("ORA", "L");
        break;
      case 0xb6:
        print_instruction("ORA", "M");
        break;
      case 0xb7:
        print_instruction("ORA", "A");
        break;
      case 0xb8:
        print_instruction("CMP", "B");
        break;
      case 0xb9:
        print_instruction("CMP", "C");
        break;
      case 0xba:
        print_instruction("CMP", "D");
        break;
      case 0xbb:
        print_instruction("CMP", "E");
        break;
      case 0xbc:
        print_instruction("CMP", "H");
        break;
      case 0xbd:
        print_instruction("CMP", "L");
        break;
      case 0xbe:
        print_instruction("CMP", "M");
        break;
      case 0xbf:
        print_instruction("CMP", "A");
        break;
      case 0xc0:
        print_instruction("RNZ");
        break;
      case 0xc1:
        print_instruction("POP", "B");
        break;
      case 0xc2:
        print_instruction("JNZ", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xc3:
        print_instruction("JMP", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xc4:
        print_instruction("CNZ", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xc5:
        print_instruction("PUSH", "B");
        break;
      case 0xc6:
        print_instruction("ADI", code[pc + 1]);
        opsize = 2;
        break;
      case 0xc7:
        print_instruction("RST", "0");
        break;
      case 0xc8:
        print_instruction("RZ");
        break;
      case 0xc9:
        print_instruction("RET");
        break;
      case 0xca:
        print_instruction("JZ", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xcb:
        print_instruction("-");
        break;
      case 0xcc:
        print_instruction("CZ", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xcd:
        print_instruction("CALL", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xce:
        print_instruction("ACI", code[pc + 1]);
        opsize = 2;
        break;
      case 0xcf:
        print_instruction("RST", "1");
        break;
      case 0xd0:
        print_instruction("RNC");
        break;
      case 0xd1:
        print_instruction("POP", "D");
        break;
      case 0xd2:
        print_instruction("JNC", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xd3:
        print_instruction("OUT", code[pc + 1]);
        opsize = 2;
        break;
      case 0xd4:
        print_instruction("CNC", code[pc + 2], code[pc + 2]);
        opsize = 3;
        break;
      case 0xd5:
        print_instruction("PUSH", "D");
        break;
      case 0xd6:
        print_instruction("SUI", code[pc + 1]);
        opsize = 2;
        break;
      case 0xd7:
        print_instruction("RST", "2");
        break;
      case 0xd8:
        print_instruction("RC");
        break;
      case 0xd9:
        print_instruction("-");
        break;
      case 0xda:
        print_instruction("JC", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xdb:
        print_instruction("IN", code[pc + 1]);
        opsize = 2;
        break;
      case 0xdc:
        print_instruction("CC", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xdd:
        print_instruction("-");
        break;
      case 0xde:
        print_instruction("SBI", code[pc + 1]);
        opsize = 2;
        break;
      case 0xdf:
        print_instruction("RST", "3");
        break;
      case 0xe0:
        print_instruction("RPO");
        break;
      case 0xe1:
        print_instruction("POP", "H");
        break;
      case 0xe2:
        print_instruction("JPO", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xe3:
        print_instruction("XTHL");
        break;
      case 0xe4:
        print_instruction("CPO", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xe5:
        print_instruction("PUSH", "H");
        break;
      case 0xe6:
        print_instruction("ANI", code[pc + 1]);
        opsize = 2;
        break;
      case 0xe7:
        print_instruction("RST", "4");
        break;
      case 0xe8:
        print_instruction("RPE");
        break;
      case 0xe9:
        print_instruction("PCHL");
        break;
      case 0xea:
        print_instruction("JPE", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xeb:
        print_instruction("XCHG");
        break;
      case 0xec:
        print_instruction("CPE", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xed:
        print_instruction("-");
        break;
      case 0xee:
        print_instruction("XRI", code[pc + 1]);
        opsize = 2;
        break;
      case 0xef:
        print_instruction("RST", "5");
        break;
      case 0xf0:
        print_instruction("RP");
        break;
      case 0xf1:
        print_instruction("POP", "PSW");
        break;
      case 0xf2:
        print_instruction("JP", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xf3:
        print_instruction("DI");
        break;
      case 0xf4:
        print_instruction("CP", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xf5:
        print_instruction("PUSH", "PSW");
        break;
      case 0xf6:
        print_instruction("ORI", code[pc + 1]);
        opsize = 2;
        break;
      case 0xf7:
        print_instruction("RST", "6");
        break;
      case 0xf8:
        print_instruction("RM");
        break;
      case 0xf9:
        print_instruction("SPHL");
        break;
      case 0xfa:
        print_instruction("JM", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xfb:
        print_instruction("EI");
        break;
      case 0xfc:
        print_instruction("CM", code[pc + 2], code[pc + 1]);
        opsize = 3;
        break;
      case 0xfd:
        print_instruction("-");
        break;
      case 0xfe:
        print_instruction("CPI", code[pc + 1]);
        opsize = 2;
        break;
      case 0xff:
        print_instruction("RST", "7");
        break;
    }
    pc += opsize;
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fmt::println(stderr, "USAGE:{} <Executable>", argv[0]);
    std::exit(EXIT_FAILURE);
  }
  Disassembler disassembler;
  disassembler.loadexe(argv[1]);
  disassembler.Disassemble();
  return 0;
}
