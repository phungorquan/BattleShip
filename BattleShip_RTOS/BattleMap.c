#include "BattleMap.h"	// Get x,y defines
#include "LCDST7920.h"	// Get LCD functions

/* Arr of X and Y position selection*/
// ########## X #############
uint8_t X0[6] =
{
	0x40,0x0E,
	0x40,0x0A,
	0x40,0x0E
};

uint8_t X1[] =
{
	0x40,0x02,
	0x40,0x02,
	0x40,0x02
};

uint8_t X2[] =
{
	0x40,0x0A,
	0x40,0x0A,
	0x40,0x0A
};

uint8_t X3[] =
{
	0x40,0x2A,
	0x40,0x2A,
	0x40,0x2A
};
uint8_t X4[] = 
{
	0x40,0x2A,
	0x40,0x2A,
	0x40,0x12
};
uint8_t X5[] =
{
	0x40,0x0A,
	0x40,0x0A,
	0x40,0x04
};
uint8_t X6[] =
{
	0x40,0x2A,
	0x40,0x2A,
	0x40,0x12
};

uint8_t X7[] =
{
	0x40,0xAA,
	0x40,0xAA,
	0x40,0x4A
};

uint8_t X8[] =
{
	0x42,0xAA,
	0x42,0xAA,
	0x41,0x2A
};

uint8_t X9[] =
{
	0x40,0x2A,
	0x40,0x24,
	0x40,0x2A
};


// ########## Y #############
uint8_t Y0[] =
{
	0x70,0x02,
	0x50,0x02,
	0x70,0x02
};

uint8_t Y1[] =
{
	0x40,0x02,
	0x40,0x02,
	0x40,0x02
};

uint8_t Y2[] =
{
	0x50,0x02,
	0x50,0x02,
	0x50,0x02
};

uint8_t Y3[] =
{
	0x54,0x02,
	0x54,0x02,
	0x54,0x02
};
uint8_t Y4[] = 
{
	0x54,0x02,
	0x54,0x02,
	0x48,0x02
};
uint8_t Y5[] =
{
	0x50,0x02,
	0x50,0x02,
	0x20,0x02
};
uint8_t Y6[] =
{
	0x54,0x02,
	0x54,0x02,
	0x24,0x02
};

uint8_t Y7[] =
{
	0x55,0x02,
	0x55,0x02,
	0x25,0x02
};

uint8_t Y8[] =
{
	0x55,0x42,
	0x55,0x42,
	0x25,0x42
};

uint8_t Y9[] =
{
	0x54,0x02,
	0x48,0x02,
	0x54,0x02
};

/*Arr of "SETTING SHIP X" , "1" , "2" , "3" , "/" and X = "1" , "2" , "3" */

uint8_t SETTING_SHIP[] =
{
	0xEE,0xEE,0xA2,0xF3,0xAA,0xE0,
	0xEE,0xEE,0xB2,0xF3,0xAA,0xE0,
	0x88,0x44,0xB2,0x82,0x2A,0xA0,
	0x88,0x44,0xBA,0x82,0x2A,0xA0,
	0xEE,0x44,0xBA,0x83,0xBA,0xA0,
	0xEE,0x44,0xAE,0xB3,0xBA,0xE0,
	0x28,0x44,0xAE,0xB0,0xAA,0xE0,
	0x28,0x44,0xA6,0x90,0xAA,0x80,
	0xEE,0x44,0xA6,0xF3,0xAA,0x80,
	0xEE,0x44,0xA2,0xF3,0xAA,0x80
};

uint8_t ONE[] =
{
	0x18,0x18,0x38,0x38,0x08,
	0x08,0x08,0x08,0x08,0x08,
	0x08,0x3C,0x3C,0x3C
};

uint8_t TWO[] =
{
	0x3C,0x3C,0x3C,0x04,0x04,
	0x3C,0x3C,0x3C,0x20,0x20,
	0x20,0x3C,0x3C,0x3C
};

uint8_t THREE[] =
{
	0x3C,0x3C,0x3C,0x04,0x04,
	0x3C,0x3C,0x3C,0x04,0x04,
	0x04,0x3C,0x3C,0x3C
};

uint8_t SLASH[] = 
{
	0x03,0x03,0x03,0x0F,0x0F,0x0F,0x0F,
	0x3C,0x3C,0x3C,0x3C,0xF0,0xF0,0xF0,
	0xF0,0xC0,0xC0,0xC0
};

uint8_t ONE_SETTING[] =
{
	0x02,0x02,0x06,0x06,0x02,0x02,0x02,0x02,0x07,0x07
};

uint8_t TWO_SETTING[] = 
{
	0x07,0x07,0x01,0x01,0x07,0x07,0x04,0x04,0x07,0x07
};

uint8_t THREE_SETTING[] =
{
	0x07,0x07,0x01,0x01,0x07,0x07,0x01,0x01,0x07,0x07
};


void Set_My_Ship_XY(uint8_t x_coor,uint8_t y_coor)
{
	int index = 0;
	int iloop = 0;
	
	/*** 0 -> 9 is x_coordinate  
	* About y i have to minus 16(col)*6(lines)*y_coor (16*6 is front blocks) to move to correspond y position 
	**/
	if(x_coor == 0 || x_coor == 4 || x_coor == 8)
			{
				if(x_coor == 0) 
					index = Mx0_coor_index - six_lines*y_coor;
				else if (x_coor == 4)
					index = Mx4_coor_index - six_lines*y_coor;
				else if (x_coor == 8)
					index = Mx8_coor_index - six_lines*y_coor;
				
				for(iloop = 0; iloop < 6; iloop++)
				{
					// Full fill block x block (6x6)
					BattleMap[index] 		|= 0x0F;	// xxxx 111111 xxxxxx // mask 4H and 2LH-4L
					BattleMap[index+1] 	|= 0xC0;
					index = index - 16; // Fill from last row to first row of block
				}
			}
			
			else if (x_coor == 1 || x_coor == 5 || x_coor == 9)
			{
				if(x_coor == 1) 
					index = Mx1_coor_index - six_lines*y_coor;
				else if (x_coor == 5)
					index = Mx5_coor_index - six_lines*y_coor;
				else if (x_coor == 9)
					index = Mx9_coor_index - six_lines*y_coor;
				
				for(iloop = 0; iloop < 6; iloop++)
				{
					// Full fill block x block (6x6)
					BattleMap[index] 		|= 0x3F;	// xx 111111 // mask 2HH
					index = index - 16; // Fill from last row to first row of block
				}
			}
			
			else if (x_coor == 2 || x_coor == 6)
			{
				if(x_coor == 2) 
					index = Mx2_coor_index - six_lines*y_coor;
				else if (x_coor == 6)
					index = Mx6_coor_index - six_lines*y_coor;
				
				for(iloop = 0; iloop < 6; iloop++)
				{
					// Full fill block x block (6x6)
					BattleMap[index] 		|= 0xFC;	// 111111 xx // mask 2LL
					index = index - 16; // Fill from last row to first row of block
				}
			}
			
			else if (x_coor == 3 || x_coor == 7)
			{
				if(x_coor == 3) 
					index = Mx3_coor_index - six_lines*y_coor;
				else if (x_coor == 7)
					index = Mx7_coor_index - six_lines*y_coor;
				
				for(iloop = 0; iloop < 6; iloop++)
				{
					// Full fill block x block (6x6)
					BattleMap[index] 		|= 0x03;	// xxxxxx 111111 xxxx // mask 4H-2LH and 4L
					BattleMap[index+1] 	|= 0xF0;
					index = index - 16; // Fill from last row to first row of block
				}
			}
			
			Display_Graphic(BattleMap);
}


void Destroy_My_Ship_XY(uint8_t x_coor, uint8_t y_coor)
{
	int index = 0;
	
	/*** 0 -> 9 is x_coordinate  
	* About y i have to minus 16(col)*6(lines)*y_coor (16*6 is front blocks) to move to correspond y position 
	**/
	if(x_coor == 0 || x_coor == 4 || x_coor == 8)
			{
				if(x_coor == 0) 
					index = Mx0_coor_index - six_lines*y_coor;
				else if (x_coor == 4)
					index = Mx4_coor_index - six_lines*y_coor;
				else if (x_coor == 8)
					index = Mx8_coor_index - six_lines*y_coor;
					
					// Fill "destroy" (x symbol) inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xF0; // Keep old bits status
					//BattleMap[index] 			|= 0x00; // xxxx 000000 xxxxxx // mask 4H and 2HL-4L
				
					BattleMap[index+1] 		&= 0x3F; // Keep bits status
					//BattleMap[index+1] 		|= 0x00; 
				
					BattleMap[index - 16] &= 0xF0; // Keep old bits status
					BattleMap[index - 16] |= 0x04; // xxxx 010010 xxxxxx 
				
					BattleMap[index - 15] &= 0x3F; // Keep old bits status	
					BattleMap[index - 15] |= 0x80;
					
					BattleMap[index - 32] &= 0xF0; // Keep old bits status
					BattleMap[index - 32] |= 0x03; // xxxx 001100 xxxxxx
				
					BattleMap[index - 31] &= 0x3F; // Keep bits status
					//BattleMap[index - 31] |= 0x00;
				
					BattleMap[index - 48] &= 0xF0; // Keep old bits status
					BattleMap[index - 48] |= 0x03; // xxxx 001100 xxxxxx
				
					BattleMap[index - 47] &= 0x3F; // Keep old bits status
					//BattleMap[index - 47] |= 0x00;
				
					BattleMap[index - 64] &= 0xF0; // Keep old bits status
					BattleMap[index - 64] |= 0x04; // xxxx 010010 xxxxxx
					
					BattleMap[index - 63] &= 0x3F; // Keep old bits status
					BattleMap[index - 63] |= 0x80;
					
					BattleMap[index - 80] &= 0xF0; // Keep old bits status
					//BattleMap[index - 80] |= 0x00; // xxxx 000000 xxxxxx
					
					BattleMap[index - 79] &= 0x3F; // Keep old bits status
					//BattleMap[index - 79] |= 0x00;
			}
			
			else if (x_coor == 1 || x_coor == 5 || x_coor == 9)
			{
				if(x_coor == 1) 
					index = Mx1_coor_index - six_lines*y_coor;
				else if (x_coor == 5)
					index = Mx5_coor_index - six_lines*y_coor;
				else if (x_coor == 9)
					index = Mx9_coor_index - six_lines*y_coor;
				
					// Fill "destroy" (x symbol) inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xC0; // Keep old bits status
					//BattleMap[index] 			|= 0x00; // xx000000  // mask 2HH
				
					BattleMap[index - 16] &= 0xD2; // Keep old bits status
					BattleMap[index - 16] |= 0x12; // xx010010
				
					BattleMap[index - 32] &= 0xCC; // Keep old bits status
					BattleMap[index - 32] |= 0x0C; // xx001100
				
					BattleMap[index - 48] &= 0xCC; // Keep old bits status
					BattleMap[index - 48] |= 0x0C; // xx001100
				
					BattleMap[index - 64] &= 0xD2; // Keep old bits status
					BattleMap[index - 64] |= 0x12; // xx010010
				
					BattleMap[index - 80] &= 0xC0; // Keep old bits status
					//BattleMap[index - 80] |= 0x00; // xx000000
			}
			
			else if (x_coor == 2 || x_coor == 6)
			{
				if(x_coor == 2) 
					index = Mx2_coor_index - six_lines*y_coor;
				else if (x_coor == 6)
					index = Mx6_coor_index - six_lines*y_coor;
				
					// Fill "destroy" (x symbol) inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0x03; // Keep old bits status
					//BattleMap[index] 			|= 0x00; // 000000xx // mask 2LL
					
					BattleMap[index - 16] &= 0x4B; // Keep old bits status
					BattleMap[index - 16] |= 0x48; // 010010xx  
				
					BattleMap[index - 32] &= 0x33; // Keep old bits status
					BattleMap[index - 32] |= 0x30; // 001100xx
					
					BattleMap[index - 48] &= 0x33; // Keep old bits status
					BattleMap[index - 48] |= 0x30; // 001100xx
				
					BattleMap[index - 64] &= 0x4B; // Keep old bits status
					BattleMap[index - 64] |= 0x48; // 010010xx
				
					BattleMap[index - 80] &= 0x03; // Keep old bits status
					//BattleMap[index - 80] |= 0x00; // 000000xx
			}
			
			else if (x_coor == 3 || x_coor == 7)
			{
				if(x_coor == 3) 
					index = Mx3_coor_index - six_lines*y_coor;
				else if (x_coor == 7)
					index = Mx7_coor_index - six_lines*y_coor;
				
					// Fill "destroy" (x symbol) inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xFC;	// Keep old bits status
					//BattleMap[index] 			|= 0x00;	// xxxxxx 000000 xxxx   // mask 4H-2LH and 4L
				
					BattleMap[index + 1] 	&= 0x0F;	// Keep old bits status
					//BattleMap[index + 1] 	|= 0x00;	
					
					BattleMap[index - 16] &= 0xFD;	// Keep old bits status
					BattleMap[index - 16] |= 0x01;	// xxxxxx 010010 xxxx
				
					BattleMap[index - 15] &= 0x2F;	// Keep old bits status
					BattleMap[index - 15] |= 0x20;	
				
					BattleMap[index - 32] &= 0xFC;	// Keep old bits status
					//BattleMap[index - 32] |= 0x00;	// xxxxxx 001100 xxxx
				
					BattleMap[index - 31] &= 0xCF;	// Keep old bits status
					BattleMap[index - 31] |= 0xC0;	
				
					BattleMap[index - 48] &= 0xFC;	// Keep old bits status
					//BattleMap[index - 48] |= 0x00;	// xxxxxx 001100 xxxx
					
					BattleMap[index - 47] &= 0xCF;	// Keep old bits status
					BattleMap[index - 47] |= 0xC0;	
					
					BattleMap[index - 64] &= 0xFD;	// Keep old bits status
					BattleMap[index - 64] |= 0x01;	// xxxxxx 010010 xxxx
					
					BattleMap[index - 63] &= 0x2F;	// Keep old bits status
					BattleMap[index - 63] |= 0x20;	
					
					BattleMap[index - 80] &= 0xFC;	// Keep old bits status
					//BattleMap[index - 80] |= 0x00;	// xxxxxx 000000 xxxx
					
					BattleMap[index - 79] &= 0x0F;	// Keep old bits status
					//BattleMap[index - 79] |= 0x00;	
			}
			
			Display_Graphic(BattleMap);
}


void Set_Enemy_Ship_XY(uint8_t x_coor,uint8_t y_coor)
{	
	int index = 0;
	int iloop = 0;
	
	/*** 0 -> 9 is x_coordinate  
	* About y i have to minus 16(col)*6(lines)*y_coor (16*6 is front blocks) to move to correspond y position 
	**/
	if(x_coor == 0 || x_coor == 4 || x_coor == 8)
			{
				if(x_coor == 0) 
					index = Ex0_coor_index - six_lines*y_coor;
				else if (x_coor == 4)
					index = Ex4_coor_index - six_lines*y_coor;
				else if (x_coor == 8)
					index = Ex8_coor_index - six_lines*y_coor;
				
				for(iloop = 0; iloop < 6; iloop++)
				{
					// Full fill block x block (6x6)
					BattleMap[index] 		|= 0xFC;	// 111111 xx // Mask 2L
					index = index - 16; // Fill from last row to first row of block
				}
			}
			
			else if (x_coor == 1 || x_coor == 5 || x_coor == 9)
			{
				if(x_coor == 1) 
					index = Ex1_coor_index - six_lines*y_coor;
				else if (x_coor == 5)
					index = Ex5_coor_index - six_lines*y_coor;
				else if (x_coor == 9)
					index = Ex9_coor_index - six_lines*y_coor;
				
				for(iloop = 0; iloop < 6; iloop++)
				{
					// Full fill block x block (6x6)
					BattleMap[index] 		|= 0x03;	// xx 111111 xxxx // Mask 2HH and 4L
					BattleMap[index + 1]|= 0xF0;
					index = index - 16; // Fill from last row to first row of block
				}
			}
			
			else if (x_coor == 2 || x_coor == 6)
			{
				if(x_coor == 2) 
					index = Ex2_coor_index - six_lines*y_coor;
				else if (x_coor == 6)
					index = Ex6_coor_index - six_lines*y_coor;
				
				for(iloop = 0; iloop < 6; iloop++)
				{
					// Full fill block x block (6x6)
					BattleMap[index] 		 |= 0x0F;	// xxxx 111111 xx // Mask 4H and 2L
					BattleMap[index + 1] |= 0xC0;
					index = index - 16; // Fill from last row to first row of block
				}
			}
			
			else if (x_coor == 3 || x_coor == 7)
			{
				if(x_coor == 3) 
					index = Ex3_coor_index - six_lines*y_coor;
				else if (x_coor == 7)
					index = Ex7_coor_index - six_lines*y_coor;
				
				for(iloop = 0; iloop < 6; iloop++)
				{
					// Full fill block x block (6x6)
					BattleMap[index] 		|= 0x3F;	// xx 111111 // Mask 2H
					index = index - 16; // Fill from last row to first row of block
				}
			}
			
			Display_Graphic(BattleMap);
}


void Destroy_Enemy_Ship_XY(uint8_t x_coor, uint8_t y_coor)
{
	int index = 0;
	
	/*** 0 -> 9 is x_coordinate  
	* About y i have to minus 16(col)*6(lines)*y_coor (16*6 is front blocks) to move to correspond y position 
	**/
	if(x_coor == 0 || x_coor == 4 || x_coor == 8)
			{
				if(x_coor == 0) 
					index = Ex0_coor_index - six_lines*y_coor;
				else if (x_coor == 4)
					index = Ex4_coor_index - six_lines*y_coor;
				else if (x_coor == 8)
					index = Ex8_coor_index - six_lines*y_coor;
					
					// Fill "destroy" (x symbol) inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0x03; // Keep old bits status
					//BattleMap[index] 			|= 0x00; // 000000 xx // Mask 4L
				
					BattleMap[index - 16] &= 0x4B; // Keep old bits status
					BattleMap[index - 16] |= 0x48; // 010010 xx
				
					BattleMap[index - 32] &= 0x33; // Keep old bits status
					BattleMap[index - 32] |= 0x30; // 001100 xx
				
					BattleMap[index - 48] &= 0x33; // Keep old bits status
					BattleMap[index - 48] |= 0x30; // 001100 xx
				
					BattleMap[index - 64] &= 0x4B; // Keep old bits status
					BattleMap[index - 64] |= 0x48; // 010010 xx
				
					BattleMap[index - 80] &= 0x03; // Keep old bits status
					//BattleMap[index - 80] |= 0x00; // 000000 xx
				
			}
			
			else if (x_coor == 1 || x_coor == 5 || x_coor == 9)
			{
				if(x_coor == 1) 
					index = Ex1_coor_index - six_lines*y_coor;
				else if (x_coor == 5)
					index = Ex5_coor_index - six_lines*y_coor;
				else if (x_coor == 9)
					index = Ex9_coor_index - six_lines*y_coor;
				
					// Fill "destroy" (x symbol) inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xFC; // Keep old bits status
					//BattleMap[index] 			|= 0x00; // xxxxxx 000000 xxxx // mask 4H-2LH and 4L
				
					BattleMap[index+1] 		&= 0x0F; // Keep old bits status
					//BattleMap[index+1] 		|= 0x00; 
				
					BattleMap[index - 16] &= 0xFD; // Keep old bits status
					BattleMap[index - 16] |= 0x01; // xxxxxx 010010 xxxx
				
					BattleMap[index - 15] &= 0x2F; // Keep old bits status
					BattleMap[index - 15] |= 0x20;
					
					BattleMap[index - 32] &= 0xFC; // Keep old bits status
					//BattleMap[index - 32] |= 0x00; // xxxxxx 001100 xxxx
				
					BattleMap[index - 31] &= 0xCF; // Keep old bits status
					BattleMap[index - 31] |= 0xC0;
				
					BattleMap[index - 48] &= 0xFC; // Keep old bits status
					//BattleMap[index - 48] |= 0x00; // xxxxxx 001100 xxxx
					
					BattleMap[index - 47] &= 0xCF; // Keep old bits status
					BattleMap[index - 47] |= 0xC0;
					
					BattleMap[index - 64] &= 0xFD; // Keep old bits status
					BattleMap[index - 64] |= 0x01; // xxxxxx 010010 xxxx
					
					BattleMap[index - 63] &= 0x2F; // Keep old bits status
					BattleMap[index - 63] |= 0x20;
					
					BattleMap[index - 80] &= 0xFC; // Keep old bits status
					//BattleMap[index - 80] |= 0x00; // xxxxxx 000000 xxxx
					
					BattleMap[index - 79] &= 0x0F; // Keep old bits status
					//BattleMap[index - 79] |= 0x00;
			}
			
			else if (x_coor == 2 || x_coor == 6)
			{
				if(x_coor == 2) 
					index = Ex2_coor_index - six_lines*y_coor;
				else if (x_coor == 6)
					index = Ex6_coor_index - six_lines*y_coor;
					
					// Fill "destroy" (x symbol) inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xF0; // Keep old bits status
					//BattleMap[index] 			|= 0x00; // xxxx 000000 xxxxxx // mask 4H and 2HL-4L
				
					BattleMap[index+1] 		&= 0x3F; // Keep old bits status 
					//BattleMap[index+1] 		|= 0x00; 
				
					BattleMap[index - 16] &= 0xF4; // Keep old bits status
					BattleMap[index - 16] |= 0x04; // xxxx 010010 xxxxxx 
				
					BattleMap[index - 15] &= 0xBF; // Keep bits status
					BattleMap[index - 15] |= 0x80;
				
					BattleMap[index - 32] &= 0xF3; // Keep old bits status
					BattleMap[index - 32] |= 0x03; // xxxx 001100 xxxxxx
				
					BattleMap[index - 31] &= 0x3F; // Keep old bits status
					//BattleMap[index - 31] |= 0x00;
				
					BattleMap[index - 48] &= 0xF3; // Keep old bits status
					BattleMap[index - 48] |= 0x03; // xxxx 001100 xxxxxx
					
					BattleMap[index - 47] &= 0x3F; // Keep old bits status
					//BattleMap[index - 47] |= 0x00;
					
					BattleMap[index - 64] &= 0xF4; // Keep old bits status
					BattleMap[index - 64] |= 0x04; // xxxx 010010 xxxxxx
					
					BattleMap[index - 63] &= 0xBF; // Keep old bits status
					BattleMap[index - 63] |= 0x80;
					
					BattleMap[index - 80] &= 0xF0; // Keep old bits status
					//BattleMap[index - 80] |= 0x00; // xxxx 000000 xxxxxx
					
					BattleMap[index - 79] &= 0x3F; // Keep old bits status
					//BattleMap[index - 79] |= 0x00;
					
			}
			
			else if (x_coor == 3 || x_coor == 7)
			{
				if(x_coor == 3) 
					index = Ex3_coor_index - six_lines*y_coor;
				else if (x_coor == 7)
					index = Ex7_coor_index - six_lines*y_coor;
				
					// Fill "destroy" (x symbol) inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xC0; // Keep old bits status
					//BattleMap[index] 			|= 0x00; // xx 000000 //Mask 2HH 
				
					BattleMap[index - 16] &= 0xD2; // Keep old bits status
					BattleMap[index - 16] |= 0x12; // xx 010010
				
					BattleMap[index - 32] &= 0xCC; // Keep old bits status
					BattleMap[index - 32] |= 0x0C; // xx 001100
				
					BattleMap[index - 48] &= 0xCC; // Keep old bits status
					BattleMap[index - 48] |= 0x0C; // xx 001100
				
					BattleMap[index - 64] &= 0xD2; // Keep old bits status
					BattleMap[index - 64] |= 0x12; // xx 010010
				
					BattleMap[index - 80] &= 0xC0; // Keep old bits status
					//BattleMap[index - 80] |= 0x00; // xx 000000
			}
			
			Display_Graphic(BattleMap);
}

void Display_Shoot_XY_Position(uint8_t x, uint8_t y)
{
	uint8_t X[6];
	uint8_t Y[6];
			
	
			switch(x)
			{
				// Copy arr 'Xa' to 'X' with size = 6 correspond with 'x' param
				case 0: memcpy(X, X0, 6); break;
				case 1: memcpy(X, X1, 6); break;
				case 2: memcpy(X, X2, 6); break;
				case 3: memcpy(X, X3, 6); break;
				case 4: memcpy(X, X4, 6); break;
				case 5: memcpy(X, X5, 6); break;
				case 6: memcpy(X, X6, 6); break;
				case 7: memcpy(X, X7, 6); break;
				case 8: memcpy(X, X8, 6); break;
				case 9: memcpy(X, X9, 6); break;
				default: break;
			}
			
			switch(y)
			{
				// Copy arr 'Ya' to 'Y' with size = 6 correspond with 'y' param
				case 0: memcpy(Y, Y0, 6); break;
				case 1: memcpy(Y, Y1, 6); break;
				case 2: memcpy(Y, Y2, 6); break;
				case 3: memcpy(Y, Y3, 6); break;
				case 4: memcpy(Y, Y4, 6); break;
				case 5: memcpy(Y, Y5, 6); break;
				case 6: memcpy(Y, Y6, 6); break;
				case 7: memcpy(Y, Y7, 6); break;
				case 8: memcpy(Y, Y8, 6); break;
				case 9: memcpy(Y, Y9, 6); break;
				default: break;
			}
			
			// Save 'x' position selection
			BattleMap[6] = X[0];
			BattleMap[7] = X[1];
			BattleMap[22] = X[2];
			BattleMap[23] = X[3];
			BattleMap[38] = X[4];
			BattleMap[39] = X[5];
					
			// Save 'y' position selection
			BattleMap[8] = Y[0];
			BattleMap[9] = Y[1];
			BattleMap[24] = Y[2];
			BattleMap[25] = Y[3];
			BattleMap[40] = Y[4];
			BattleMap[41] = Y[5];
				
			Display_Graphic(BattleMap);
			
}

void Display_Turn(bool myturn)
{
	// Display "turn" word of enemy or me
	if(myturn == false)
		{
			BattleMap[0] = 0xEA;
			BattleMap[1] = 0x6E;
			BattleMap[16] = 0x4A;
			BattleMap[17] = 0x8A;
			BattleMap[32] = 0x4E;
			BattleMap[33] = 0x8A;
			
			BattleMap[14] = 0x00;
			BattleMap[15] = 0x00;
			BattleMap[30] = 0x00;
			BattleMap[31] = 0x00;	
			BattleMap[46] = 0x00;
			BattleMap[47] = 0x00;
			
		}
	else
		{
			BattleMap[14] = 0xEA;
			BattleMap[15] = 0x6E;
			BattleMap[30] = 0x4A;
			BattleMap[31] = 0x8A;		
			BattleMap[46] = 0x4E;
			BattleMap[47] = 0x8A;
			
			BattleMap[0] = 0x00;
			BattleMap[1] = 0x00;
			BattleMap[16] = 0x00;
			BattleMap[17] = 0x00;	
			BattleMap[32] = 0x00;
			BattleMap[33] = 0x00;
		}
		
		Display_Graphic(BattleMap);
}

void Display_Setup_NumofShip(uint8_t pos, uint8_t num)
{
	short i = 0;
	short j = 0;
	uint8_t POS[14];
	
	switch(num)
			{
				// Copy arr on the right to 'POS' with size = 14 or 10 correspond with 'num' param
				case 1: memcpy(POS, ONE, 14); break;
				case 2: memcpy(POS, TWO, 14); break;
				case 3: memcpy(POS, THREE, 14); break;
				// I use these values for convenient instead of create one more param :)) 
				case 5: memcpy(POS, ONE_SETTING, 10); break; 
				case 6: memcpy(POS, TWO_SETTING, 10); break;
				case 7: memcpy(POS, THREE_SETTING, 10); break;
				default: break;
			}
	
	// If 'L'(left), this is index of ship are setting up, E.g: Left / Right
	if(pos == 'L')
	{
		for(i = 546; i<= 754; i+=16)
		{
			BattleMap[i] = POS[j];
			j++;
		}
	}
	// If 'R'(right), this is index of ship are setting up, E.g: Left / Right
	else if(pos == 'R')
	{
		for(i = 548; i<= 756; i+=16)
		{
			BattleMap[i] = POS[j];
			j++;
		}
	}
	// If 'S'(setting), this is type of index of ship are setting up, E.g: "SETTING SHIP X"
	else if(pos == 'S')
	{
		for(i = 262; i<= 406; i+=16)
		{
			BattleMap[i] &= 0xF0;	// And(&) old bits because 'P' and this symbol is in the same element
			BattleMap[i] |= POS[j];
			j++;
		}
	}
	Display_Graphic(BattleMap);
}

void Display_Word_SettingShip(void)
{
	short i = 0;
	short j = 0;

	// DISPLAY "SETTING SHIP"
	for(i = 257 ; i <= 406 ; i+=16)
	{
		BattleMap[i]   = SETTING_SHIP[j];
		BattleMap[i+1] = SETTING_SHIP[j+1];
		BattleMap[i+2] = SETTING_SHIP[j+2];
		BattleMap[i+3] = SETTING_SHIP[j+3];
		BattleMap[i+4] = SETTING_SHIP[j+4];
		BattleMap[i+5] = SETTING_SHIP[j+5];
		j+=6;
	}
	
	// DISPLAY "SETTING SHIP 1"
	Display_Setup_NumofShip('S',5);
		
	// DISPLAY ONE - LEFT
	j=0;
	for(i = 546; i<= 754; i+=16)
	{
		BattleMap[i] = ONE[j];
		j++;
	}
		
	// DISPLAY SLASH
	j=0;
	for(i = 515; i <= 787 ; i+=16)
	{
		BattleMap[i] = SLASH[j];
		j++;
	}
		
	// DISPLAY ONE - RIGHT
	j=0;
	for(i = 548; i<= 756; i+=16)
	{
		BattleMap[i] = ONE[j];
		j++;
	}	
	Display_Graphic(BattleMap);
}

void Display_None(void)
{
	short i = 0;
	for(i = 257 ; i <= 787 ; i+=16)
	{
		BattleMap[i]   = 0x00;
		BattleMap[i+1] = 0x00;
		BattleMap[i+2] = 0x00;
		BattleMap[i+3] = 0x00;
		BattleMap[i+4] = 0x00;
		BattleMap[i+5] = 0x00;
	}
	Display_Graphic(BattleMap);
}
