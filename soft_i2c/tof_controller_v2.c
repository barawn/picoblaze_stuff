#include "kcpsm6.h"

//
// PicoBlaze TOF code.
// note: grep all C/H for OPTIMIZE to find any
// known places where code can be saved

#define sSpare sF

#include "tof_device_addresses.h"
#include "tof_devices.h"
#include "pb_stack.h"

/*
 * Config addresses:
 * 0-7:   VTHTRIGA0,VTHTRIGA1,VSETA1,VSETA0,VSETA2,VSETA3,VTHTIMA,VHYSTA
 * 8-15:  VTHTRIGB0,VTHTRIGB1,VSETB1,VSETB0,VSETB2,VSETB3,VTHTIMB,VHYSTB
 * 16-23: VTHTRIGC0,VTHTRIGC1,VSETC1,VSETC0,VSETC2,VSETC3,VTHTIMC,VHYSTC
 * 24-31: VTHTRIGD0,VTHTRIGD1,VSETD1,VSETD0,VSETD2,VSETD3,VTHTIMD,VHYSTD
 * 32-39: VTHTRIGE0,VTHTRIGE1,VSETE1,VSETE0,VSETE2,VSETE3,VTHTIME,VHYSTE
 * 40-47: VTHTRIGF0,VTHTRIGF1,VSETF1,VSETF0,VSETF2,VSETF3,VTHTIMF,VHYSTF
 * 48-55: VTHTRIGG0,VTHTRIGG1,VSETG1,VSETG0,VSETG2,VSETG3,VTHTIMG,VHYSTG
 * 56-63: VTHTRIGH0,VTHTRIGH1,VSETH1,VSETH0,VSETH2,VSETH3,VTHTIMH,VHYSTH
 * 64: VSLOPE
 * 65: ENABLES ( [14] \FEEPENE, [13] \FEEPENF, [12] \FEEPENG, [11] \FEEPENH,
 *                [7] \FEEPENA, [6]  \FEEPENB, [5]  \FEEPENC,  [4] \FEEPENB,
 *                [3] \VP75EN )
 *
 * Monitor addresses:
 * 0-7:   RMONA2, VCOMPA, RMONA1, RMONA0, IMONA3, IMONA2, IMONA1, IMONA0
 * 8-15:  RMONB2, VCOMPB, RMONB1, RMONB0, IMONB3, IMONB2, IMONB1, IMONB0
 * 16-23: RMONC2, VCOMPC, RMONC1, RMONC0, IMONC3, IMONC2, IMONC1, IMONC0
 * 24-31: RMOND2, VCOMPD, RMOND1, RMOND0, IMOND3, IMOND2, IMOND1, IMOND0
 * 32-39: RMONE2, VCOMPE, RMONE1, RMONE0, IMONE3, IMONE2, IMONE1, IMONE0
 * 40-47: RMONF2, VCOMPF, RMONF1, RMONF0, IMONF3, IMONF2, IMONF1, IMONF0
 * 48-55: RMONG2, VCOMPG, RMONG1, RMONG0, IMONG3, IMONG2, IMONG1, IMONG0
 * 56-63: RMONH2, VCOMPH, RMONG1, RMONG0, IMONG3, IMONG2, IMONG1, IMONG0
 * 64: RMONA3
 * 65: RMONE3
 * 72: RMONB3
 * 73: RMONF3
 * 80: RMONC3
 * 81: RMONG3
 * 88: RMOND3
 * 89: RMONH3
 * 96: TEMP0
 * 97: TEMP1
 * 98: VIN
 * 101: VOUT
 * 102: IOUT
 */

#define CFG_DATA                     0x20
#define CFG_ADDR                     0x40
#define MON_DATA                     0x60
#define MON_ADDR                     0x80
#define MONITOR_RATE                 0xA0
#define READYINIT                    0xC0
#define MONSTATUS                    0xE0

#define READY                        0x80
#define INIT                         0x40
#define INITDONE                     0x10
#define UPDATECOMPLETE               0x04
#define UPDATING                     0x02
#define UPDATEREQ                    0x01

#define MONCYCLETOGGLE               0x80

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
    } while (s1 != 65);
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
#define ST_PMON_0 7
#define ST_PMON_1 8
#define ST_PMON_2 9
#define ST_FINISH 10

#define PMON_CONTROL 0xD3
#define PMON_VIN 0x88
#define PMON_VOUT 0x8B
// PMON_IOUT is determined programmatically, it's 0x8C

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
  if (MONITOR_STATE < ST_PMON_0) {
    monitor_temp_read();
    MONITOR_STATE += 1;
    if (MONITOR_STATE == ST_PMON_0) {
      // still on I2C BUS 1
      sB = PMON_CONTROL;
      sA = 0x1;
      sSpare = PMON_ADDR;
      store(I2C_BUFFER(2), sSpare);
      I2C_reg_write();
    }
    psm("jump monitor_finish");
  }
  if (MONITOR_STATE == ST_PMON_0) {
    monitor_tick();
    psm("jump NZ, monitor_finish");
  }
  monitor_pmon_read();
  MONITOR_STATE += 1;
  if (MONITOR_STATE == ST_FINISH) {
    MONITOR_STATE = ST_EXTEND;
    MONITOR_CHANNEL = 0;
    // maybe do something to flag
    // a completed cycle
    sSpare = ( MONCYCLETOGGLE | ST_EXTEND );
    output(MONSTATUS, sSpare);
  }
 monitor_finish:
  // let the world know what we're doing
  output(MONSTATUS, MONITOR_STATE);
  psm("regbank A");  
}

// we read out VIN/VOUT/IOUT
// these are 88/8B/8C
// and are stored in 98/101/102 
void monitor_pmon_read() {
  sSpare = PMON_ADDR;
  store(I2C_BUFFER(1), sSpare);
  // construct the channel from the monitor state
  s5 = MONITOR_STATE;
  s5 -= ST_PMON_0;
  // s5 = 0, 1, or 2
  if (Z != 0) {
    s5 += 2;
  }
  // s5 = 0, 3, 4
  s5 += 98;
  // s5 = 98, 101, 102
  sA = s5;
  // sA = 88, 8B, 8C
  sA += 38;
  I2C_reg_read16();  
  monitor_output();
}

// 96/97 are the two temperature channels
#define TEMP_CHANNEL_0 96
void monitor_temp_read() {
  sSpare = MONITOR_STATE;
  sSpare -= ST_TEMP_0;
  // 0 or 1
  SET_I2C_BUS(sSpare);
  sSpare = TEMP_ADDR;
  store(I2C_BUFFER(1), sSpare);
  // read register 0
  sA = 0;
  I2C_reg_read16();
  s5 = MONITOR_STATE;
  s5 += (TEMP_CHANNEL_0 - ST_TEMP_0);
  // make s5 96/97
  monitor_output();
}

// clear XSEL in ST_EXTEND, otherwise
// set it. Leave everything else high.
void toggle_xsel() {
  sA = 0xFF;
  if (MONITOR_STATE == ST_EXTEND) {
    sA ^= 0x10;
  }
  gpio0_write_output();
}

// read extended monitor
// from bus s5
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

// this is now a nested function inside I2C_reg_read16
/*
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
*/

void monitor_operation() {
  // Monitor cycle is:
  // Change switch to value in sA.
  // Read ADC (which is from the LAST channel) and
  // return its result in sB.sA.
  // we do this because as soon as we read the ADC
  // it starts converting again.
  switch_write();
  sA = ADC_ADDR_RD;  
  I2C_read_two_bytes();
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
