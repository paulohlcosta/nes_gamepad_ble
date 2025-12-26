/* CONVERTER SINAIS DO NES nes CLONE PARA um byte
 * INTERPRETAR ESSE BYTE
 * ENVIAR COMNADOS DE KEYBOARD PELA LIBRARY BLEKEYBOARD
 *           
 * 1 DPAD DIREITA    ultimo lido
 * 2 DPAD ESQUERDA
 * 3 DPAD BAIXO 
 * 4 DPAD CIMA
 * 5 START
 * 6 SELECT
 * 7 BOTAO B
 * 8 BOTAO A     primeiro lido      
 *   
 * CHANGELOG
 * 1.1 remove bleNES.sendReport() que eu acredito estar em redundancia com press e release.
 *   -> nao funcionou, ele conecta mas nao recebe os sinais dos botoes
 * 1.2 em testes (PROBLEMAS NA LEITURA)
 *      tratamento dos direcionais como eixo X e Y  <
 *      criado funcao DEBUG com saida serial
 *      criado funcoes separadas para leitura dos botoes e envio BLE
 * 1.3 alterado de Gamepad para keyboard - LEITURA OK
 */

#define LED_BUILTIN 22
#define nesClock    18      //       NES controller clock pin2
#define nesLatch    17      //       NES controller latch pin3
#define nesData     19      //       NES controller data pin4

#include <BleKeyboard.h>
BleKeyboard bleKeyboard("NES BLE Keyboard", "Paulo ESP32 Lite", 100);

byte nesRegister;            // NES controller button states. 0=pressed 1=released
const int shiftDelay  = 20;  // delay in microseconds to help with shift register setup/hold timing

enum ButtonState { IDLE, PRESSED, RELEASED };

ButtonState buttonUpState = IDLE;
ButtonState buttonDownState = IDLE;
ButtonState buttonLeftState = IDLE;
ButtonState buttonRightState = IDLE;
ButtonState buttonAState = IDLE;
ButtonState buttonBState = IDLE;
ButtonState buttonStartState = IDLE;
ButtonState buttonSelectState = IDLE;

void setup() {
  //Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); digitalWrite(LED_BUILTIN, LOW);
  pinMode(nesData, INPUT);
  pinMode(nesClock, OUTPUT);
  pinMode(nesLatch, OUTPUT);
  digitalWrite(nesClock, LOW);
  digitalWrite(nesLatch, LOW);

  bleKeyboard.begin();

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if (bleKeyboard.isConnected()) {
    nesRegister = readNesController();
    digitalWrite(LED_BUILTIN, HIGH); //LED fica apagado quando conectado
    handleButton(0, KEY_RIGHT_CTRL, buttonAState, "(A)CTRL"); // ---------------------- nao le
    handleButton(1, KEY_RIGHT_SHIFT, buttonBState, "(B)SHIFT");
    handleButton(2, KEY_RETURN, buttonSelectState, "(select)ENTER");
    handleButton(3, KEY_ESC, buttonStartState, "(start)ESCAPE");
    handleButton(4, KEY_UP_ARROW, buttonUpState, "UP");
    handleButton(5, KEY_DOWN_ARROW, buttonDownState, "DOWN");
    handleButton(6, KEY_LEFT_ARROW, buttonLeftState, "LEFT");
    handleButton(7, KEY_RIGHT_ARROW, buttonRightState, "RIGHT");
    delayMicroseconds(shiftDelay * 10); // Small delay to avoid excessive reading
  } 
  else {digitalWrite(LED_BUILTIN, LOW);}  //LED fica aceso quando desconectado
}

byte readNesController() {
  byte tempData = 255;

  digitalWrite(nesLatch, HIGH); // pulsar LATCH
  delayMicroseconds(10);
  if (digitalRead(nesData) == LOW) {bitClear(tempData, 0);}     // o primeiro lido é (A)CTRL
  digitalWrite(nesLatch, LOW);
  delayMicroseconds(10);

  for (int i = 1; i < 8; i++) {
    digitalWrite(nesClock, HIGH); // pulsar CLOCK
    delayMicroseconds(shiftDelay/10);
    digitalWrite(nesClock, LOW);
    delayMicroseconds(shiftDelay);

    if (digitalRead(nesData) == LOW) {bitClear(tempData, i);}     // lê B, select, start, up, down, left, right
   }

  return tempData;
}
  
void handleButton(int buttonBit, uint8_t key, ButtonState &state, const char* keyName) {
  switch (state) {
    case IDLE:
      if (bitRead(nesRegister, buttonBit) == 0) {
        bleKeyboard.press(key);  // alterar estado de IDLE para PRESSED
        state = PRESSED;
      }
      break;
    case PRESSED:
      if (bitRead(nesRegister, buttonBit) == 1) {
        bleKeyboard.release(key);
        state = RELEASED;
      }
      break;
    case RELEASED:
      if (bitRead(nesRegister, buttonBit) == 0) {
        state = PRESSED;
      } else {
        state = IDLE;  // alterar estado de RELEASED para IDLE
      }
      break;
  }
}
