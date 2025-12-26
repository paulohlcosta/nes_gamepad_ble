# nes_gamepad_ble
Convert NES gamepad clone to Bluetooth Gamepad
Board used: Lolin ESP32 Lite + 3.7V battery

This Arduino IDE code converts a NES gamepad clone into Bluetooth (BLE).

Video where I learned how to read the pressed buttons - https://www.youtube.com/watch?v=luMPAOVm8Us


References:

Button number / Physical reference
 * 1 DPAD RIGHT
 * 2 DPAD LEFT
 * 3 DPAD DOWN 
 * 4 DPAD UP
 * 5 START
 * 6 SELECT
 * 7 BUTTON B
 * 8 BUTTON A


DB9 connector pin / wire / function

                   black         green           white
     NC            CLOCK         LATCH           DATA          NC
     O             O              O              O              O
     
                         yellow                         red
           NC            GND            NC              VCC        
           O             O              O                O
            


# NES to Bluetooth Keyboard (ESP32)

This project uses an **ESP32** to convert signals from an original (or clone) **NES (Nintendo Entertainment System)** controller into Bluetooth Keyboard commands (BLE). 

The firmware reads the NES controller's shift register and maps each button to specific keyboard keys, allowing you to play emulators on PC, Mac, Android, or iOS wirelessly.

## 🚀 Features

* **Bluetooth Low Energy (BLE):** The ESP32 is recognized as a standard HID Keyboard.
* **Low Latency:** Optimized bit-banging for reading Clock, Latch, and Data pins.
* **State Machine Logic:** Robust handling of `IDLE`, `PRESSED`, and `RELEASED` states to ensure precise inputs without ghosting.
* **Visual Feedback:** Built-in LED (GPIO 22) indicates connection status.

## 🕹️ Key Mapping

| NES Button | Keyboard Key |
| :--- | :--- |
| **A** | Right Control |
| **B** | Right Shift |
| **SELECT** | Enter |
| **START** | Escape |
| **D-PAD UP** | Up Arrow |
| **D-PAD DOWN** | Down Arrow |
| **D-PAD LEFT** | Left Arrow |
| **D-PAD RIGHT** | Right Arrow |

## 🛠️ Wiring Diagram (Pinout)

Connect your NES controller to the ESP32 using the following pins as defined in the source code:

| NES Function | Wire Color (Typical) | ESP32 Pin (GPIO) |
| :--- | :--- | :--- |
| **VCC (3.3V - 5V)** | White | 3.3V |
| **GND** | Brown | GND |
| **Latch** | Orange | GPIO 17 |
| **Clock** | Yellow | GPIO 18 |
| **Data** | Red | GPIO 19 |

> **Note:** Wire colors may vary depending on whether you are using an original controller or a third-party clone. Always verify the pinout with a multimeter if possible.

## 💻 Prerequisites

1.  **Arduino IDE** configured for ESP32 boards.
2.  **ESP32-BLE-Keyboard Library:** You must install the [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard) library for the code to compile.
3.  **Board Manager:** Ensure you have the latest ESP32 core installed in your IDE.

## 📝 How to Use

1.  Clone this repository or copy the code into a new Arduino sketch.
2.  Upload the code to your ESP32.
3.  Power the ESP32. The built-in LED will stay **OFF** (Logic: Low) when disconnected.
4.  On your PC/Smartphone, search for Bluetooth devices and pair with **"NES BLE Keyboard"**.
5.  Once connected, the LED will turn **ON** (Logic: High), indicating the device is ready.
6.  Open your favorite emulator and the keys will be mapped automatically.

## 📑 Changelog

* **v1.1:** Removed redundant `sendReport()` calls to improve stability.
* **v1.2:** Added serial debug functions and improved state handling for directional inputs.
* **v1.3:** Switched from Gamepad HID to Keyboard HID for universal compatibility and better reading accuracy.

---
**Developed by:** Paulo
