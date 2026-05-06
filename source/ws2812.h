#ifndef __APP_WS2812_H__
#define __APP_WS2812_H__

#include <stdint.h>

void ws2812_init(void);
void ws2812_show_solid_rgb(uint8_t red, uint8_t green, uint8_t blue);

#endif
