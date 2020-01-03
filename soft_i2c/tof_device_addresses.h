#ifndef TOF_DEVICE_ADDRESSES_H
#define TOF_DEVICE_ADDRESSES_H

// these are all 8-bit addresses

#define PMON_ADDR 0x30
#define PMON_ADDR_RD 0x31
#define TEMP_ADDR 0x90
#define TEMP_ADDR_RD 0x91
#define ADC_ADDR    0x28
#define ADC_ADDR_RD 0x29

// NOTE: DAC_ADDR_1/DAC_ADDR_2 are programmatically
// determined (they're +2/+4 from DAC_ADDR_0)
#define DAC_ADDR_0 0x62
#define DAC_ADDR_3 0x80
#define DAC_ADDR_VSLOPE 0x20

// NOTE: SWITCH1/2/3_ADDR are programmatically
// determined (they're +2/+4/+6 from SWITCH0_ADDR)
#define SWITCH0_ADDR 0x98

#define GP0_ADDR 0x40
#define GP1_ADDR 0x42

#endif
