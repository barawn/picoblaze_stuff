#ifndef PB_DELAY_H
#define PB_DELAY_H

#include "pb_func_helpers.h"
#define DELAY3MS( cnt ) \
  FUNC1( cnt , delay3ms_fn )
void delay3ms_fn() {
  while (sA != 0) {
    // each pass takes 6 clocks (sub/subcy/jump) so 50K*6=300K cycles=3 ms
    sB.sC = 50000;
    do {
      sB.sC -= 1;
    } while (Z == 0);
    sA -= 1;
  }
}

// overall timing:
//
// once per: call/return = 4 clocks
// once per loop:
// compare     = 2 clocks
// move        = 2 clocks
// jump        = 2 clocks
// sub         = 2 clocks
// = 8 clock overhead
// each loop is 4 cycles (sub/jump)
// so 1 microsecond is 23 cycles (23*4 + 8 = 100)
// this gives us 

#define DELAY1US( cnt ) \
  FUNC1( cnt, delay1us_fn )
void delay1us_fn() {
  while (sA != 0) {
    sB = 23;
    do {
      sB -= 1;
    } while (Z == 0);
    sA -= 1;
  }
}

#endif
