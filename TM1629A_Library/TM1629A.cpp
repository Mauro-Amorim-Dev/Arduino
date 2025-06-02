#include "Arduino.h"
#include "TM1629A.h"

void TM1629A::sendCommand(uint8_t cmd) {
  digitalWrite(_stb_pin, LOW);
  shiftOut(_dio_pin, _clk_pin, LSBFIRST, cmd);
  digitalWrite(_stb_pin, HIGH);
}

void TM1629A::sendData(uint8_t address, uint16_t data) {
  digitalWrite(_stb_pin, LOW);
  shiftOut(_dio_pin, _clk_pin, LSBFIRST, 0xC0 | address);
  shiftOut(_dio_pin, _clk_pin, LSBFIRST, data & 0xFF);
  shiftOut(_dio_pin, _clk_pin, LSBFIRST, data >> 8);
  digitalWrite(_stb_pin, HIGH);
}

void TM1629A::clearDisplay() {
  for (uint8_t i = 0; i < _digits; i++) {
    sendData(i * 2, 0x0000);
  }
}

void TM1629A::printString(const char* str, uint16_t delayTime = 400) {
  size_t len = strlen(str);

  if (len <= _digits) {
    // Mostra direto se couber
    for (uint8_t i = 0; i < _digits; i++) {
      char c = i < len ? toupper(str[i]) : ' ';
      sendData(i * 2, getCharMap(c));
    }
  } else {
    // Rola o texto
    for (uint8_t offset = 0; offset <= len - _digits; offset++) {
      for (uint8_t i = 0; i < _digits; i++) {
        char c = toupper(str[offset + i]);
        sendData(i * 2, getCharMap(c));
      }
      delay(delayTime);
    }
  }
}

//Piscar por Caractere (char blink)
//Cada letra do texto pisca individualmente, tipo efeito digitando senha ou cursor digital.
void TM1629A::blinkChar(char c, uint8_t pos, uint8_t times = 3, uint16_t delayTime = 250) {
  for (uint8_t i = 0; i < times; i++) {
    sendData(pos * 2, getCharMap(c));
    delay(delayTime);
    sendData(pos * 2, 0x0000);
    delay(delayTime);
  }
}

//Máquina de Escrever (typewriter)
//Escreve uma string no display um caractere por vez com delay e preenchimento da esquerda para a direita.
void TM1629A::typeWriter(const char* str, uint16_t delayTime = 300) {
  clearDisplay();
  size_t len = strlen(str);
  for (uint8_t i = 0; i < _digits && i < len; i++) {
    char c = toupper(str[i]);
    sendData(i * 2, getCharMap(c));
    delay(delayTime);
  }
}

//Loop Reverso (Ping-Pong)
//Texto rola até o fim e depois volta revertendo a direção, como uma onda digital.
void TM1629A::scrollPingPong(const char* str, uint16_t delayTime = 400) {
  size_t len = strlen(str);
  if (len <= _digits) {
    printString(str);
    return;
  }

  // Scroll normal
  for (uint8_t offset = 0; offset <= len - _digits; offset++) {
    for (uint8_t i = 0; i < _digits; i++) {
      char c = toupper(str[offset + i]);
      sendData(i * 2, getCharMap(c));
    }
    delay(delayTime);
  }

  // Scroll reverso
  for (int offset = len - _digits - 1; offset >= 0; offset--) {
    for (uint8_t i = 0; i < _digits; i++) {
      char c = toupper(str[offset + i]);
      sendData(i * 2, getCharMap(c));
    }
    delay(delayTime);
  }
}

//Simulação de Fade (brilho dinâmico com buffer)
//O TM1629A não tem PWM real por canal, mas podemos simular fade com “on-off buffer alternado”:
void TM1629A::fadeText(const char* str, uint8_t steps = 5, uint16_t delayTime = 100) {
  // Exibe a string inteira
  for (uint8_t i = 0; i < _digits; i++) {
    char c = toupper(str[i]);
    sendData(i * 2, getCharMap(c));
  }

  // Simula fade-out e fade-in
  for (int i = 0; i < steps; i++) {
    clearDisplay();
    delay(delayTime);
    for (uint8_t i = 0; i < _digits; i++) {
      char c = toupper(str[i]);
      sendData(i * 2, getCharMap(c));
    }
    delay(delayTime);
  }
}

// Mapeamento getCharMap completo gerado anteriormente (colado aqui por brevidade)
/* 
 *  Mapeamento getCharMap completo 
 *  Retorna o valor binário do caractere desejado
*/
/*
Definition for the displayable ASCII chars on a 15-segment display.

Segment labels and bit-assignment: 2X5241AO DISPLAY
   |---A---|          
   |\  |  /|          
   F H J K B          
   |  \|/  |          
   |-G1-G2-|          
   |  /|\  |          
   E N M L C          
   |/  |  \|         
   |---D---| DP      

 8  5 4 2 14 15 17 6 13 18 1 7 9 10 12
DP G1 L M  N G2  K J  H  F E D C  B  A

Tm1629A → Display 2x5241AO  
SEG1    → Pino 12 (A)  
SEG2    → Pino 10 (B)  
SEG3    → Pino 9  (C)  
SEG4    → Pino 7  (D)  
SEG5    → Pino 1  (E)  
SEG6    → Pino 18 (F)  
SEG7    → Pino 13 (H)  
SEG8    → Pino 6  (J)  
SEG9    → Pino 17 (K)  
SEG10   → Pino 15 (G2)  
SEG11   → Pino 14 (N)  
SEG12   → Pino 2  (M)  
SEG13   → Pino 4  (L)  
SEG14   → Pino 5  (G1)  
SEG15   → Pino 8  (DP) 

 

Note: For compatibility bits 0-7 are in the same order as A-G on a 7-segment display.
      Most characters in the font below are based on their 7-seg counterparts to keep styling alike.
*/
uint16_t TM1629A::getCharMap(char c) {
  switch (toupper(c)) {
     case '0': return 0b0000110000111111; 
     case '1': return 0b0000010000000110;
     case '2': return 0b0000000011011011;
     case '3': return 0b0000000010001111;
     case '4': return 0b0000000011100110;
     case '5': return 0b0000000011101101;
     case '6': return 0b0000000011111101;
     case '7': return 0b0000000000000111;
     case '8': return 0b0000000011111111;
     case '9': return 0b0000000011101111;
     case 'A': return 0b0000000011110111;
     case 'B': return 0b0001001011001111;
     case 'C': return 0b0000000000111001;
     case 'D': return 0b0001001000001111;
     case 'E': return 0b0000000011111001;
     case 'F': return 0b0000000001110001;
     case 'G': return 0b0000000010111101;
     case 'H': return 0b0000000011110110;
     case 'I': return 0b0001001000001001;
     case 'J': return 0b0000000000011111;
     case 'K': return 0b0010010001110000;
     case 'L': return 0b0000000000111000;
     case 'M': return 0b0000010100110110;
     case 'N': return 0b0010000100110110;
     case 'O': return 0b0000000000111111;
     case 'P': return 0b0000000011110011;
     case 'Q': return 0b0010000000111111;
     case 'R': return 0b0010000010110011;
     case 'S': return 0b0000000011101101;
     case 'T': return 0b0001001000000001;
     case 'U': return 0b0000000000111110;
     case 'V': return 0b0000110000110000;
     case 'W': return 0b0010100000110110;
     case 'X': return 0b0010110100000000;
     case 'Y': return 0b0001010100000000;
     case 'Z': return 0b0000110000001001;
     case ' ': return 0b0000000000000000;
     case '-': return 0b0000000011000000;
     case '+': return 0b0001001011000000;
     case '_': return 0b0000000000001000;
     case '=': return 0b0000000011001000;
     case '*': return 0b0011111111000000;
     case '/': return 0b0000110000000000;
     case '<': return 0b0010010010000110;
     case '>': return 0b0000100101110000;     
      default:  return 0x0000; // Espaço ou caractere não definido
  }
}

//incluidas ************************************************************************************

void TM1629A::reset(){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_AUTO);
  digitalWrite(_stb_pin, LOW);
  writeData(FIRST_DISPLAY_ADDRESS);
  for(uint8_t i=0;i<16;i++)
    writeData(0);
  digitalWrite(_stb_pin, HIGH);
}

void TM1629A::displayDig(uint8_t digitId, uint8_t pgfedcba){
  if (digitId>15) return;
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_FIXED);
  writeDataAt(FIRST_DISPLAY_ADDRESS+14-digitId, pgfedcba);
}

void TM1629A::displayClear(){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA | INSTRUCTION_ADDRESS_FIXED);
  for (uint8_t i=0;i<15;i+=2){
    writeDataAt(FIRST_DISPLAY_ADDRESS+i,0x00);
  }
}

void TM1629A::writeLed(uint8_t num,bool state){
  if (num<1 |num>8) return;
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA | INSTRUCTION_ADDRESS_FIXED);
  writeDataAt(FIRST_DISPLAY_ADDRESS + (num*2-1), state);
}

void TM1629A::writeLeds(uint8_t val){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA | INSTRUCTION_ADDRESS_FIXED);
  for(uint8_t i=1;i<9;i++){
    writeDataAt(FIRST_DISPLAY_ADDRESS + (i*2-1), val & 0x01);
    val >>= 1; 
  }
}

void TM1629A::displayTurnOn(){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  _isOn = true;
}


void TM1629A::displayTurnOff(){
  setDisplayMode(DISPLAY_TURN_OFF | _pulse);
  _isOn = false;
}


void TM1629A::displaySetBrightness(pulse_t newpulse){
  if (newpulse<PULSE1_16 | newpulse>PULSE14_16) return;
  _pulse = newpulse;
  uint8_t data = (_isOn) ? DISPLAY_TURN_ON : DISPLAY_TURN_OFF;
  data |= _pulse;
  setDisplayMode(data);
}


void TM1629A::writeData(uint8_t data){
  shiftOut(_dio_pin,_clk_pin,LSBFIRST,data);
} 

void TM1629A::writeDataAt(uint8_t displayAddress, uint8_t data){
    digitalWrite(_stb_pin, LOW);
    writeData(displayAddress);
    writeData(data);
    digitalWrite(_stb_pin, HIGH);
    delayMicroseconds(1);
}

void TM1629A::setDisplayMode(uint8_t displayMode){
  digitalWrite(_stb_pin, LOW);
  writeData(displayMode);
  digitalWrite(_stb_pin, HIGH);
  delayMicroseconds(1);
}
void TM1629A::setDataInstruction(uint8_t dataInstruction){
  digitalWrite(_stb_pin, LOW);
  writeData(dataInstruction);
  digitalWrite(_stb_pin, HIGH);
  delayMicroseconds(1);  
}

void TM1629A::test(){
  uint8_t val=0;
  for(uint8_t i=0;i<5;i++){
    setDisplayMode(DISPLAY_TURN_ON | _pulse);
    setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_AUTO);
    digitalWrite(_stb_pin, LOW);
    writeData(FIRST_DISPLAY_ADDRESS);
    for(uint8_t i=0;i<16;i++)
      writeData(val);
    digitalWrite(_stb_pin, HIGH);
    delay(1000);
    val = ~val;
  }
}

