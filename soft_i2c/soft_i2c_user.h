#include "soft_i2c.h"

// writes bytes, starting from buffer
// pointed at sA and going BACKWARDS
// to I2C base.
// on return sA contains number of remaining bytes
// (Z is set on total success)
void I2C_write_bytes() {
  // save sA since sA/sB get clobbered
  // in I2C_Tx_byte_and_Rx_ACK()
  sC = sA;
  I2C_start();
  I2C_write_bytes_process();
  I2C_stop();
}

// read register sA (returned back in sA)
// device address is already in I2C_BUFFER(1)
void I2C_reg_read() {
  store(I2C_BUFFER(0), sA);
  sC = I2C_BUFFER(1);
  I2C_start();
  I2C_write_bytes_process();
  psm("jump NZ, I2C_reg_read_failure");
  fetch(I2C_BUFFER(1), sA);
  sA |= 0x1;
  I2C_start();
  I2C_Tx_byte_and_Rx_ACK();
  psm("jump C, I2C_reg_read_failure");
  I2C_Rx_byte();
  // sA isn't clobbered by anything
  I2C_Tx_NACK();
  // this is a bad idea, obviously,
  // need to fix this somehow
 I2C_reg_read_failure:
  I2C_stop();
}

// write sA to register sB.
// device address is already in I2C_BUFFER(2)
// Z is set on total success
void I2C_reg_write() {
  // we use address 0 and up as a buffer
  store(I2C_BUFFER(1), sB);
  store(I2C_BUFFER(0), sA);
  sA = I2C_BUFFER(2);
  I2C_write_bytes();
}
