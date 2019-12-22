#ifndef PB_AXI_IIC_H
#define PB_AXI_IIC_H

#include "pb_func_helpers.h"
#include "pb_axi.h"

#define IIC_SOFTRESET      0x0040
#define IIC_CONTROL        0x0100
#define IIC_STATUS         0x0104
#define IIC_TXFIFO         0x0108

#define IIC_STATUS_TX_FIFO_EMPTY 0x80
#define IIC_STATUS_RX_FIFO_EMPTY 0x40
#define IIC_STATUS_RX_FIFO_FULL 0x20
#define IIC_STATUS_TX_FIFO_FULL 0x10
#define IIC_STATUS_SRW 0x08
#define IIC_STATUS_BB 0x04
#define IIC_STATUS_AAS 0x02
#define IIC_STATUS_ABGC 0x01

#define IIC_REG(reg) (IIC_BASE + reg)

#define i2c_reset()				\
  axi_reset_data();                             \
  axi_writek_byte(IIC_REG(IIC_SOFTRESET), 0xA);	\
  axi_writek_byte(IIC_REG(IIC_CONTROL), 0x1)

#define I2C_WRITEREG(addr, reg, val)	        \
  FUNC3( addr, reg, val, i2c_writereg_fn )
void i2c_writereg_fn(void) {
  // This is a callable function, so we grab the next available
  // spare register (sD) for inline functions.
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
    // We need to make sure the TX FIFO is empty,
    // *and* that the bus is not busy.
    // To do that, we first toggle the TX_FIFO_EMPTY bit...
    sA ^= IIC_STATUS_TX_FIFO_EMPTY;
    // and then test to make sure both bits are cleared.
  } while (sA & (IIC_STATUS_BB | IIC_STATUS_TX_FIFO_EMPTY));
  // restore the sSpare definition
#pragma MCPP pop_macro("sSpare")
}

#endif
