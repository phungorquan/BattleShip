#include "LCDST7920.h"

int count_index = 0; // count buffer array when user display with Basic display mode

void delay(long itime){ // itime = 10 = 1ms
         
   long icompTime = 0;

   for(icompTime=itime*1000;icompTime!=0;icompTime--);
}

void TransferData(char data,uint8_t command)  //if command=0 -> Transfer command ... if command=1 -> Transfer data
{
	LCD_RW = 0; // RW
	LCD_RS_DI = command;	// RS(DI)
	delay(2);
	PF->DOUT = data >> 2;	//Set 2 High byte to PF from PA port
	PA->DOUT = data|(PA->DOUT & 0xFF80);		//Set 6 Low byte to PA port and mask to keep the PA8 PA12 PA13 PA14 
	LCD_EN = 1; // EN
	delay(2);
	LCD_EN = 0; // EN
}

void InitLCD_BasicMode(void) //LCD BasicMode init
{
	// Reset LCD
	delay(400); 	
	LCD_Reset = 0;	
	delay(100);	
	LCD_Reset = 1;	
	delay(100);
	
	TransferData(0x30,0x00);  //Extended Function Set :8BIT,RE=0: basic instruction set, G=0 :graphic display OFF
	delay(2);            
	TransferData(0x30,0x00);  //Function Set
	delay(1);             
	TransferData(0x0C,0x00);  //Display Control,D=1,display on
	delay(2);            
	TransferData(0x01,0x00);  //Display Clear
	delay(100);             
	TransferData(0x06,0x00);  //Enry Mode Set
	delay(2);         
}

void Display_BasicMode(char data[])
{
	int i = 0;
  
  while(data[i] != 0x00)
       {
        if(count_index == 0)
          TransferData(0x80,0x00);  //Line0 // count enough 16 characters it will auto move to new line
        else if (count_index == 16)
          TransferData(0x90,0x00);  //Line1 // count enough 32 characters it will auto move to new line
        else if (count_index == 32)
          TransferData(0x88,0x00);  //Line2 // count enough 48 characters it will auto move to new line
        else if (count_index == 48)
          TransferData(0x98,0x00);  //Line3 // count enough 64 characters it will reset count_index
        else if (count_index == 64)
          {
            count_index=0;
            break;
          }
        
        delay(20);	
        TransferData(data[i],0x01);	// Display one by one characters
        i++;
        count_index++;
       }
       count_index = 0;
}

void InitLCD_Graphic(void)           //Graphic display Mode
{
	// Reset LCD
	delay(400); 	
	LCD_Reset = 0;	
	delay(100);	
	LCD_Reset = 1;	
	delay(100);
	
	TransferData(0x30,0x00);  //Extended Function Set DL = 1 before RE = 1 
	delay(2);        
	TransferData(0x01,0x00);  //Display Clear
	delay(100);     
	TransferData(0x36,0x00);  //Extended Function Set RE=1 after set DL = 1
	delay(1);         
	TransferData(0x3E,0x00);  //EXFUNCTION(DL=8BITS,RE=1,G=1) //3E or 36 because don't care bit 3
	delay(2);               
}

void Display_Graphic(uint8_t data[]) // This function i got from producer on the internet
{
   int i,j;
	 int k = 0;
//*******display top half screen
   for(i=0;i<32;i++)              //
    { 
      TransferData((0x80 + i),0x00); //SET   VERTICAL ADDRESS
      TransferData(0x80,0x00);       //SET   HORIZONTAL ADDRESS
      for(j=0;j<16;j++)
       {
         TransferData(data[k],0x01);
         k++;
       }
    }
//*******display bottom half screen
   for(i=0;i<32;i++)              //
    {
      TransferData((0x80 + i),0x00); //SET VERTICAL ADDRESS
      TransferData(0x88,0);       //SET HORIZONTAL ADDRESS
      for(j=0;j<16;j++)
       {
         TransferData(data[k],0x01);
         k++;
       }
    }
}
