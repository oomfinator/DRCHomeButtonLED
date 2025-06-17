#ifndef DRC_LED_PLUGIN_HPP
#define DRC_LED_PLUGIN_HPP

#include <stdint.h>

typedef enum DrcLedPattern
{
    DRC_LED_OFF = 0,
    DRC_LED_ON = 1,
    DRC_LED_TWO_TICKS = 2,
    DRC_LED_ONE_TICK = 3,
    DRC_LED_THREE_TICKS = 4,

} DrcLedPattern;

extern "C" {

uint32_t GetPersistentId() asm("GetPersistentId__Q2_2nn3actFv");

}

#endif // DRC_LED_PLUGIN_HPP