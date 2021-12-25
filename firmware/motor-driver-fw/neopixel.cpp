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

void neopixel_set_color(uint32_t pixel_grb) {
    pio_sm_put_blocking(neopixel_pio, 0, pixel_grb << 8u);
}

void neopixel_update(){
    uint32_t color;
    if (*motor_status == STATUS_ON) {
        color = GREEN;
    } else {
        color = BLUE;
    }
    
    neopixel_set_color(color);
}
