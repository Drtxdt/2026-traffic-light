#include "ws2812.h"

#include "appinc.h"
#include "IOMuxManager.h"

#define WS2812_PAD             CSK_IOMUX_PAD_A
#define WS2812_PIN             10U
#define WS2812_PIN_MASK        CSK_GPIO_PIN10

static uint32_t s_cycles_per_bit;
static uint32_t s_cycles_t0h;
static uint32_t s_cycles_t1h;
static uint32_t s_cycles_reset;

static inline uint32_t ws2812_cycle32(void)
{
    return __RV_CSR_READ(CSR_MCYCLE);
}

static inline void ws2812_wait_cycles(uint32_t cycles)
{
    const uint32_t start = ws2812_cycle32();
    while ((uint32_t)(ws2812_cycle32() - start) < cycles) {
        __NOP();
    }
}

static void __fast__ ws2812_send_bit(uint32_t high_cycles)
{
    const uint32_t start = ws2812_cycle32();

    IP_GPIO->REG_DOUTSET.all = WS2812_PIN_MASK;
    while ((uint32_t)(ws2812_cycle32() - start) < high_cycles) {
        __NOP();
    }

    IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
    while ((uint32_t)(ws2812_cycle32() - start) < s_cycles_per_bit) {
        __NOP();
    }
}

static void __fast__ ws2812_send_byte(uint8_t value)
{
    for (uint32_t mask = 0x80; mask != 0; mask >>= 1) {
        ws2812_send_bit((value & mask) ? s_cycles_t1h : s_cycles_t0h);
    }
}

static void __fast__ ws2812_send_grb(uint8_t green, uint8_t red, uint8_t blue)
{
    ws2812_send_byte(green);
    ws2812_send_byte(red);
    ws2812_send_byte(blue);
}

static void ws2812_update_timing(void)
{
    const uint32_t cpu_hz = BOARD_BOOTCLOCKRUN_CRM_CORE_CLK;
    const uint32_t cycles_per_us = cpu_hz / 1000000U;

    s_cycles_per_bit = (cycles_per_us * 5U) / 4U;     // 1.25 us
    s_cycles_t0h = (cycles_per_us * 35U) / 100U;      // 0.35 us
    s_cycles_t1h = (cycles_per_us * 70U) / 100U;      // 0.70 us
    s_cycles_reset = cycles_per_us * 300U;            // >= 280 us latch/reset margin

    if (s_cycles_t0h == 0U) {
        s_cycles_t0h = 1U;
    }
    if (s_cycles_t1h <= s_cycles_t0h) {
        s_cycles_t1h = s_cycles_t0h + 1U;
    }
    if (s_cycles_per_bit <= s_cycles_t1h) {
        s_cycles_per_bit = s_cycles_t1h + 1U;
    }
}

void ws2812_init(void)
{
    ws2812_update_timing();

    __HAL_CRM_GPIO0_CLK_ENABLE();
    IOMuxManager_PinConfigure(WS2812_PAD, WS2812_PIN, CSK_IOMUX_FUNC_DEFAULT);
    IOMuxManager_ModeConfigure(WS2812_PAD, WS2812_PIN, HAL_IOMUX_NONE_MODE);
    GPIO_Initialize(GPIOA(), NULL, NULL);
    GPIO_SetDir(GPIOA(), WS2812_PIN_MASK, CSK_GPIO_DIR_OUTPUT);
    IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
    ws2812_wait_cycles(s_cycles_reset);
}

void ws2812_show_rgb_frame(const ws2812_rgb_t *pixels, uint32_t count)
{
    const uint32_t mstatus = __RV_CSR_READ(CSR_MSTATUS);

    __RV_CSR_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
    IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
    ws2812_wait_cycles(s_cycles_reset);
    for (uint32_t i = 0; i < count; i++) {
        ws2812_send_grb(pixels[i].green, pixels[i].red, pixels[i].blue);
    }
    IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
    ws2812_wait_cycles(s_cycles_reset);

    if (mstatus & MSTATUS_MIE) {
        __RV_CSR_SET(CSR_MSTATUS, MSTATUS_MIE);
    }
}

void ws2812_show_solid_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    const uint32_t mstatus = __RV_CSR_READ(CSR_MSTATUS);

    __RV_CSR_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
    IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
    ws2812_wait_cycles(s_cycles_reset);
    for (uint32_t i = 0; i < WS2812_LED_COUNT; i++) {
        ws2812_send_grb(green, red, blue);
    }
    IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
    ws2812_wait_cycles(s_cycles_reset);

    if (mstatus & MSTATUS_MIE) {
        __RV_CSR_SET(CSR_MSTATUS, MSTATUS_MIE);
    }
}

void ws2812_clear(void)
{
    ws2812_show_solid_rgb(0, 0, 0);
}
