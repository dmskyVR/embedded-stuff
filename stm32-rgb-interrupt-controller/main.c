#include <stdint.h>
#include <stdbool.h>
#include "stm32c031xx.h"
#include "stm32c0xx.h"
#include "system_stm32c0xx.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#define RGB_RED         GPIO_BSRR_BS0
#define RGB_GREEN       GPIO_BSRR_BS7
#define RGB_BLUE        GPIO_BSRR_BS4
#define LED_ONBOARD     GPIO_BSRR_BS5
#define LED_YELLOW      GPIO_BSRR_BS6
#define RGB_RED_OFF     GPIO_BSRR_BR0
#define RGB_GREEN_OFF   GPIO_BSRR_BR7
#define RGB_BLUE_OFF    GPIO_BSRR_BR4
#define LED_ONBOARD_OFF GPIO_BSRR_BR5
#define LED_YELLOW_OFF  GPIO_BSRR_BR6

typedef enum
{
    STATE_NORMAL,
    STATE_RGB_ANIMATION
} State;

volatile State currentState = STATE_NORMAL;

volatile bool buttonPressed = false;

void EXTI4_15_IRQHandler(void)
{
    if(EXTI->FPR1 & (1U << 13))
    {
        EXTI->FPR1 = (1U << 13);

        buttonPressed = true;
    }
}


void SysTick_Handler(void)
{
    static u16 ticks = 0U;
    static u16 rgbTicks = 0U;
    static u16 colorTicks = 0U;
    if(currentState == STATE_NORMAL)
    {
        ticks++;

        if(ticks >= 500U)
        {
            GPIOA->ODR ^= GPIO_ODR_OD5 | GPIO_ODR_OD6;
            ticks = 0U;
        }
    }
    else
    {
        static u8 color = 1U;
        u16 interval = 0;
        rgbTicks++;
        colorTicks++;
        if(rgbTicks <= 2000U)
        {
            interval = 200U;
        }
        else if(rgbTicks <= 4000U)
        {
            interval = 100U;
        }
        else
        {
            interval = 50U;
        }
        if(colorTicks >= interval)
        {
            if(color == 0)
            {
                GPIOA->BSRR = RGB_RED | RGB_BLUE_OFF | RGB_GREEN_OFF;
            }
            else if(color == 1)
            {
                GPIOA->BSRR = RGB_RED_OFF | RGB_BLUE | RGB_GREEN_OFF;
            }
            else if(color == 2)
            {
                GPIOA->BSRR = RGB_RED_OFF | RGB_BLUE_OFF | RGB_GREEN;
            }
            else if(color == 3)
            {
                GPIOA->BSRR = RGB_RED | RGB_BLUE | RGB_GREEN_OFF;
            }
            else if(color == 4)
            {
                GPIOA->BSRR = RGB_RED | RGB_BLUE_OFF | RGB_GREEN;
            }
            else if(color == 5)
            {
                GPIOA->BSRR = RGB_RED_OFF | RGB_BLUE | RGB_GREEN;
            }
            else if(color == 6)
            {
                GPIOA->BSRR = RGB_RED | RGB_BLUE | RGB_GREEN;
            }
            colorTicks = 0;
            color = (color + 1U) % 7;
        }
    
        if(rgbTicks >= 5000U)
        {
            GPIOA->BSRR = RGB_RED_OFF | RGB_BLUE_OFF | LED_ONBOARD | LED_YELLOW_OFF | RGB_GREEN_OFF;
            rgbTicks = 0U;
            colorTicks = 0U;
            ticks = 0U;
            color = 1U;
            currentState = STATE_NORMAL;
        }
    }
}

int main(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN | RCC_IOPENR_GPIOAEN;

    GPIOA->MODER &= ~(GPIO_MODER_MODE0_Msk | 
        GPIO_MODER_MODE4_Msk | 
        GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE6_Msk |
        GPIO_MODER_MODE7_Msk);

    GPIOA->MODER |= (GPIO_MODER_MODE0_0 |
        GPIO_MODER_MODE4_0 |
        GPIO_MODER_MODE5_0 |
        GPIO_MODER_MODE6_0 |
        GPIO_MODER_MODE7_0);

    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk;

    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT0 |
        GPIO_OTYPER_OT4 |
        GPIO_OTYPER_OT5 |
        GPIO_OTYPER_OT6 |
        GPIO_OTYPER_OT7);

    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0_Msk |
        GPIO_PUPDR_PUPD4_Msk |
        GPIO_PUPDR_PUPD5_Msk |
        GPIO_PUPDR_PUPD6_Msk |
        GPIO_PUPDR_PUPD7_Msk);

    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;
    GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_0;

    GPIOA->BSRR = RGB_GREEN_OFF |
              RGB_RED_OFF |
              RGB_BLUE_OFF |
              LED_ONBOARD |
              LED_YELLOW_OFF;

    SysTick->CTRL = 0U;
    SysTick->LOAD = (SystemCoreClock / 1000U) - 1U;
    SysTick->VAL = 0U;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    EXTI->EXTICR[3] &= ~(0x7U << 8);
    EXTI->EXTICR[3] |=  (0x2U << 8);
    EXTI->RTSR1 &= ~(1U << 13);
    EXTI->FTSR1 |= (1U << 13);
    EXTI->FPR1 = (1U << 13);
    EXTI->IMR1 |= (1u << 13);

    NVIC_EnableIRQ(EXTI4_15_IRQn);

    while(1)
    {
        if(buttonPressed)
        {
            buttonPressed = false;
            if(currentState == STATE_NORMAL)
            {
                GPIOA->BSRR = RGB_RED | RGB_BLUE_OFF | LED_ONBOARD_OFF | LED_YELLOW_OFF | RGB_GREEN_OFF;
                currentState = STATE_RGB_ANIMATION;
            }
        }
    }
}
