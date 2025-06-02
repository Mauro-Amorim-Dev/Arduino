#define DIN_PIN 7
#define CLK_PIN 8
#define STB_PIN 9
#define NUM_DIG 4 //Display Alphanumeric 2x5241AO

#include <TM1629A.h>

const uint8_t DIGITS = 4;  // Número de dígitos no display
// DIN,CLOCK,STB,NUM_DIG Display Alphanumeric 2x5241AO
TM1629A TM(DIN_PIN, CLK_PIN, STB_PIN,NUM_DIG);

void setup() {
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(STB_PIN, OUTPUT);
  
  TM.sendCommand(0x88); // Display ON + brilho médio

  TM.clearDisplay();
}

void loop() {
  TM.printString("MDAWEB WITH TM1629A AND DISPLAY 5241AO");
  delay(1000);
  TM.printString("SCROLLING AND EFFECTS");
  delay(1000);
  TM.clearDisplay();
 // Piscar cada caractere individualmente
  TM.blinkChar('A', 0);
  TM.blinkChar('B', 1);
  TM.blinkChar('C', 2);
  TM.blinkChar('D', 3);
  delay(500);
  TM.clearDisplay();
  // Efeito máquina de escrever
  TM.typeWriter("DEMO");
  delay(1000);

  // Ping-pong scroll
  TM.scrollPingPong("ROLLING BOTH WAYS");
  delay(1000);

  // Simula fade
  TM.fadeText("FADE");
  delay(1000);
  
  // Mostra "GAME"
  TM.sendData(0, TM.getCharMap('G'));
  TM.sendData(2, TM.getCharMap('A'));
  TM.sendData(4, TM.getCharMap('M'));
  TM.sendData(6, TM.getCharMap('E'));
  delay(1000);
}
