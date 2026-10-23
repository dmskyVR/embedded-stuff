Originally, I had most of the application logic running inside the SysTick interrupt handler, including the LED blinking and RGB animation. It worked, but the interrupt was doing a lot more than it needed to.

For this version, I separated the hardware-specific code into a Board Support Package (bsp.c and bsp.h) and moved the application logic into the main loop.

The functionality is still exactly the same.

## What Changed

- Added a BSP to handle GPIO configuration, LED control, and interrupts.
- Simplified the SysTick handler to increment a counter every millisecond.
- Moved the LED blinking and RGB animation into the main loop using non-blocking timing.
- Used a software flag to communicate button presses between the interrupt handler and main loop.
- Used atomic GPIO writes through BSRR and a critical section to protect shared state from race conditions.
- Kept the application logic separate from the STM32-specific register operations.

This was mainly an exercise in understanding foreground/background architecture and making my code more organized and less prone to race conditions.
