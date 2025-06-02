/*!
 * @file TM1629A.h
 * @brief Arduino library for interface with TM1629A . 
 * @n read buttons (untested), switch leds (untested), display on 14 segment (only tested on 10 of them).
 * @author Mauro de Amorim maurodeamorim1 gmail com
 * @version  V0.1
 * @date  2025-05-30
 */

#ifndef TM1629A_h
#define TM1629A_h
#include "Arduino.h"

#ifndef ON
#define ON 1
#endif
#ifndef OFF
#define OFF 0
#endif

    typedef enum{
      PULSE1_16,
      PULSE2_16,
      PULSE4_16,
      PULSE10_16,
      PULSE11_16,
      PULSE12_16,
      PULSE13_16,
      PULSE14_16
    } pulse_t;

class TM1629A{
  private:
    #define INSTRUCTION_WRITE_DATA 0x40
    #define INSTRUCTION_READ_KEY 0x42
    #define INSTRUCTION_ADDRESS_AUTO 0x40
    #define INSTRUCTION_ADDRESS_FIXED 0x44
    #define INSTRUCTION_NORMAL_MODE 0x40
    #define INSTRUCTION_TEST_MODE 0x48

    #define FIRST_DISPLAY_ADDRESS 0xC0

    #define DISPLAY_TURN_OFF 0x80
    #define DISPLAY_TURN_ON 0x88

    uint8_t _clk_pin;
    uint8_t _stb_pin;
    uint8_t _dio_pin;
    uint8_t _buttons;
    uint8_t _pulse;
    bool _isOn;
    uint8_t _nDigitsUsed;
    uint8_t _digits;  // Número de dígitos no display

  public:
    TM1629A(uint8_t dio_pin, uint8_t clk_pin, uint8_t stb_pin, uint8_t nDigitsUsed, bool activateDisplay=true, byte intensity=7){
      _clk_pin = clk_pin;
      _stb_pin = stb_pin;
      _dio_pin = dio_pin;
      _pulse = PULSE1_16;
      _isOn = false;
      _nDigitsUsed = nDigitsUsed;
      _digits = nDigitsUsed;

      pinMode(stb_pin, OUTPUT);
      pinMode(clk_pin, OUTPUT);
      pinMode(dio_pin, OUTPUT);
      digitalWrite(stb_pin, HIGH);
      digitalWrite(clk_pin, HIGH);
      digitalWrite(dio_pin, HIGH);  
    
    }
    /**
    * @fn getCharMap
    * @brief Retorna o valor binário do caractere desejado 
    */
    uint16_t getCharMap(char c);

    /**
    * @fn sendCommand
    * @brief executa comandos 
    */   
    void sendCommand(uint8_t cmd);

    /**
    * @fn sendData
    * @brief envia dados 
    */ 
    void sendData(uint8_t address, uint16_t data);

    /**
    * @fn clearDisplay
    * @brief limpa o display
    */
    void clearDisplay();

    /**
    * @fn printString
    * @brief escreve no display
    */
    void printString(const char* str, uint16_t delayTime = 400);

    /**
    * @fn blinkChar
    * @brief pisca caracter no display
    */
    void blinkChar(char c, uint8_t pos, uint8_t times = 3, uint16_t delayTime = 250);

    /**
    * @fn typeWriter
    * @brief 
    */    
    void typeWriter(const char* str, uint16_t delayTime = 300);

    /**
    * @fn scrollPingPong
    * @brief rolaq testo no display
    */
    void scrollPingPong(const char* str, uint16_t delayTime = 400);

    /**
    * @fn fadeText
    * @brief fade testo no display
    */ 
    void fadeText(const char* str, uint8_t steps = 5, uint16_t delayTime = 100);

    /**
    * @fn displayTurnOff
    * @brief turn on lights
    */
    void displayTurnOff();

    /**
    * @fn displayTurnOn
    * @brief turn off lights
    */
    void displayTurnOn();
    /**
    * @fn writeLed
    * @brief put led ON or OFF
    * @param num num of led(1-8)
    * @param state (true or false)
    */
    void writeLed(uint8_t num, bool state);
    /**
    * @fn writeLeds
    * @brief set all 8 leds ON or OFF
    * @param val 8bits
    */
    void writeLeds(uint8_t val);
    /**
    * @fn displaySetBrightness
    * @brief set display brightness
    * @param pulse_t (0-7)
    */
    void displaySetBrightness(pulse_t pulse);
    /**
    * @fn reset
    * @brief switch off all displays-leds
    */
    void reset();
    /**
    * @fn test
    * @brief blink all displays and leds
    */
    void test();

    /**
    * @fn displayDig
    * @brief set 7 segment display + dot
    * @param digitId num of digit(0-7)
    * @param val value 8 bits
    */
    void displayDig(uint8_t digitId, uint8_t pgfedcba);

    /**
    * @fn displayClear
    * @brief switch off all leds and segment display
    */
    void displayClear();

private:
    void writeData(uint8_t data);
    void writeDataAt(uint8_t displayAddress, uint8_t data);
    void setDisplayMode(uint8_t displayMode);
    void setDataInstruction(uint8_t dataInstruction);
};
#endif
