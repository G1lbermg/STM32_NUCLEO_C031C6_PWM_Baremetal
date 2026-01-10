# Pulse Width Modulation (PWM)
## STM32 NUCLEO-C031C6 Bare-Metal Implementation

This repository demonstrates a bare-metal C implementation of Pulse Width Modulation (PWM) on the STM32C031C6. While CubeMX is used for SWD/debug and clock initialization, all GPIO, Timer, USART, and ADC manipulation is performed via direct memory-mapped register access. The project also features a multi-tiered error handler integrated into all BSPs, allowing the system to recover gracefully from faults. Users can configure the handler to provide real-time logging via USART or indicate critical errors via an external LED.

---

### 🛠️ STM32 Nucleo Setup

| Category | Details |
| :--- | :--- |
| **Hardware** | NUCLEO-C031C6 (ARM Cortex-M0+) |
| **Debugger** | ST-LINK V2-1 (SWD interface) |
| **IDE** | STM32CubeIDE 2.0.0 |
| **Toolchain** | GNU Tools for STM32 (13.3.rel1) |
| **Libraries** | STM32 LL, CMSIS STM32C0xx, CMSIS STM32C031xx |

---

### 💡 Implementation Versions

#### **Version 1: State Machine & Button Control**

Uses the onboard user button to cycle through brightness levels. A software state machine tracks button presses to calculate the corresponding PWM duty cycle.
* **Input:** PC13 (B1 USER Button)
* **Output:** PB4 (External LED Receiving PWM Signal)

<img width="834" height="865" alt="Nucleo PWM State Machine" src="https://github.com/user-attachments/assets/753405b9-5da3-46ff-b683-8a37a8ab2739" />


#### **Version 2: ADC & Potentiometer Control**
Utilizes the MCU's Analog-to-Digital Converter (ADC) to convert a variable voltage from a potentiometer into real-time LED brightness levels.
* **Input:** PA0 (Analog Input Reading From Potentiometer)
* **Output:** PB4 (External LED Receiving PWM Signal)

<img width="950" height="656" alt="Nucleo PWM ADC" src="https://github.com/user-attachments/assets/bf8740a5-2594-42c0-a69d-38c18d28b5d9" />


---

### 🚀 How to Run

1. **Import:** Open either the `PWM_Ver1` or `PWM_Ver2` folder as an STM32CubeIDE project.
2. **Configure:** (Optional) Adjust error logging and indicator settings in `error_check_utilities.h`.
3. **Build & Flash:** Select **Project > Build**, then use the **Run** or **Debug** icons.
4. **Monitor:**
    * Open your serial terminal at **9600 Baud**.
    * **Ver 1:** Press the button to cycle brightness; observe terminal feedback.
    * **Ver 2:** Rotate the potentiometer; observe smooth dimming and real-time ADC data.
