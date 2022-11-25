/* CONVERTER SINAIS DO GAMEPAD nes CLONE PARA um byte
 * INTERPRETAR ESSE BYTE PELA LIBRARY DO BLEGAMEPAD E ENVIAR OS COMANDOS
 * 
 *                  preto         verde           branco
 *     NC           CLOCK         LATCH           DATA          NC
 *     O            O              O              O              O
 *     
 *                        amarelo                         vermelho
 *           NC           GND             NC              5V        
 *           O             O              O                O
 *           
 *  OLHANDO A PCB POR CIMA, ONDE TEM AS SOLDAS DOS FIOS, A SEQUENCIA SEGUE:        
 *   | VCC | GROUND | LATCH | CLOCK | DATA |       
 *           
 *           
 * 1 DPAD DIREITA
 * 2 DPAD ESQUERDA
 * 3 DPAD BAIXO 
 * 4 DPAD CIMA
 * 5 START
 * 6 SELECT
 * 7 BOTAO B
 * 8 BOTAO A           
 *           
 *           
 * THANKS TO https://www.youtube.com/watch?v=luMPAOVm8Us
 * https://github.com/lemmingDev/ESP32-BLE-Gamepad
 */

#include <Arduino.h>
#include <BleGamepad.h>
BleGamepad bleGamepad;
#define numOfButtons 8
#define LED_BUILTIN 22
byte previousButtonStates[numOfButtons];
byte currentButtonStates[numOfButtons];


const int shiftDelay  = 20;  // delay in microseconds to help with shift register setup/hold timing
int GAMEPADRegister;            // GAMEPAD controller button states. 0=pressed 1=released
//int GAMEPADRegister2;    //necessario somente se tiver mais de 8 botoes
int GAMEPADClock      = 18;      //       GAMEPAD controller clock pin2
int GAMEPADLatch      = 17;      //       GAMEPAD controller latch pin3
int GAMEPADData       = 19;      //       GAMEPAD controller data pin4


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); digitalWrite(LED_BUILTIN, LOW);
    for (byte currentPinIndex = 0; currentPinIndex < numOfButtons; currentPinIndex++)
    {
        previousButtonStates[currentPinIndex] = HIGH;
        currentButtonStates[currentPinIndex] = HIGH;
    }
    BleGamepadConfiguration bleGamepadConfig;
    bleGamepadConfig.setAutoReport(false);
    bleGamepadConfig.setButtonCount(numOfButtons);
    bleGamepad.begin(&bleGamepadConfig);
    // changing bleGamepadConfig after the begin function has no effect, unless you call the begin function again

  // configure pins
  pinMode(GAMEPADData, INPUT);
  pinMode(GAMEPADClock, OUTPUT);
  pinMode(GAMEPADLatch, OUTPUT);

  digitalWrite(GAMEPADClock, LOW);      // GAMEPAD control lines idle low
  digitalWrite(GAMEPADLatch, LOW);

//  Serial.begin(115200);  Serial.println("Starting work!");
//  delay(1200);  

  digitalWrite(LED_BUILTIN, HIGH);
}


void loop()
{
  // read the 12 controller buttons into the GAMEPADRegister
  // GAMEPAD controller button states are asynchronously loaded into the shift register while GAMEPADLatch is high
  // When GAMEPADLatch goes low, the first data bit is shifted to GAMEPADData
  // Button data is shifted to GAMEPADData on each low to high transition of GAMEPADClock
 
  digitalWrite(GAMEPADLatch, HIGH);       // while the latch pin is high, 
  delayMicroseconds(shiftDelay);       // impose a delay to allow a setup and hold time
  digitalWrite(GAMEPADLatch, LOW);  


//-----LEITURA DO BYTE NEGATIVO
  for (int x = 0; x <= 7; x++) {                    // read in the *8* controller buttons that were latched
    bitWrite(GAMEPADRegister, x, !digitalRead(GAMEPADData)); // store the current button state on the data input into the correct GAMEPADRegister position
    digitalWrite(GAMEPADClock, HIGH);                   // generate a clock pulse to bring the next button to the data input
    delayMicroseconds(shiftDelay);       
    digitalWrite(GAMEPADClock, LOW);                    
  }

/*-----LEITURA DO BYTE POSITIVO --------------- nao eh necessario quando tem ate 8 botoes
  for (int x = 0; x <= 7; x++) {                    // read in the *16* controller buttons that were latched
    bitWrite(GAMEPADRegister2, x, digitalRead(GAMEPADData)); // store the current button state on the data input into the correct GAMEPADRegister position
    digitalWrite(GAMEPADClock, HIGH);                   // generate a clock pulse to bring the next button to the data input
    delayMicroseconds(shiftDelay);       
    digitalWrite(GAMEPADClock, LOW);                    
  }*/

 //termina a etapa de leitura dos botoes fisicos
 //aqui eu tenho uma variavel GAMEPADRegister com os status dos botoes
 
/*  Serial.println(":-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:");
  Serial.println("leitura:");
  Serial.println(byte(GAMEPADRegister), BIN);
  Serial.println(":");  //-----------------------------------------------*/

    if (bleGamepad.isConnected())
    {
        digitalWrite(LED_BUILTIN, HIGH); //LED fica apagado quando conectado
        
        byte GAMEPADRegisterByte = byte(GAMEPADRegister);  //a partir daqui vou converter INT para BYTE
        for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++)
        {
        currentButtonStates[currentIndex] = bitRead(GAMEPADRegisterByte, 7-currentIndex);
//        Serial.print(currentButtonStates[currentIndex], BIN);

            if (currentButtonStates[currentIndex] != previousButtonStates[currentIndex])
            {
                if (currentButtonStates[currentIndex] == HIGH)
                {
                    bleGamepad.press(currentIndex+1);
                }
                else
                {
                    bleGamepad.release(currentIndex+1);
                }
            }
        }

        if (currentButtonStates != previousButtonStates)
        {
            for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++)
            {
                previousButtonStates[currentIndex] = currentButtonStates[currentIndex];
            }

            bleGamepad.sendReport();
        }

        delay(20);
    }
    else {digitalWrite(LED_BUILTIN, LOW);}
}
