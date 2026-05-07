#ifndef __APP_TRAFFIC_LIGHT_H__
#define __APP_TRAFFIC_LIGHT_H__

#include <stdint.h>

typedef enum {
    TRAFFIC_LIGHT_OFF = 0,
    TRAFFIC_LIGHT_RED,
    TRAFFIC_LIGHT_STRAIGHT,
    TRAFFIC_LIGHT_LEFT,
    TRAFFIC_LIGHT_RIGHT,
} traffic_light_state_t;

void traffic_light_init(void);
void traffic_light_show(traffic_light_state_t state);
void traffic_light_show_command_index(uint32_t index);

#endif
