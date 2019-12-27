#include "kcpsm6.h"

#define sSpare sF

#include "tof_devices.h"
#include "pb_stack.h"

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
      // sC.sB is the value
      if (sC & 0x80) {
	// yes
	sA = s1;
	dac_write();
      }
    } while (s1 != 65);
    output( CFG_ADDR, s1);
    input( CFG_DATA, sA);   // lo byte I2C0
    input( CFG_DATA+1, sB); // hi byte I2C1
    if (sB & 0x80) {
      // yes, do bus 0
      KSET_I2C_BUS(0);
      sA |= 0x3;
      gpio1_write_output();
      KSET_I2C_BUS(1);
      sB <<= 1;
      gpio1_write_output();
    }
    s0 |= (UPDATECOMPLETE | READY);
    output(READYINIT, s0);
  }
}

//
// The monitoring path is a pain in the neck,
// because the monitoring ADC is incredibly
// slow and we need to switch between things
// periodically.
//
#define MONITOR_COUNTER 0x8
#define MONITOR_CHANNEL 0xC

#define MONITOR_STATE 0xD
// call monitor_reset and move immediate to ST_SYNC
#define ST_RESET 0
// call monitor_sync_check on both I2C busses until both acknowledge
#define ST_SYNC 1
// extended path: we start with XSELn high, so
// we do channel 0/8/16/24 first (channels 65-73). then we lower XSEL.
#define ST_EXTEND 2
// normal path, go through channels 0-64
#define ST_NORMAL 3
// finish up
#define ST_FINISH 4

// each cycle is ~100 ns
// for a ~17 ms cycle that's
// a monitor rate of 664*256
// (169,984) cycles.
void monitor_tick() {
  fetch(MONITOR_COUNTER, sA);
  fetch(MONITOR_COUNTER+1, sB);
  fetch(MONITOR_COUNTER+2, sC);
  psm("sub %1, 1", sA);
  psm("subcy %1, 1", sB);
  psm("subcy %1, 1", sC);
  if (Z) {
    sA = 0;
    input(MONITOR_RATE, sB);
    input(MONITOR_RATE, sC);
  }
  store(MONITOR_COUNTER, sA);
  store(MONITOR_COUNTER+1, sB);
  store(MONITOR_COUNTER+2, sC);
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
  I2C_start();
  sA = ADC_ADDR_RD;
  I2C_Tx_byte_and_Rx_ACK();
  // this should never happen, but if it does,
  // nonsense data gets updated.
  psm("jump C, monitor_operation_fail");
  I2C_Rx_byte();
  push1();
  I2C_Tx_ACK();
  I2C_Rx_byte();
  I2C_Tx_NACK();
  I2C_stop();
  sB = sA;
  pop1();
  return;
 monitor_operation_fail:
  I2C_stop();
  sB.sA = 0xFFFF;
}

void monitor_reset() {
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
