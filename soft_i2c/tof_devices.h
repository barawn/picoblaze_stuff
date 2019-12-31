#include "soft_i2c_user.h"
#include "tof_device_addresses.h"
#include "tof_gpio.h"

void gpio_init() {
  // drive outputs
  sB = 3;             // register 3 (configuration)
  sA = 0x1;           // all but bit 0
  gpio0_write();      // write
  // might need to configure the power enables off
  // first, but they're pulled up.
  sB = 3;             // register 3
  sA = 0x0;           // all
  gpio1_write();      // write
}

void spi_begin() {
  // lower all chip selects
  sA = 0x00;
  gpio0_write_output();
}

void spi_stop() {
  // raise all chip selects
  sA = 0xFF;
  gpio0_write_output();
}

#define SPI_SCLK   0x2
#define SPI_MOSI   0x4
#define SPI_MISO   0x1

// write sA through SPI in I2C GPIO
void spi_write_byte() {
  // sA/sB/sC get clobbered
  // we need 2 registers (sD/sE)
  sD = sA;
  // Use bit mask for the counter loop.
  sE = 0x80;
  do {    
    // sigh, SDI is on bit 2 not 1.
    // could do this programmatic but
    // the two extra shifts make it
    // same cost, and this is more
    // readable.
    if (sD & sE) {
      // data high
      sA = SPI_MOSI;
    } else {
      // data low
      sA = 0x0;
    }
    // save sA
    push1();
    gpio0_write_output();
    // restore sA
    pop1();
    sA ^= SPI_SCLK;
    gpio0_write_output();
    sE >>= 1;
    // when sE is 0x1, the downshift makes carry set
  } while (!C);
}

// no device at address 0x68, it's 0x80
#define DAC_ADDR_INVALID 0x68

// Converts the DAC address in sA
// into chip address (sA) and channel address (sB)
// and also sets the appropriate I2C bus
void dac_convert_address() {
  // Channel 64 is the VSLOPE ADC automatically.
  if (sA == 64) {
    KSET_I2C_BUS(0);
    // OPTIMIZE 1
    // use load&return
    sA = DAC_ADDR_VSLOPE;
    return;
  }
  // Now we need to group by 8, so peel
  // off the low 3 bits into sB,
  // and downshift sA
  sB = sA;
  sB &= 0x7;
  sA >>= 3;
  // Now if sA is 4,5,6,7, it's on I2C bus 1
  // sleaze it, it's cheaper
  psm("test %1, 0x4", sA);
  psm("sla %1", sSpare);
  SET_I2C_BUS(sSpare);

  // only want bottom 3 bits
  sA &= 0x3;
  // now we have sA = 0,1,2,3
  // multiply it by 2 (0, 2, 4, 6)
  sA <<= 1;
  // add it to 62 (62, 64, 66, 68)
  sA += DAC_ADDR_0;
  // and if's equal to 0x68, switch it
  // magic can save an instruction here
  // w/compare, ret NZ, load&return
  // OPTIMIZE 1
  // use load&return
  if (sA == DAC_ADDR_INVALID) {
    sA = DAC_ADDR_3;
  }
  // Done!
}

// writes (sC.sB) into DAC at addr sA
// this takes 4 writes
// address control upper lower
void dac_write() {
  // goddamn it Linear you suck
  sC.sB <<= 4;
  store(I2C_BUFFER(1), sC);
  store(I2C_BUFFER(0), sB);
  // convert address in sA:
  // set I2C bus
  // put address in sA
  // put channel in sB
  dac_convert_address();
  // set control bits in sB
  sB |= 0x30;
  store(I2C_BUFFER(3), sA);
  store(I2C_BUFFER(2), sB);
  I2C_write_bytes();
  // do something on not Z
  // if (!Z) {
  //    do_something();
  // }
}

// each bus has 32 channels
// which can be selected
// (plus an additional 4
// via wacko crap, but they
// don't count)
// these go from
// 4c/4d/4e/4f
// 98/9A/9C/A0
// so channel 8 needs to map
// to "9A".

void switch_write() {
  // copy sA into sB
  sB = sA;
  // only grab low 3 bits (0-7)
  sB &= 0x7;
  // mask off 
  sA &= 0xF8;
  sA >>= 2;
  sA += SWITCH0_ADDR;
  sC = 0x1;
  while (sB != 0) {
    sC <<= 1;
  }
  store(I2C_BUFFER(1), sA);
  store(I2C_BUFFER(0), sC);
  sA = I2C_BUFFER(1);
  I2C_write_bytes();
}

