#ifndef PB_AXI_H
#define PB_AXI_H

// Note: AXI_BASE and sSpare have to be defined before
// using these functions.

// AXI registers
#define AXI_CONTROL 0x0
#define AXI_DATA_0 0x8
#define AXI_ADDRESS_0 0xC

// These are all inline functions.

#define AXI_REG(reg) (AXI_BASE + reg)
// Determine if we can use the constant-optimized ports.
#if AXI_BASE==0
#define axi_outputk(reg, val) outputk(AXI_REG(reg),val)
#else
#define axi_outputk(reg, val) \
  sSpare = val;               \
  output(AXI_REG(reg),sSpare)
#endif
// Input a value from the AXI interface registers
#define axi_input(reg, inreg) input(AXI_REG(reg),inreg)
// Output a value to the AXI interface registers
#define axi_output(reg, outreg) output(AXI_REG(reg),outreg)

// Reset AXI data registers.
#define axi_reset_data() axi_outputk(AXI_CONTROL, 0x80)
// Reset AXI address registers.
#define axi_reset_addr() axi_outputk(AXI_CONTROL, 0x40)

// Execute an AXI read.
#define do_axi_read()				       \
  axi_outputk(AXI_CONTROL, 0x01);                      \
  do {						       \
    axi_input(AXI_CONTROL, sSpare);			       \
  } while (sSpare |^ 0x8)

// Execute an AXI write.
#define do_axi_write()				\
  axi_outputk(AXI_CONTROL, 0x03);               \
  do {						\
    axi_input(AXI_CONTROL, sSpare);			\
  } while (sSpare |^ 0x8)


// Read from a constant AXI address.
#define axi_read_constaddr(address)                     \
  axi_outputk(AXI_ADDRESS_0, address & 0xFF);		\
  axi_outputk(AXI_ADDRESS_0+1, (address >> 8) & 0xFF);	\
  do_axi_read()

// Write from a constant AXI address.
#define axi_write_constaddr(address)                    \
  axi_outputk(AXI_ADDRESS_0, address & 0xFF);           \
  axi_outputk(AXI_ADDRESS_0+1, (address >> 8) & 0xFF);  \
  do_axi_write()

// Write a constant byte to a constant address
#define axi_writek_byte(address, data)		       \
  axi_outputk(AXI_DATA_0, data);		       \
  axi_outputk(AXI_ADDRESS_0, address & 0xFF);	       \
  axi_outputk(AXI_ADDRESS_0+1, (address >> 8) & 0xFF); \
  do_axi_write()

// Write a constant short to a constant address
#define axi_writek_short(address, data)		       \
  axi_outputk(AXI_DATA_0, data & 0xFF);		       \
  axi_outputk(AXI_DATA_0+1, (data >> 8) & 0xFF);       \
  axi_outputk(AXI_ADDRESS_0, address & 0xFF);	       \
  axi_outputk(AXI_ADDRESS_0+1, (address >> 8) & 0xFF); \
  do_axi_write()

// Write a constant long to a constant address
#define axi_writek_long(address, data)		       \
  axi_outputk(AXI_DATA_0, data & 0xFF);		       \
  axi_outputk(AXI_DATA_0+1, (data >> 8) & 0xFF);       \
  axi_outputk(AXI_DATA_0+2, (data >> 16) & 0xFF);      \
  axi_outputk(AXI_DATA_0+3, (data >> 24) & 0xFF);      \
  axi_outputk(AXI_ADDRESS_0, address & 0xFF);	       \
  axi_outputk(AXI_ADDRESS_0+1, (address >> 8) & 0xFF); \
  do_axi_write()

// Read in a given AXI address byte
#define get_axi_addrbyte(byte_num, target)	\
  axi_input(AXI_ADDRESS_0+byte_num, target)

// Set a given AXI address byte.
#define set_axi_addrbyte(byte_num, target)	\
  axi_output(AXI_ADDRESS_0+byte_num, target)

// Set a given AXI address byte to a constant value.
#define kset_axi_addrbyte(byte_num, constant)	\
  axi_outputk(AXI_ADDRESS_0+byte_num, constant)

// Get a given AXI data byte
#define get_axi_byte(byte_num, target)		\
  axi_input(AXI_DATA_0+byte_num, target)

// Set a given AXI data byte
#define set_axi_byte(byte_num, target)		\
  axi_output(AXI_DATA_0+byte_num, target)

// Set a given AXI data byte to a constant value.
#define kset_axi_byte(byte_num, constant)       \
  axi_outputk(AXI_DATA_0+byte_num, constant)

#endif
