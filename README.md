# nes_gamepad_ble
Convert NES gamepad clone to Bluetooth Gamepad
Board used: Lolin ESP32 Lite + 3.7V battery

This Arduino IDE code converts a NES gamepad clone into Bluetooth (BLE).
Uses BleGamepad library - https://github.com/lemmingDev/ESP32-BLE-Gamepad

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
            
