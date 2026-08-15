#ifndef __APP_WS2812_H__
#define __APP_WS2812_H__

#include <stdint.h>

#include "traffic_light_config.h"

#define WS2812_MATRIX_WIDTH     16U
#define WS2812_MATRIX_HEIGHT    16U
#define WS2812_LED_COUNT        (WS2812_MATRIX_WIDTH * WS2812_MATRIX_HEIGHT)

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} ws2812_rgb_t;

void ws2812_init(void);
void ws2812_clear(void);
void ws2812_show_rgb_frame(const ws2812_rgb_t *pixels, uint32_t count);
#if TRAFFIC_LIGHT_AUTO_CYCLE
void ws2812_show_rgb_frame_yellow(const ws2812_rgb_t *pixels, uint32_t count);
#endif
void ws2812_show_solid_rgb(uint8_t red, uint8_t green, uint8_t blue);

#endif
