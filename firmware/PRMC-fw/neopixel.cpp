#include <Arduino.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "ws2812.pio.h"
#include "globals.h"
#include "pins.h"
#include "neopixel.h"


#define IS_RGBW false



PIO neopixel_pio  = pio1;

void neopixel_init() {
    uint neopixel_offset = pio_add_program(neopixel_pio, &ws2812_program);
    uint neopixel_sm = pio_claim_unused_sm(neopixel_pio, true);
    ws2812_program_init(neopixel_pio, neopixel_sm, neopixel_offset,PIN_NEOPIXEL, 800000, IS_RGBW);
}

void neopixel_set_color(uint32_t c) {
    pio_sm_put_blocking(neopixel_pio, 0, c << 8u);
}

void neopixel_update(){
  uint32_t color = YELLOW; //default - if no motor is enabled 
  if (REGBANK[REG_ENABLE]) {//at least one motor is enabled 
    color = GREEN;
  }
  if (REGBANK[REG_STATUS]) {//there is an error on one of motors
    color = RED;
  }
  neopixel_set_color(color);
}
