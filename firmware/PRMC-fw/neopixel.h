#ifndef _PRMC_NEOPIXEL_H
#define _PRMC_NEOPIXEL_H

//colors for Neopixel; note that it uses grb color order
#define  RED             0x004000
#define  GREEN           0x400000
#define  BLUE            0x000040
#define  YELLOW          0x404000

void neopixel_init();

void neopixel_set_color(uint32_t c);

void neopixel_update();

#endif
