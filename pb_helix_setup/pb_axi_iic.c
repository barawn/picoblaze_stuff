#include "pb_axi_iic.h"

#define I2C_WRITEREG(addr, reg, val)	        \
  FUNC3( addr, reg, val, i2c_writereg_fn )
void i2c_writereg_fn(void) {
#pragma MCPP push_macro("sSpare")
#undef sSpare
#define sSpare sD
  // i2c reset, sigh
  i2c_reset();
  // write 0x100 | addr to TXFIFO
  kset_axi_byte(1, 0x1);
  set_axi_byte(0, sA);
  axi_write_constaddr(IIC_REG(IIC_TXFIFO));
  // write reg to TXFIFO
  kset_axi_byte(1, 0x0);
  set_axi_byte(0, sB);
  do_axi_write();
  // write 0x200 | val to TXFIFO
  kset_axi_byte(1, 0x2);
  set_axi_byte(0, sC);
  do_axi_write();
  // Wait for completion by checking IIC_STATUS.
  do {
    axi_read_constaddr(IIC_REG(IIC_STATUS));
    get_axi_byte(0, sA);
  } while (sA |^ 0x80);
#pragma MCPP pop_macro("sSpare")
}

