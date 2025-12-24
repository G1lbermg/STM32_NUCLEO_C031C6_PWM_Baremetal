# Pulse Width Modulation
## STM32 NUCLEO-C031C6 Bare-Metal Implementation

This repository contains a bare-metal C implementation of pulse width modulation on the STM32C031C6. While CubeMX is used for SWD/debug and clock initialization, all GPIO, timer, USART, and ADC manipulation is performed via direct memory-mapped register access. The project is split into two different versions. In Ver1: the user presses the onboard button to change the LED's brightness and a state machine keeps track of these presses to determine the PWM duty cycle. In Ver2: the MCU's analog to digital converter converts the variable signal from a potentiometer into LED brightness levels. Also included in each version is a multi-tiered error handler. The handler is integrated into all BSPs and enables the system to recover gracefully from common faults and protect itself from critical ones. Depending on the user's preference, the error handler can provide error logging using USART and/or indicate critical errors with an external LED.

### 1. Hardware Setup

   * **Board:** NUCLEO-C031C6
   * **MCU:** STM32C031C6 (ARM Cortex-M0+)
   * **Debugger:** ST-LINK V2-1 (SWD interface)
   * **Onboard LED:** Configured to PA5 (User LED 4)
   * **External LED:** Configured to PB4
   * **Onboard Button (Ver1):** Configured to PC13 (B1 USER)
   * **Potentiometer (Ver2):**
     * PWR/GND leads connected to 3v3/GND respectively.
     * Variable lead connected to PA0.

### 2. Software Setup

  * **IDE:** STM32CubeIDE 2.0.0
  * **Programming Languages:** C
  * **Toolchain:** GNU Tools for STM32 (13.3.rel1)
  * **Libraries & Header Files:** 
    * STM32 LL libraries
    * CMSIS STM32C0xx
    * CMSIS STM32C031xx (memory map)

### 3. How to Run
   1. **Import:** "PWM_Ver1" or "PWM_Ver2" sub folder as STM32CubeIde project
   2. **Setup (Option):** Review and adjust the error handler logging and LED indicator settings within the error_check_utilities.h file
   3. **Build:** Select Project > Build Project
   4. **Flash:** use Run or Debug icons
   5. **Run**:
      * Open serial terminal and set to 9600 Baud
      * Ver1: Press button and observe the LED dimming and messages printed to the terminal
      * Ver2: Turn potentiometer knob and observe the LED dimming and messages printed to the terminal.
