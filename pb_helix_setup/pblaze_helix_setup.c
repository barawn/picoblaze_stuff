#include "kcpsm6.h"

// This is not really C: it's run through a C preprocessor, so
// all C preprocessor tricks work. But it's really assembly.

// The C-preprocessor is stupidly capable, though, so abuse it.

#define AXI_BASE 0x00
#define IIC_BASE           0x1000
#define PWM_BASE           0x2000
#define sSpare sA

#include "pb_axi.h"
#include "pb_axi_iic.h"
#include "pb_delay.h"

void init(void) {
  // Check if we completed initialization before.
  input(0x80, &s0);
  if (s0 |^ 0x2) {    
    // Read first address of configuration BRAM, tells us final address
    axi_read_constaddr(0x0000);
    // Max is 1024, read two bytes. Store as a double register pair as s0.s
    get_axi_byte(0, &s1);
    get_axi_byte(1, &s0);
    // Byte align s1.
    s1 &= 0xFC;
    // Check to make sure s0 isn't out of bounds (if any high bits are set, it is)
    if (s0 & 0xF0) return;
    // OK, so s0.s1 is now an int-aligned address between 0x0000 and 0x0FFC.
    
    s2.s3 = 0x0;  // s2.s3 is the current BRAM pointer
    s4 = 0;  // s4 is the current bank
    // set the current bank (register 1)
    I2C_WRITEREG( 0xD0, 0x1, 0x0 );
    // sleep for a while for testing
    DELAY1US(1);
    // BRAM stores data as
    // [31:24] delay counter
    // [23:16] bank
    // [15:08] register
    // [07:00] value
    do {
      // Increment the BRAM pointer
      s2.s3 += 0x4;
      // read from 0x0000 + s1
      set_axi_addrbyte(0, s3);
      set_axi_addrbyte(1, s2);
      do_axi_read();
      DELAY1US(1);
      // Check delay
      get_axi_byte(3, sA);
      // Delay is in 3 ms increments, passed in via sA.
      DELAY3MS(sA);
      // Get everything. First byte is value...
      get_axi_byte(0, s5);
      // Next byte is register.
      get_axi_byte(1, s6);
      // Second byte is bank.
      // Remember functions want parameters in sA/sB/sC,
      // and if they're already there, the macro skips the
      // assign before call. So pull the bank into sC
      // to save an assign.
      get_axi_byte(2, sC);
      // Is the bank the same as the old bank?
      if (sC != s4) {
	// No, so we need to update it (page register is register 1 in every bank)
	I2C_WRITEREG( 0xD0, 0x1, sC);
	s4 = sC;
      }
      // Now update the register.
      I2C_WRITEREG(0xD0, s6, s5);
      DELAY1US(1);
    } while (s2.s3 != s0.s1);
    // done initializing clock from BRAM register list
  }
  s0 = 2;
  output(0x80, s0);
}

void loop(void) {
  
}

