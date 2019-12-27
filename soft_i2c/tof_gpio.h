#ifndef TOF_GPIO_H
#define TOF_GPIO_H

#define MULTIGPIO
#include "gpio.h"

#define GP0_ADDR 0x40
#define GP1_ADDR 0x42

// create gpio0/gpio1 functions
GPIO_FUNCTIONS( gpio0 , GP0_ADDR)
GPIO_FUNCTIONS( gpio1 , GP1_ADDR)

#endif
