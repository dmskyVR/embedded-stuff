#include "bsp.h"
#include "stm32c0xx.h"

#define RGB_RED_SET        GPIO_BSRR_BS0
#define RGB_GREEN_SET      GPIO_BSRR_BS7
#define RGB_BLUE_SET       GPIO_BSRR_BS4
#define LED_ONBOARD_SET    GPIO_BSRR_BS5
#define LED_YELLOW_SET     GPIO_BSRR_BS6
#define RGB_RED_RESET      GPIO_BSRR_BR0
#define RGB_GREEN_RESET    GPIO_BSRR_BR7
#define RGB_BLUE_RESET     GPIO_BSRR_BR4
#define LED_ONBOARD_RESET  GPIO_BSRR_BR5
#define LED_YELLOW_RESET   GPIO_BSRR_BR6

static volatile u32 tickCounter = 0U;
static volatile bool buttonPressed = false;

void SysTick_Handler(void)
{
    tickCounter++;
}

void EXTI4_15_IRQHandler(void)
{
    if(EXTI->FPR1 & (1U << 13))
    {
        EXTI->FPR1 = (1U << 13);

        buttonPressed = true;
    }
}

void BSP_init(void)
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

    BSP_setRGB(RGB_OFF);
    BSP_onboardLED(true);
    BSP_yellowLED(false);
    
    SysTick->CTRL = 0U;
    SysTick->LOAD = (SystemCoreClock / BSP_TICKS_PER_SEC) - 1U;
    SysTick->VAL = 0U;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    EXTI->EXTICR[3] &= ~(0x7U << 8);
    EXTI->EXTICR[3] |=  (0x2U << 8);
    EXTI->RTSR1 &= ~(1U << 13);
    EXTI->FTSR1 |= (1U << 13);
    EXTI->FPR1 = (1U << 13);
    EXTI->IMR1 |= (1u << 13);

    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

u32 BSP_getTicks(void)
{
    return tickCounter;
}

bool BSP_buttonPressed(void)
{
    bool pressed;

    u32 primask = __get_PRIMASK();
    __disable_irq();

    pressed = buttonPressed;
    buttonPressed = false;

    __set_PRIMASK(primask);

    return pressed;
}

void BSP_onboardLED(bool on)
{
    GPIOA->BSRR = on ? LED_ONBOARD_SET : LED_ONBOARD_RESET;
}

void BSP_yellowLED(bool on)
{
    GPIOA->BSRR = on ? LED_YELLOW_SET : LED_YELLOW_RESET;
}

void BSP_setRGB(RGB_Color color)
{
    switch (color)
    {
        case RGB_OFF:
            GPIOA->BSRR = RGB_RED_RESET |
                          RGB_GREEN_RESET |
                          RGB_BLUE_RESET;
            break;

        case RGB_RED:
            GPIOA->BSRR = RGB_RED_SET |
                          RGB_GREEN_RESET |
                          RGB_BLUE_RESET;
            break;

        case RGB_GREEN:
            GPIOA->BSRR = RGB_RED_RESET |
                          RGB_GREEN_SET |
                          RGB_BLUE_RESET;
            break;

        case RGB_BLUE:
            GPIOA->BSRR = RGB_RED_RESET |
                          RGB_GREEN_RESET |
                          RGB_BLUE_SET;
            break;

        case RGB_YELLOW:
            GPIOA->BSRR = RGB_RED_SET |
                          RGB_GREEN_SET |
                          RGB_BLUE_RESET;
            break; 

        case RGB_CYAN:
            GPIOA->BSRR = RGB_RED_RESET |
                          RGB_GREEN_SET |
                          RGB_BLUE_SET;
            break;

        case RGB_MAGENTA:
            GPIOA->BSRR = RGB_RED_SET |
                          RGB_GREEN_RESET |
                          RGB_BLUE_SET;
            break;

        case RGB_WHITE:
            GPIOA->BSRR = RGB_RED_SET | 
                          RGB_GREEN_SET |
                          RGB_BLUE_SET;
            break;

        default:
            break;
    }
}