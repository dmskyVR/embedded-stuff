#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>

#define BSP_TICKS_PER_SEC 1000U

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef enum
{
    RGB_OFF,
    RGB_RED,
    RGB_GREEN,
    RGB_BLUE,
    RGB_YELLOW,
    RGB_CYAN,
    RGB_MAGENTA,
    RGB_WHITE
} RGB_Color;

void BSP_init(void);

u32 BSP_getTicks(void);

bool BSP_buttonPressed(void);

void BSP_setRGB(RGB_Color color);

void BSP_onboardLED(bool on);

void BSP_yellowLED(bool on);

#endif