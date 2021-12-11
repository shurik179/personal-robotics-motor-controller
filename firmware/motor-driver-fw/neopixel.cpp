#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "ws2812.pio.h"
#include "globals.h"
#include "neopixel.h"
#define IS_RGBW false



PIO neopixel_pio  = pio0;

void neopixel_setup() {
  uint neopixel_offset = pio_add_program(neopixel_pio, &ws2812_program);
  uint neopixel_sm = pio_claim_unused_sm(neopixel_pio, true);
  ws2812_program_init(neopixel_pio, neopixel_sm, neopixel_offset,PIN_NEOPIXEL, 800000, IS_RGBW);
}

void neopixel_color(uint32_t pixel_grb) {
    pio_sm_put_blocking(neopixel_pio, 0, pixel_grb << 8u);
}
