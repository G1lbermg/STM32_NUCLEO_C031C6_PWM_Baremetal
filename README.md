# Pulse Width Modulation
## STM32 NUCLEO-C031C6 Bare Metal Implementation

Baremetal C implementation for pulse width modulation on STM32C031C6. Uses CubeMX only for SWD/debug and clock initialization. All GPIO, timer, and USART manipulation is done via direct memory-mapped register access. 

### 1. Hardware Setup

   * **Board:** NUCLEO-C031C6
   * **MCU:** STM32C031C6 (ARM Cortex-M0+)
   * **Debugger:** ST-LINK V2-1 (SWD interface)
   * **Onboard LED:** Configured to PA5 (User LED 4)
   * **Onboard Button (Ver1):** Configured to PC13 (B1 USER)
   * **Potentiometer (Ver3):**
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
   1. **Import:** "PWM Ver1", "PWM Ver2", or "PWM Ver3" sub folder as STM32CubeIde project
   2. **Setup (Option):** Review and adjust the error handler logging and LED indicator settings within the error_check_utilities.h file
   3. **Build:** Select Project > Build Project
   4. **Flash:** use Run or Debug icons
   5. **Run**:
      * Open serial terminal and set to 9600 Baud
      * Ver1: Press button and observe the LED dimming and messages printed to the terminal
      * Ver2: 
      * Ver3: Turn potentiometer knob and observe the LED dimming and messages printed to the terminal
