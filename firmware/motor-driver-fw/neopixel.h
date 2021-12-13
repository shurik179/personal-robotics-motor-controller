#ifndef _MD_NEOPIXEL_H
#define _MD_NEOPIXEL_H
#include <Arduino.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "globals.h"

extern PIO neopixel_pio;

void neopixel_setup();

void neopixel_color(uint32_t pixel_grb);

void neopixel_update();

#endif
