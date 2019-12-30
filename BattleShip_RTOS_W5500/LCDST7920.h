#include <stdio.h>
#include "NUC131.h"	// Avoid uint8_t error

// Pins definition
#define LCD_D7	PF5
#define LCD_D6	PF4
#define LCD_D5	PA5
#define LCD_D4	PA4
#define LCD_D3	PA3
#define LCD_D2	PA2
#define LCD_D1	PA1
#define LCD_D0	PA0

#define LCD_Reset	PA9
#define LCD_EN 	PA8
#define LCD_RW 	PA12
#define LCD_RS_DI	PA13

/* Pin connection : LCD <-> NUC131 (Numaker)
	
 D7 D6 : F5 F4
 D5->D0 : A5 -> A0
 EN : A8
 RESET : A9
 RW : A12
 RS(DI) :  A13
 
*/

// TransferData to LCD
void TransferData(char data,uint8_t command);

/**  BASIC MODE GUIDELINE
* Setup LCD in basic mode with basic ASCII from 0x00->0x7F in datasheet (LCD can display 64 characters(8*16) from user array input)
*	You can try with array Basic_Mode_Array[65] above with 2 steps like this:
*	1. InitLCD_BasicMode();
*	2. Display_BasicMode("HELLO THIS IS XIU");
* Then enjoy yourself
**/  
void InitLCD_BasicMode(void); 
void Display_BasicMode(char data[]);

/**  GRAPHIC MODE GUIDELINE
* Setup LCD in graphic mode, you can control one by one pixel in 128*64 pixel on LCD
*	1. InitLCD_Graphic();
*	2. Display_Graphic(BattleMap);
* Then enjoy yourself
**/  
void InitLCD_Graphic(void);
void Display_Graphic(uint8_t data[]);
void delay(long itime);
