# STM32 RGB LED Interrupt Project

This project uses STMicroelectronics' CMSIS device headers and was developed using the ARM GNU Toolchain.

I'm currently teaching myself embedded programming by following
Miro Samek's Modern Embedded Systems Programming course on YouTube.

After learning about interrupts, I wanted to get some hands-on
practice with my STM32 NUCLEO-C031C6, so I put together this
little project.

Normally, two LEDs alternate every 500 ms. When I press the
onboard USER button, they turn off and an RGB LED starts cycling
through seven colors for five seconds. The colors change faster
as the animation progresses, and afterward everything returns
to normal.

I used:
- C with CMSIS (no HAL)
- GPIO registers to control the LEDs
- SysTick interrupts for timing
- EXTI to detect the button press
- A simple state machine to switch between normal operation
  and the RGB animation

This was mainly an exercise in understanding how interrupts
work on a microcontroller and getting more comfortable
interacting with hardware registers.

## Hardware

- STM32 NUCLEO-C031C6
- Common-cathode RGB LED
- Yellow LED
- 220-ohm resistors
- Breadboard and jumper wires

## Demo

[Video coming soon]