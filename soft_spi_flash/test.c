#include "kcpsm6.h"

#include "soft_spi_flash.h"

#define RDID_BYTE_0 0x20
#define RDID_BYTE_1 0xBA
#define RDID_BYTE_2 0x18

#define LED_PORT 0x10

void init() {
  SPI_STARTUP_initialize();
  // Soo... we don't actually need to wait at all here,
  // because obviously the FPGA booted.
  s8 = 0;
  SPI_Flash_read_ID();
  psm("compare %1, %2", sC, 0x20);
  psm("comparecy %1, %2", sB, 0xBA);
  psm("comparecy %1, %2", sA, 0x18);
  if (Z != 0) {
    s8 = 1;
  }
}

void loop() {
  // flag an error condition
  if (s8 != 0) {
    // long blink first
    outputk(LED_PORT, 1);
    // 1 second
    sA = 152;
    bigdelay();
    outputk(LED_PORT, 0);
    sA = 38;
    bigdelay();
    // now blink for each
    do {
      outputk(LED_PORT, 1);
      sA = 38;
      bigdelay();
      outputk(LED_PORT, 0);
      sA = 38;
      bigdelay();
      s8 -= 1;
    } while (Z != 0);
  }
}

// we're running at 80 MHz
// a 250 millisecond delay is
// 20M cycles.
// each loop takes 4 instructions = 8 cycles
// 38*65536*8 = 19.92M cycles.
void bigdelay() {
  s0 = 0;
  s1 = 0;
  s2 = sA;
  // sub/sub/sub/jump Z
  do {
    s0 -= 1;
    psm("subcy %1, %2", s1, 0);
    psm("subcy %1, %2", s2, 0);    
  } while (Z == 0);
}
