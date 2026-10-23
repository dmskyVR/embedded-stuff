#include "bsp.h"

typedef enum
{
    STATE_NORMAL,
    STATE_RGB_ANIMATION
} State;

static State currentState = STATE_NORMAL;

static const RGB_Color colors[] =
{
    RGB_RED,
    RGB_BLUE,
    RGB_GREEN,
    RGB_MAGENTA,
    RGB_YELLOW,
    RGB_CYAN,
    RGB_WHITE
};

int main(void)
{
    BSP_init();

    u32 lastToggle = BSP_getTicks();
    u32 animationStart = 0U;
    u32 lastColorChange = 0U;
    u8 colorIndex = 0U;
    bool ledOn = true;


    for(;;)
    {
        u32 now = BSP_getTicks();
        if(currentState == STATE_NORMAL)
        {
            if((now - lastToggle) >= 500U)
            {
                ledOn = !ledOn;
                BSP_onboardLED(ledOn);
                BSP_yellowLED(!ledOn);

                lastToggle = now;
            }
        }
        else if(currentState == STATE_RGB_ANIMATION)
        {
            u32 elapsed = now - animationStart;
            u32 interval;

            if(elapsed < 2000U)
            {
                interval = 200U;
            }
            else if(elapsed < 4000U)
            {
                interval = 100U;
            }
            else
            {
                interval = 50U;
            }

            if(elapsed >= 5000U)
            {
                BSP_setRGB(RGB_OFF);
                BSP_onboardLED(true);
                BSP_yellowLED(false);
                ledOn = true;
                lastToggle = now;

                currentState = STATE_NORMAL;
            }
            else if((now - lastColorChange) >= interval)
            {
                colorIndex = (colorIndex + 1U) % 7U;
                BSP_setRGB(colors[colorIndex]);
                lastColorChange = now;
            }
        }
        if(BSP_buttonPressed())
        {
            if(currentState == STATE_NORMAL)
            {
                BSP_setRGB(RGB_RED);
                BSP_onboardLED(false);
                BSP_yellowLED(false);

                animationStart = now;
                lastColorChange = now;  
                colorIndex = 0U;

                currentState = STATE_RGB_ANIMATION;
            }
        }
    }
}