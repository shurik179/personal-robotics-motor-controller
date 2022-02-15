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

void neopixel_set_colors(uint32_t pixel1,uint32_t pixel2) {
    pio_sm_put_blocking(neopixel_pio, 0, pixel1 << 8u);
    pio_sm_put_blocking(neopixel_pio, 0, pixel2 << 8u);
}

void neopixel_update(){
    uint32_t color1, color2;
    if ( ! have_i2c) {
        neopixel_set_colors(BLUE, BLUE);
        return;
    }
    color1=GREEN;
    color2=GREEN;
    if (*motor_status &  STATUS_M1_OFF) {
        color1 = RED;
    }
    if (*motor_status &  STATUS_M2_OFF) {
        color2 = RED;
    }


    neopixel_set_colors(color1,color2);
}
