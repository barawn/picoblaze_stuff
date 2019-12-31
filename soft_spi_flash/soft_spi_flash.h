#ifndef SOFT_SPI_FLASH_H
#define SOFT_SPI_FLASH_H

#include "soft_spi.h"

// rework of SPI flash
// routines from Ken Chapman in
// pseudo-c.

// Again, standard convention is that
// parameters are sA/sB/sC/sD/sE,
// and sF is a completely spare
// register.

// commands
#define SPI_FLASH_RDID 0x9F
#define SPI_FLASH_PP 0x2
#define SPI_FLASH_WREN 0x06
#define SPI_FLASH_WRDI 0x04
#define SPI_FLASH_SE 0xD8

// bits
#define SPI_FLASH_WIP 0x01

// SPI_Flash_read_ID: Executes read ID instruction
//     input: none
//     output: sC/sB/sA data from 'RDID' (0x9F)
void SPI_Flash_read_ID() {
  SPI_DISABLE();
  sA = 0x9F;
  // transmit sA and receive the byte after that.
  SPI_tx_rx_twice();
  // store sA in 0x3F
  push1();
  SPI_tx_rx();
  // store sA in 0x3E
  push1();
  SPI_tx_rx();
  // store sA in 0x3D
  push1();
  // put 0x3D in sA
  // put 0x3E in sB
  // put 0x3F in sC
  pop3();
  SPI_DISABLE();
}

// SPI_Flash_tx_address: Transmits the three bytes from the stack.
//     input: none
//     output: none
//     uses: sA/sB (implicitly)
//
// Also creates SPI_Flash_tx_stack2 and SPI_Flash_tx_stack.
// The nested call this way uses 5 instructions total,
// as opposed to the 7 instructions otherwise (3x pop/tx, return)
void SPI_Flash_tx_address() {
  // pop, then transmit
  SPI_Flash_tx_stack();
  // returns here...
 SPI_Flash_tx_stack2:
  // pop, then transmit
  SPI_Flash_tx_stack();
  // returns here...
 SPI_Flash_tx_stack:
  // pop
  pop1();
  // transmit
  SPI_tx_rx();
}

// SPI_Flash_read_byte: Executes read instruction
//     input: sA/sB/sC : address to read (sA is most-significant-byte)
void SPI_Flash_read_byte() {
  // Stacks are last in first out, which is why we
  // ask for the address in inverse order.
  // That way we can push3 straight from the beginning
  // and pop1/pop1/pop1.
  SPI_DISABLE();  
  push3();
  sA = 0x3;
  SPI_tx_rx();
  SPI_Flash_tx_address();
  // get data
  SPI_tx_rx();
  // done
  SPI_DISABLE();
}

// SPI Flash writing:
//    Write procedure involves multiple calls,
//    to allow for multiple bytes to be sent.
//  1: SPI_Flash_write_begin() (with address in sA/sB/sC)
//  2: sA = byte_to_write; SPI_tx_rx(); (for each byte to write)
//  3: SPI_Flash_write_complete()
//
void SPI_Flash_write_begin() {
  // save address
  push3();
  SPI_Flash_set_WREN();
  sA = SPI_FLASH_PP;
  SPI_tx_rx();
  SPI_Flash_tx_address();
  // and done. we're just the beginning
}

void SPI_Flash_write_complete() {
  // end last command
  SPI_DISABLE();
  // wait for WIP done
  SPI_Flash_wait_WIP();
  // clear WREN
  SPI_Flash_reset_WREN();
}

// We use this in 2 places, so it's equivalent
// to have it in a function: but it also isn't
// a bad idea to go ahead and call it at the
// beginning if RDID worked. So leave it a function.
// call test jump return + call call   vs
// call test jump + call test jump
void SPI_Flash_wait_WIP() {
  do {
    SPI_Flash_read_SR();
  } while (sA & SPI_FLASH_WIP);  
}
			  
// Nest all of these, just to save a few returns.
// If there were an easy way for the user functions
// to do this, I'd do it there too, but I don't
// have a way of saying "don't use a return".
// I should add an attribute for that maybe.
void SPI_Flash_set_WREN() {
  SPI_DISABLE();
  sA = SPI_FLASH_WREN;
  SPI_tx_rx();
  psm("jump SPI_register_disable");
 SPI_Flash_reset_WREN:
  SPI_DISABLE();
  sA = SPI_FLASH_WRDI;
  SPI_tx_rx();
  psm("jump SPI_register_disable");
 SPI_Flash_read_SR:
  SPI_DISABLE();
  sA = SPI_FLASH_RDSR;
  // transmit sA and receive the byte after that.
  SPI_tx_rx_twice();
 SPI_register_disable:
  SPI_DISABLE();
}

// Erase the SPI flash sector pointed to by the address.
// NOTE: I SHOULD CHECK THIS TO AVOID BOOTLOADER OVERWRITING!!
void SPI_Flash_erase_sector() {
  push3();
  SPI_Flash_set_WREN();
  sA = SPI_FLASH_SE;
  SPI_tx_rx();
  SPI_Flash_tx_address();
  SPI_DISABLE();
  SPI_Flash_wait_WIP();
}

#endif
