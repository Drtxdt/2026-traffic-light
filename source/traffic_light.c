#include "traffic_light.h"

#include "ws2812.h"

#define TRAFFIC_LIGHT_DIM       WS2812_MATRIX_WIDTH
#define TRAFFIC_LIGHT_BRIGHT    64U

static ws2812_rgb_t s_frame[WS2812_LED_COUNT];

static const ws2812_rgb_t k_black = {0, 0, 0};
static const ws2812_rgb_t k_red = {TRAFFIC_LIGHT_BRIGHT, 0, 0};
static const ws2812_rgb_t k_green = {0, TRAFFIC_LIGHT_BRIGHT, 0};

static uint32_t traffic_light_index(uint32_t x, uint32_t y)
{
    return y * TRAFFIC_LIGHT_DIM + x;
}

static void traffic_light_fill(ws2812_rgb_t color)
{
    for (uint32_t i = 0; i < WS2812_LED_COUNT; i++) {
        s_frame[i] = color;
    }
}

static void traffic_light_pixel(uint32_t x, uint32_t y, ws2812_rgb_t color)
{
    if (x < WS2812_MATRIX_WIDTH && y < WS2812_MATRIX_HEIGHT) {
        s_frame[traffic_light_index(x, y)] = color;
    }
}

static void traffic_light_hline(uint32_t x0, uint32_t x1, uint32_t y, ws2812_rgb_t color)
{
    for (uint32_t x = x0; x <= x1; x++) {
        traffic_light_pixel(x, y, color);
    }
}

static void traffic_light_vline(uint32_t x, uint32_t y0, uint32_t y1, ws2812_rgb_t color)
{
    for (uint32_t y = y0; y <= y1; y++) {
        traffic_light_pixel(x, y, color);
    }
}

static void traffic_light_rect(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1, ws2812_rgb_t color)
{
    for (uint32_t y = y0; y <= y1; y++) {
        traffic_light_hline(x0, x1, y, color);
    }
}

static void traffic_light_draw_straight(void)
{
    traffic_light_rect(7, 8, 5, 14, k_green);
    traffic_light_hline(7, 8, 1, k_green);
    traffic_light_hline(6, 9, 2, k_green);
    traffic_light_hline(5, 6, 3, k_green);
    traffic_light_hline(9, 10, 3, k_green);
    traffic_light_hline(4, 5, 4, k_green);
    traffic_light_hline(10, 11, 4, k_green);
    traffic_light_hline(3, 4, 5, k_green);
    traffic_light_hline(11, 12, 5, k_green);
}

static void traffic_light_draw_left(void)
{
    traffic_light_rect(5, 14, 7, 8, k_green);
    traffic_light_vline(1, 7, 8, k_green);
    traffic_light_vline(2, 6, 7, k_green);
    traffic_light_vline(2, 8, 9, k_green);
    traffic_light_vline(3, 5, 6, k_green);
    traffic_light_vline(3, 9, 10, k_green);
    traffic_light_vline(4, 4, 5, k_green);
    traffic_light_vline(4, 10, 11, k_green);
    traffic_light_vline(5, 3, 4, k_green);
    traffic_light_vline(5, 11, 12, k_green);
}

static void traffic_light_draw_right(void)
{
    traffic_light_rect(1, 10, 7, 8, k_green);
    traffic_light_vline(14, 7, 8, k_green);
    traffic_light_vline(13, 6, 7, k_green);
    traffic_light_vline(13, 8, 9, k_green);
    traffic_light_vline(12, 5, 6, k_green);
    traffic_light_vline(12, 9, 10, k_green);
    traffic_light_vline(11, 4, 5, k_green);
    traffic_light_vline(11, 10, 11, k_green);
    traffic_light_vline(10, 3, 4, k_green);
    traffic_light_vline(10, 11, 12, k_green);
}

void traffic_light_init(void)
{
    ws2812_init();
    traffic_light_show(TRAFFIC_LIGHT_OFF);
}

void traffic_light_show(traffic_light_state_t state)
{
    traffic_light_fill(k_black);

    switch (state) {
    case TRAFFIC_LIGHT_RED:
        traffic_light_fill(k_red);
        break;
    case TRAFFIC_LIGHT_STRAIGHT:
        traffic_light_draw_straight();
        break;
    case TRAFFIC_LIGHT_LEFT:
        traffic_light_draw_left();
        break;
    case TRAFFIC_LIGHT_RIGHT:
        traffic_light_draw_right();
        break;
    case TRAFFIC_LIGHT_OFF:
    default:
        break;
    }

    ws2812_show_rgb_frame(s_frame, WS2812_LED_COUNT);
}

void traffic_light_show_command_index(uint32_t index)
{
    switch (index) {
    case 0:
        traffic_light_show(TRAFFIC_LIGHT_RED);
        break;
    case 1:
        traffic_light_show(TRAFFIC_LIGHT_STRAIGHT);
        break;
    case 2:
        traffic_light_show(TRAFFIC_LIGHT_LEFT);
        break;
    case 3:
        traffic_light_show(TRAFFIC_LIGHT_RIGHT);
        break;
    default:
        break;
    }
}
