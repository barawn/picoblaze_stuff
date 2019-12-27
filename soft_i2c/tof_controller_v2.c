#include "kcpsm6.h"

//
// PicoBlaze TOF code.
// note: grep all C/H for OPTIMIZE to find any
// known places where code can be saved

#define sSpare sF

#include "tof_devices.h"
#include "pb_stack.h"

#define CFG_DATA                     0x20
#define CFG_ADDR                     0x40
#define MON_DATA                     0x60
#define MON_ADDR                     0x80
#define MONITOR_RATE                 0xA0
#define READYINIT                    0xC0

#define READY                        0x80
#define INIT                         0x40
#define INITDONE                     0x10
#define UPDATECOMPLETE               0x04
#define UPDATING                     0x02
#define UPDATEREQ                    0x01

void init() {
  stack_init();
  KSET_I2C_BUS(0);
  gpio_init();
  KSET_I2C_BUS(1);
  gpio_init();
  KSET_I2C_BUS(0);
  // read input register on GPIO1
  gpio1_read_input();
  s0 = sA;
  if (s0 & 0x3) {
    if (s0 & 0x1) {
      s1 = INIT;
      output( READYINIT, s1 );
      do {
	input(READYINIT, s1);
      } while (s1 |^ INITDONE);
      s1 = 0;
      output(READYINIT, s1);
      s0 ^= 0x1;
      sA = s0;
      gpio1_write_output();
    }
    if (s0 & 0x2) {
      adc_init();
      s1 = READY;
      output(READYINIT, s1);
      s0 ^= 0x2;
      sA = s0;
      gpio1_write_output();
    }
  }
  monitor_init();
}

void loop() {
  input( READYINIT, s0);
  if (s0 & UPDATEREQ) {
    s0 |= UPDATING;
    output( READYINIT, s0);
    s1 = 0;
    do {
      output( CFG_ADDR, s1 );
      input( CFG_DATA, sB );
      input( CFG_DATA+1, sC );
      // sC.sB is the value,
      // it'll be shifted out of oblivion
      if (sC & 0x80) {
	// yes
	sA = s1;
	dac_write();
      }
    } while (s1 != 64);
    output( CFG_ADDR, s1);
    input( CFG_DATA, sA);   // lo byte[7:3] I2C0
    input( CFG_DATA+1, sB); // hi byte[6:3] I2C1
    // shift up sB. they're downshifted by 1 to preserve bit 15 as an update bit
    sB <<= 1;
    // if C, then bit 15 was set
    if (C != 0) {
      KSET_I2C_BUS(0);
      // preserve ready/init bits
      sA |= 0x3;
      gpio1_write_output();
      KSET_I2C_BUS(1);
      sA = sB;
      gpio1_write_output();
    }
    s0 |= (UPDATECOMPLETE | READY);
    output(READYINIT, s0);
  }
  monitor();
}

//
// The monitoring path is a pain in the neck,
// because the monitoring ADC is incredibly
// slow and we need to switch between things
// periodically.
//
// We use the second regbank when we're in
// the monitor path, to allow us to reuse registers.
//

#define MONITOR_COUNT_0 s0
#define MONITOR_COUNT_1 s1
#define MONITOR_COUNT_2 s2
#define MONITOR_CHANNEL s3
#define MONITOR_STATE   s4

// call monitor_reset and move immediate to ST_SYNC
#define ST_RESET 0
// call monitor_sync_check on both I2C busses until both acknowledge
#define ST_SYNC_0 1
#define ST_SYNC_1 2
// extended path: we start with XSELn high, so
// we do channel 0/8/16/24 first. then we lower XSEL.
// the 'extended channels' are channels 64/65, 72/73, 80/81, 88/89.
#define ST_EXTEND 3
// normal path, go through channels 0-64
#define ST_NORMAL 4
// finish, these are the quick reads
// (channels 96+).
#define ST_TEMP_0 5
#define ST_TEMP_1 6
#define ST_PMON 7

void monitor_init() {
  psm("regbank B");
  MONITOR_COUNT_0 = 0;
  input(MONITOR_RATE, MONITOR_COUNT_1);
  input(MONITOR_RATE+1, MONITOR_COUNT_2);
  MONITOR_STATE = ST_RESET;
  MONITOR_CHANNEL = 0;
  psm("regbank A");
}

void monitor() {
  psm("regbank B");
  if (MONITOR_STATE == ST_RESET) {
    monitor_reset();
    MONITOR_STATE = ST_SYNC_0;
  }
  if (MONITOR_STATE < ST_EXTEND) {
    sA = MONITOR_STATE;
    sA -= 1;
    SET_I2C_BUS(sA);
    monitor_sync_check();
    psm("jump Z, monitor_finish");
    MONITOR_STATE += 1;
  }
  if (MONITOR_STATE == ST_EXTEND) {
    monitor_tick();
    psm("jump NZ, monitor_finish");
    // oookay need to do something

    // set up our NEXT read
    sA = MONITOR_CHANNEL;
    sA += 8;
    // do ops on both busses
    s5 = 0;
    monitor_extend_read();
    s5 = 1;
    monitor_extend_read();
    // now increment what channel we're on
    MONITOR_CHANNEL += 8;
    // are we past the end of the extended set?
    if (MONITOR_CHANNEL == 32) {
      // yes, go to normal
      MONITOR_CHANNEL = 0;
      MONITOR_STATE = ST_NORMAL;
    }
    psm("jump monitor_finish");
  }
  if (MONITOR_STATE == ST_NORMAL) {
    monitor_tick();
    psm("jump NZ, monitor_finish");
    // oookay need to do something
    sA = MONITOR_CHANNEL;
    sA += 1;
    // ops on both busses
    s5 = 0;
    monitor_normal_read();
    // MASSIVE SLEAZE since none of the higher bits are preserved
    s5 = 33;
    monitor_normal_read();
    // now increment what channel we're on
    MONITOR_CHANNEL += 1;
    // are we past the end of the normal set?
    if (MONITOR_CHANNEL == 32) {
      // yes go to finish state
      MONITOR_CHANNEL = 0;
      MONITOR_STATE = ST_TEMP_0;
    }
    psm("jump monitor_finish");
  }
  if (MONITOR_STATE < ST_PMON) {
    monitor_temp_read();
    MONITOR_STATE += 1;
    if (MONITOR_STATE == ST_PMON) {
      // still on I2C BUS 1
      sB = 0xD3;
      sA = 0x1;
      sSpare = 0x30;
      store(I2C_BUFFER(2), sSpare);
      I2C_reg_write();
    }
    psm("jump_monitor_finish");
  }
  // ST_PMON
  monitor_tick();
  psm("jump NZ, monitor_finish");

  
  
monitor_finish:
  psm("regbank A");  
}

#define TEMP_ADDR 0x90
#define TEMP_ADDR_RD 0x91
#define TEMP_CHANNEL_0 96
void monitor_temp_read() {
  sSpare = MONITOR_STATE;
  sSpare -= ST_TEMP_0;
  SET_I2C_BUS(sSpare);
  // sigh, set pointer
  I2C_start();
  sSpare = TEMP_ADDR;
  store(I2C_BUFFER(1), sSpare);
  sSpare = 0;
  store(I2C_BUFFER(0), sSpare);
  sC = I2C_BUFFER(1);
  I2C_write_bytes_process();
  sA = TEMP_ADDR_RD;
  I2C_two_byte_read();
  s5 = MONITOR_STATE;
  s5 += (TEMP_CHANNEL_0 - ST_TEMP_0);
  monitor_output();
}

void toggle_xsel() {
  sA = 0xFF;
  if (MONITOR_STATE == ST_EXTEND) {
    sA ^= 0x10;
  }
  gpio0_write_output();
}
    
  
void monitor_extend_read() {
  SET_I2C_BUS(s5);
  if (sA == 32) {
    toggle_xsel();
    sA = 0;
  }
  monitor_operation();
  // bus 1 guys are 1 above 
  s5 += MONITOR_CHANNEL;
  s5 |= 64;
  monitor_output();
}

void monitor_normal_read() {
  SET_I2C_BUS(s5);
  if (sA == 32) {
    toggle_xsel();
    sA = 0;
  }
  monitor_operation();
  // massive sleaze: SET_I2C_BUS
  // doesn't use any bits above 1
  // so we use 33 (10_0001) for bus 1
  // and then mask off the remainder
  // so that all the bus 1 guys are channels 32-63
  s5 &= 0xFE;
  s5 += MONITOR_CHANNEL;
  monitor_output();
}

void monitor_output() {
  output(MON_ADDR, s5);
  output(MON_DATA, sA);
  output(MON_DATA+1, sB);
}

// each cycle is ~100 ns
// for a ~17 ms cycle that's
// a monitor rate of 664*256
// (169,984) cycles.
void monitor_tick() {
  psm("sub %1, 1", MONITOR_COUNT_0);
  psm("subcy %1, 1", MONITOR_COUNT_1);
  psm("subcy %1, 1", MONITOR_COUNT_2);
  if ( Z != 0 ) {
    MONITOR_COUNT_0 = 0;
    input(MONITOR_RATE, MONITOR_COUNT_1);
    input(MONITOR_RATE+1, MONITOR_COUNT_2);
  }
  // Z should be preserved here
}

void I2C_two_byte_read() {
  I2C_start();
  I2C_Tx_byte_and_Rx_ACK();
  // this should never happen, but if it does,
  // nonsense data gets updated.
  psm("jump C, I2C_two_byte_read_fail");
  I2C_Rx_byte();
  push1();
  I2C_Tx_ACK();
  I2C_Rx_byte();
  I2C_Tx_NACK();
  I2C_stop();
  sB = sA;
  pop1();
  return;
 I2C_two_byte_read_fail:
  I2C_stop();
  // OPTIMIZE 1
  // use load&return
  sB.sA = 0xFFFF;  
}

#define ADC_ADDR    0x28
#define ADC_ADDR_RD 0x29

void monitor_operation() {
  // Monitor cycle is:
  // Change switch to value in sA.
  // Read ADC (which is from the LAST channel) and
  // return its result in sB.sA.
  // we do this because as soon as we read the ADC
  // it starts converting again.
  switch_write();
  sA = ADC_ADDR_RD;
  I2C_two_byte_read();
}

void monitor_reset() {
  KSET_I2C_BUS(0);
  sA = 0;
  switch_write();
  KSET_I2C_BUS(1);
  sA = 0;
  switch_write();
}

void monitor_sync_check() {
  sA = 0;
  store(I2C_BUFFER(1), sA);
  sA = ADC_ADDR;
  store(I2C_BUFFER(0), sA);
  sA = I2C_BUFFER(1);
  I2C_write_bytes();
  // set zero if success
  psm("add %1, 0", sA);
}

#define ADC_WRITE( addr, data ) \
  sA = addr;                    \
  sB = data;                    \
  adc_write_fn()

void adc_init() {
  s8 = 0;
  do {
    SET_I2C_BUS(s8);
    // this setup takes load/load/call x 5 = 15 instructions
    // unlikely that a call table would help
    // a loop might tho
    ADC_WRITE( 0x00 , 0x00 );
    ADC_WRITE( 0x01 , 0x00 );
    ADC_WRITE( 0x02 , 0x00 );
    ADC_WRITE( 0x03 , 0x02 );
    ADC_WRITE( 0x04 , 0x00 );
    s8 += 1;
  } while (s8 < 2);
}

void adc_write_fn() {
  push2();
  spi_begin();
  pop1();
  spi_write_byte();
  pop1();
  spi_write_byte();
  spi_stop();
}
