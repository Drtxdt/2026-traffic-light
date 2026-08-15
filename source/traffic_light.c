#include "traffic_light.h"

#include "traffic_light_config.h"
#if TRAFFIC_LIGHT_AUTO_CYCLE
#include "FreeRTOS.h"
#include "task.h"
#endif
#include "ws2812.h"

#define TRAFFIC_LIGHT_DIM       WS2812_MATRIX_WIDTH
#define TRAFFIC_LIGHT_BRIGHT    8U

/*
 * Matrix wiring map.
 *
 * Logical coordinates use x=0..15 from left to right, y=0..15 from top to
 * bottom. The physical LED stream is configured for a panel whose first LED is
 * at the top-right corner, with row 0 running right-to-left and subsequent
 * rows in serpentine order.
 *
 * Manual tuning:
 * - If left/right are mirrored, change TRAFFIC_LIGHT_FIRST_ROW_RIGHT_TO_LEFT.
 * - If every other row is shifted/reversed incorrectly, change
 *   TRAFFIC_LIGHT_SERPENTINE.
 */
#define TRAFFIC_LIGHT_FIRST_ROW_RIGHT_TO_LEFT  1U
#define TRAFFIC_LIGHT_SERPENTINE               1U

static ws2812_rgb_t s_frame[WS2812_LED_COUNT];
#if TRAFFIC_LIGHT_AUTO_CYCLE
typedef enum {
    TRAFFIC_LIGHT_CYCLE_IDLE = 0,
    TRAFFIC_LIGHT_CYCLE_WAIT_YELLOW,
    TRAFFIC_LIGHT_CYCLE_WAIT_RED,
} traffic_light_cycle_stage_t;

static volatile uint8_t s_cycle_active;
static traffic_light_cycle_stage_t s_cycle_stage;
static traffic_light_state_t s_cycle_direction;
static TickType_t s_cycle_deadline;
#endif

static const ws2812_rgb_t k_black = {0, 0, 0};
static const ws2812_rgb_t k_red = {TRAFFIC_LIGHT_BRIGHT, 0, 0};
#if TRAFFIC_LIGHT_AUTO_CYCLE
static const ws2812_rgb_t k_yellow = {TRAFFIC_LIGHT_BRIGHT, TRAFFIC_LIGHT_BRIGHT, 0};
#endif
static const ws2812_rgb_t k_green = {0, TRAFFIC_LIGHT_BRIGHT, 0};

#if TRAFFIC_LIGHT_AUTO_CYCLE
static void traffic_light_render_begin(void)
{
    vTaskSuspendAll();
}

static void traffic_light_render_end(void)
{
    (void)xTaskResumeAll();
}
#endif

static uint32_t traffic_light_index(uint32_t x, uint32_t y)
{
    uint32_t physical_x = x;
    uint32_t row_right_to_left = TRAFFIC_LIGHT_FIRST_ROW_RIGHT_TO_LEFT;

#if TRAFFIC_LIGHT_SERPENTINE
    if ((y & 1U) != 0U) {
        row_right_to_left = !row_right_to_left;
    }
#endif

    if (row_right_to_left) {
        physical_x = (WS2812_MATRIX_WIDTH - 1U) - x;
    }

    return y * TRAFFIC_LIGHT_DIM + physical_x;
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

static void traffic_light_draw_straight(ws2812_rgb_t color)
{
    traffic_light_rect(7, 8, 5, 14, color);
    traffic_light_hline(7, 8, 1, color);
    traffic_light_hline(6, 9, 2, color);
    traffic_light_hline(5, 6, 3, color);
    traffic_light_hline(9, 10, 3, color);
    traffic_light_hline(4, 5, 4, color);
    traffic_light_hline(10, 11, 4, color);
    traffic_light_hline(3, 4, 5, color);
    traffic_light_hline(11, 12, 5, color);
}

static void traffic_light_draw_left(ws2812_rgb_t color)
{
    traffic_light_rect(5, 14, 7, 8, color);
    traffic_light_vline(1, 7, 8, color);
    traffic_light_vline(2, 6, 7, color);
    traffic_light_vline(2, 8, 9, color);
    traffic_light_vline(3, 5, 6, color);
    traffic_light_vline(3, 9, 10, color);
    traffic_light_vline(4, 4, 5, color);
    traffic_light_vline(4, 10, 11, color);
    traffic_light_vline(5, 3, 4, color);
    traffic_light_vline(5, 11, 12, color);
}

static void traffic_light_draw_right(ws2812_rgb_t color)
{
    traffic_light_rect(1, 10, 7, 8, color);
    traffic_light_vline(14, 7, 8, color);
    traffic_light_vline(13, 6, 7, color);
    traffic_light_vline(13, 8, 9, color);
    traffic_light_vline(12, 5, 6, color);
    traffic_light_vline(12, 9, 10, color);
    traffic_light_vline(11, 4, 5, color);
    traffic_light_vline(11, 10, 11, color);
    traffic_light_vline(10, 3, 4, color);
    traffic_light_vline(10, 11, 12, color);
}

#if TRAFFIC_LIGHT_AUTO_CYCLE
static void traffic_light_show_yellow_direction(traffic_light_state_t state)
{
    traffic_light_render_begin();
    traffic_light_fill(k_black);

    switch (state) {
    case TRAFFIC_LIGHT_STRAIGHT:
        traffic_light_draw_straight(k_yellow);
        break;
    case TRAFFIC_LIGHT_LEFT:
        traffic_light_draw_left(k_yellow);
        break;
    case TRAFFIC_LIGHT_RIGHT:
        traffic_light_draw_right(k_yellow);
        break;
    default:
        break;
    }

    ws2812_show_rgb_frame_yellow(s_frame, WS2812_LED_COUNT);
    traffic_light_render_end();
}
#endif

void traffic_light_init(void)
{
    ws2812_init();
#if TRAFFIC_LIGHT_AUTO_CYCLE
    s_cycle_active = 0U;
    s_cycle_stage = TRAFFIC_LIGHT_CYCLE_IDLE;
    s_cycle_direction = TRAFFIC_LIGHT_OFF;
    s_cycle_deadline = 0U;
#endif
    traffic_light_show(TRAFFIC_LIGHT_OFF);
}

void traffic_light_show(traffic_light_state_t state)
{
#if TRAFFIC_LIGHT_AUTO_CYCLE
    traffic_light_render_begin();
#endif
    traffic_light_fill(k_black);

    switch (state) {
    case TRAFFIC_LIGHT_RED:
        traffic_light_fill(k_red);
        break;
    case TRAFFIC_LIGHT_STRAIGHT:
        traffic_light_draw_straight(k_green);
        break;
    case TRAFFIC_LIGHT_LEFT:
        traffic_light_draw_left(k_green);
        break;
    case TRAFFIC_LIGHT_RIGHT:
        traffic_light_draw_right(k_green);
        break;
    case TRAFFIC_LIGHT_OFF:
    default:
        break;
    }

    ws2812_show_rgb_frame(s_frame, WS2812_LED_COUNT);
#if TRAFFIC_LIGHT_AUTO_CYCLE
    traffic_light_render_end();
#endif
}

#if TRAFFIC_LIGHT_AUTO_CYCLE
static void traffic_light_cycle_finish_red(void)
{
    s_cycle_active = 0U;
    s_cycle_stage = TRAFFIC_LIGHT_CYCLE_IDLE;
    s_cycle_direction = TRAFFIC_LIGHT_OFF;
    traffic_light_show(TRAFFIC_LIGHT_RED);
}

void traffic_light_process(void)
{
    TickType_t now;

    vTaskSuspendAll();
    if (s_cycle_active == 0U) {
        (void)xTaskResumeAll();
        return;
    }

    now = xTaskGetTickCount();
    if ((int64_t)(now - s_cycle_deadline) < 0) {
        (void)xTaskResumeAll();
        return;
    }

    if (s_cycle_stage == TRAFFIC_LIGHT_CYCLE_WAIT_YELLOW) {
        s_cycle_stage = TRAFFIC_LIGHT_CYCLE_WAIT_RED;
        traffic_light_show_yellow_direction(s_cycle_direction);
        s_cycle_deadline = xTaskGetTickCount() + pdMS_TO_TICKS(TRAFFIC_LIGHT_PERIOD_MS);
        (void)xTaskResumeAll();
        return;
    }

    traffic_light_cycle_finish_red();
    (void)xTaskResumeAll();
}
#else
void traffic_light_process(void)
{
}
#endif

void traffic_light_show_command_index(uint32_t index)
{
    traffic_light_state_t state;

    if (index > 3U) {
        return;
    }
#if TRAFFIC_LIGHT_AUTO_CYCLE
    vTaskSuspendAll();
    if (s_cycle_active != 0U) {
        (void)xTaskResumeAll();
        return;
    }
#endif

    switch (index) {
    case 0:
        traffic_light_show(TRAFFIC_LIGHT_RED);
#if TRAFFIC_LIGHT_AUTO_CYCLE
        (void)xTaskResumeAll();
#endif
        return;
    case 1:
        state = TRAFFIC_LIGHT_STRAIGHT;
        break;
    case 2:
        state = TRAFFIC_LIGHT_LEFT;
        break;
    case 3:
        state = TRAFFIC_LIGHT_RIGHT;
        break;
    default:
#if TRAFFIC_LIGHT_AUTO_CYCLE
        (void)xTaskResumeAll();
#endif
        return;
    }

#if TRAFFIC_LIGHT_AUTO_CYCLE
    s_cycle_stage = TRAFFIC_LIGHT_CYCLE_WAIT_YELLOW;
    s_cycle_direction = state;
#endif
    traffic_light_show(state);
#if TRAFFIC_LIGHT_AUTO_CYCLE
    s_cycle_deadline = xTaskGetTickCount() + pdMS_TO_TICKS(TRAFFIC_LIGHT_PERIOD_MS);
    s_cycle_active = 1U;
    (void)xTaskResumeAll();
#endif
}

void traffic_light_stop(void)
{
#if TRAFFIC_LIGHT_AUTO_CYCLE
    vTaskSuspendAll();
    s_cycle_active = 0U;
    s_cycle_stage = TRAFFIC_LIGHT_CYCLE_IDLE;
    s_cycle_direction = TRAFFIC_LIGHT_OFF;
    s_cycle_deadline = 0U;
#endif

    traffic_light_show(TRAFFIC_LIGHT_OFF);
#if TRAFFIC_LIGHT_AUTO_CYCLE
    (void)xTaskResumeAll();
#endif
}
