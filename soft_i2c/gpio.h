#ifndef GPIO_H
#define GPIO_H
#include "cpp_helpers.h"

// If you define MULTIGPIO
// you can then use GPIO_FUNCTIONS()
// to create multiple named copies
// for each GPIO expander which
// will use the common functions for
// each.
// If you only plan on using a few
// GPIO calls, you can just set
// sSpare to the GPIO address
// beforehand. But of course if you
// make even 3 calls it's cheaper
// to define a convenience function.
// without:
// load, call (xN)
//
// with:
// call (xN)
// load
// call
// return

#ifndef MULTIGPIO
#ifndef GPIO_I2C_ADDRESS
#error "GPIO_I2C_ADDRESS not defined!"
#endif
#define SET_GPIO_ADDRESS_IN_COMMON
#endif

void gpio_write() {
#ifdef SET_GPIO_ADDRESS_IN_COMMON
  sSpare = GPIO_I2C_ADDRESS;
#endif
  store(I2C_BUFFER(2), sSpare);
  I2C_reg_write();
}
void gpio_write_output() {
  sB = 1;
  gpio_write();
}
void gpio_read_input() {
#ifdef SET_GPIO_ADDRESS_IN_COMMON
  sSpare = GPIO_I2C_ADDRESS;
#endif
  sA = 1;
  store(I2C_BUFFER(1), sSpare);
  I2C_reg_read();
}

// Macro to create device-specific functions
// you just do GPIO_FUNCTIONS(blah , addr)
// and you get blah_write, blah_write_output,
// blah_read_input

#define WRITE _write()
#define GPIO_FUNCTIONS( name , addr )		\
  void CAT( name , WRITE ) {		        \
    sSpare = addr;				\
    gpio_write();                               \
  }                                             \
  void CAT( name , _write_output() ) {		\
    sSpare = addr;                              \
    gpio_write_output();                        \
  }                                             \
  void CAT( name , _read_input() ) {		\
    sSpare = addr;                              \
    gpio_read_input();                          \
  }

/*
// specific ones
void gpio0_write() {
  sSpare = GP0_ADDR;
  gpio_write();
}
void gpio1_write() {
  sSpare = GP1_ADDR;
  gpio_write();
}
void gpio0_write_output() {
  sSpare = GP0_ADDR;
  gpio_write_output();
}
void gpio1_write_output() {
  sSpare = GP1_ADDR;
  gpio_write_output();
}
void gpio0_read_input() {
  sSpare = GP0_ADDR;
  gpio_read_input();
}
void gpio1_read_input() {
  sSpare = GP1_ADDR;
  gpio_read_input();
}
*/

#endif
