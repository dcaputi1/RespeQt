//
// disassembly810 class
// (c) 2018 Eric BACHER
//

#ifndef DISASSEMBLY810_H
#define DISASSEMBLY810_H

#include "diskimages/cpu6502.h"

namespace DiskImages {

  class disassembly810 : public Cpu6502 {
  public:
    disassembly810();
    virtual ~disassembly810() {}

    // read/write a byte in memory (unused)
    virtual unsigned char ReadByte(unsigned short) { return 0; }
    virtual void WriteByte(unsigned short, unsigned char) {}

    // enable/disable traces (unused)
    virtual void Trace(int, bool, const char *, ...) {}
    __attribute__((unused)) virtual void Dump(char *, int) {}
    virtual bool IsAddressSkipped(unsigned short) { return false; }

    virtual char *GetAddressLabel(unsigned short addr);
  };
}
#endif// DISASSEMBLY810_H
