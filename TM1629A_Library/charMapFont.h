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
#ifndef charMapFont_h
#define charMapFont_h

uint16_t getCharMap(char c) {
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
      default:  return 0x0000; // Espaço ou caractere não definido
  }
}
#endif
