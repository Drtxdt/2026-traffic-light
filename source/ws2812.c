#include "ws2812.h"

#include "appinc.h"
#include "IOMuxManager.h"

#define WS2812_PAD             CSK_IOMUX_PAD_A
#define WS2812_PIN             10U
#define WS2812_PIN_MASK        CSK_GPIO_PIN10

#define WS2812_BIT_NS          1250U
#define WS2812_T0H_NS          320U
#define WS2812_T1H_NS          720U
#define WS2812_RESET_US        300U
#if TRAFFIC_LIGHT_AUTO_CYCLE
#define WS2812_YELLOW_BIT_NS   1230U
#define WS2812_YELLOW_T0H_NS    280U
#define WS2812_YELLOW_T1H_NS    740U
#endif

#define WS2812_ALWAYS_INLINE   __attribute__((always_inline))

static uint32_t s_cycles_per_bit;
static uint32_t s_cycles_t0h;
static uint32_t s_cycles_t1h;
static uint32_t s_cycles_reset;
#if TRAFFIC_LIGHT_AUTO_CYCLE
static uint32_t s_yellow_cycles_per_bit;
static uint32_t s_yellow_cycles_t0h;
static uint32_t s_yellow_cycles_t1h;
#endif

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

static inline uint32_t ws2812_ns_to_cycles(uint32_t cpu_hz, uint32_t ns)
{
    const uint32_t cycles_per_us = cpu_hz / 1000000U;

    return ((cycles_per_us * ns) + 500U) / 1000U;
}

static inline void __fast__ WS2812_ALWAYS_INLINE ws2812_send_bit(uint32_t high_cycles)
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

#if TRAFFIC_LIGHT_AUTO_CYCLE
static inline void __fast__ WS2812_ALWAYS_INLINE ws2812_send_yellow_bit(uint32_t high_cycles)
{
    const uint32_t start = ws2812_cycle32();

    IP_GPIO->REG_DOUTSET.all = WS2812_PIN_MASK;
    while ((uint32_t)(ws2812_cycle32() - start) < high_cycles) {
        __NOP();
    }

    IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
    while ((uint32_t)(ws2812_cycle32() - start) < s_yellow_cycles_per_bit) {
        __NOP();
    }
}

static void __fast__ ws2812_send_yellow_byte(uint8_t value)
{
    for (uint32_t mask = 0x80; mask != 0; mask >>= 1) {
        ws2812_send_yellow_bit((value & mask) ? s_yellow_cycles_t1h : s_yellow_cycles_t0h);
    }
}

static void __fast__ ws2812_send_yellow_grb(uint8_t green, uint8_t red, uint8_t blue)
{
    ws2812_send_yellow_byte(green);
    ws2812_send_yellow_byte(red);
    ws2812_send_yellow_byte(blue);
}
#endif

static void ws2812_update_timing(void)
{
    const uint32_t cpu_hz = BOARD_BOOTCLOCKRUN_CRM_CORE_CLK;
    const uint32_t cycles_per_us = cpu_hz / 1000000U;

    s_cycles_per_bit = ws2812_ns_to_cycles(cpu_hz, WS2812_BIT_NS);
    s_cycles_t0h = ws2812_ns_to_cycles(cpu_hz, WS2812_T0H_NS);
    s_cycles_t1h = ws2812_ns_to_cycles(cpu_hz, WS2812_T1H_NS);
    s_cycles_reset = cycles_per_us * WS2812_RESET_US;
#if TRAFFIC_LIGHT_AUTO_CYCLE
    s_yellow_cycles_per_bit = ws2812_ns_to_cycles(cpu_hz, WS2812_YELLOW_BIT_NS);
    s_yellow_cycles_t0h = ws2812_ns_to_cycles(cpu_hz, WS2812_YELLOW_T0H_NS);
    s_yellow_cycles_t1h = ws2812_ns_to_cycles(cpu_hz, WS2812_YELLOW_T1H_NS);
#endif

    if (s_cycles_t0h == 0U) {
        s_cycles_t0h = 1U;
    }
    if (s_cycles_t1h <= s_cycles_t0h) {
        s_cycles_t1h = s_cycles_t0h + 1U;
    }
    if (s_cycles_per_bit <= s_cycles_t1h) {
        s_cycles_per_bit = s_cycles_t1h + 1U;
    }
#if TRAFFIC_LIGHT_AUTO_CYCLE
    if (s_yellow_cycles_t0h == 0U) {
        s_yellow_cycles_t0h = 1U;
    }
    if (s_yellow_cycles_t1h <= s_yellow_cycles_t0h) {
        s_yellow_cycles_t1h = s_yellow_cycles_t0h + 1U;
    }
    if (s_yellow_cycles_per_bit <= s_yellow_cycles_t1h) {
        s_yellow_cycles_per_bit = s_yellow_cycles_t1h + 1U;
    }
#endif
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

#if TRAFFIC_LIGHT_AUTO_CYCLE
void ws2812_show_rgb_frame_yellow(const ws2812_rgb_t *pixels, uint32_t count)
{
    const uint32_t mstatus = __RV_CSR_READ(CSR_MSTATUS);

    __RV_CSR_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
    IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
    ws2812_wait_cycles(s_cycles_reset);

    for (uint32_t i = 0U; i < count; i++) {
        ws2812_send_yellow_grb(pixels[i].green, pixels[i].red, pixels[i].blue);
    }
    IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
    ws2812_wait_cycles(s_cycles_reset);

    /*
     * The first physical LED is the panel's top-right pixel.  It is the only
     * LED affected by the first yellow-profile word after reset.  Correct it
     * with one normal-timing pixel after the complete yellow frame has
     * latched.  The first LED consumes these 24 bits, so downstream LEDs keep
     * their already-latched yellow-frame values.
     */
    if (count != 0U) {
        ws2812_send_grb(pixels[0].green, pixels[0].red, pixels[0].blue);
        IP_GPIO->REG_DOUTCLEAR.all = WS2812_PIN_MASK;
        ws2812_wait_cycles(s_cycles_reset);
    }

    if (mstatus & MSTATUS_MIE) {
        __RV_CSR_SET(CSR_MSTATUS, MSTATUS_MIE);
    }
}
#endif

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
