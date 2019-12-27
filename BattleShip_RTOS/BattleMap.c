#include "BattleMap.h"	// Get x,y defines
#include "LCDST7920.h"	// Get LCD functions

// ########## X #############
uint8_t X0[6] =
{
	0x00,0xE0,
	0x00,0xA0,
	0x00,0xE0
};

uint8_t X1[] =
{
	0x00,0x20,
	0x00,0x20,
	0x00,0x20
};

uint8_t X2[] =
{
	0x00,0xA0,
	0x00,0xA0,
	0x00,0xA0
};


uint8_t X3[] =
{
	0x02,0xA0,
	0x02,0xA0,
	0x02,0xA0
};
uint8_t X4[] = 
{
	0x02,0xA0,
	0x02,0xA0,
	0x02,0x40
};
uint8_t X5[] =
{
	0x00,0xA0,
	0x00,0xA0,
	0x00,0x40
};
uint8_t X6[] =
{
	0x02,0xA0,
	0x02,0xA0,
	0x01,0x20
};

uint8_t X7[] =
{
	0x0A,0xA0,
	0x0A,0xA0,
	0x04,0xA0
};

uint8_t X8[] =
{
	0x2A,0xA0,
	0x2A,0xA0,
	0x12,0xA0
};

uint8_t X9[] =
{
	0x02,0xA0,
	0x02,0x40,
	0x02,0xA0
};


// ########## Y #############

uint8_t Y0[] =
{
	0x01,0xC0,
	0x01,0x40,
	0x01,0xC0
};

uint8_t Y1[] =
{
	0x01,0x00,
	0x01,0x00,
	0x01,0x00
};

uint8_t Y2[] =
{
	0x01,0x40,
	0x01,0x40,
	0x01,0x40
};

uint8_t Y3[] =
{
	0x01,0x50,
	0x01,0x50,
	0x01,0x50
};
uint8_t Y4[] = 
{
	0x01,0x50,
	0x01,0x50,
	0x01,0x20
};
uint8_t Y5[] =
{
	0x01,0x40,
	0x01,0x40,
	0x00,0x80
};
uint8_t Y6[] =
{
	0x01,0x50,
	0x01,0x50,
	0x00,0x90
};

uint8_t Y7[] =
{
	0x01,0x54,
	0x01,0x54,
	0x00,0x94
};

uint8_t Y8[] =
{
	0x01,0x55,
	0x01,0x55,
	0x00,0x95
};

uint8_t Y9[] =
{
	0x01,0x50,
	0x01,0x20,
	0x01,0x50
};


void Set_My_Ship_XY(uint8_t x_coor,uint8_t y_coor)//(uint8_t position)
{
		//uint8_t x_coor = position/10; // Get x_coor
		//uint8_t y_coor = position%10; // Get y_coor
	
		int index = 0;
		int iloop = 0;
	
	// 0 -> 9 is x_coordinate , about y i have to minus 16(col)*6(lines)*y_coor (16*6 is front block)
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
					// full fill block x block (6x6)
					BattleMap[index] 		|= 0x0F;	// xxxx 111111 xxxxxx // mask 4H and 2LH-4L
					BattleMap[index+1] 	|= 0xC0;
					index = index - 16;
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
					// full fill block x block (6x6)
					BattleMap[index] 		|= 0x3F;	// xx 111111 // mask 2HH
					index = index - 16;
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
					// full fill block x block (6x6)
					BattleMap[index] 		|= 0xFC;	// 111111 xx // mask 2LL
					index = index - 16;
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
					// full fill block x block (6x6)
					BattleMap[index] 		|= 0x03;	// xxxxxx 111111 xxxx // mask 4H-2LH and 4L
					BattleMap[index+1] 	|= 0xF0;
					index = index - 16;
				}
			}
			
			Display_Graphic(BattleMap);
}


void Destroy_My_Ship_XY(uint8_t x_coor, uint8_t y_coor)
{
		int index = 0;
	// 0 -> 9 is x_coordinate , about y i have to minus 16(col)*6(lines)*y_coor (16*6 is front block)
	if(x_coor == 0 || x_coor == 4 || x_coor == 8)
			{
				if(x_coor == 0) 
					index = Mx0_coor_index - six_lines*y_coor;
				else if (x_coor == 4)
					index = Mx4_coor_index - six_lines*y_coor;
				else if (x_coor == 8)
					index = Mx8_coor_index - six_lines*y_coor;
					
					// fill 'destroy - x' symbol inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xF0; // Keep bits status
					//BattleMap[index] 			|= 0x00; // xxxx 000000 xxxxxx // mask 4H and 2HL-4L
				
					BattleMap[index+1] 		&= 0x3F; // Keep bits status
					//BattleMap[index+1] 		|= 0x00; 
				
					BattleMap[index - 16] &= 0xF0; // Keep bits status
					BattleMap[index - 16] |= 0x04; // xxxx 010010 xxxxxx 
				
					BattleMap[index - 15] &= 0x3F; // Keep bits status	
					BattleMap[index - 15] |= 0x80;
					
					BattleMap[index - 32] &= 0xF0; // Keep bits status
					BattleMap[index - 32] |= 0x03; // xxxx 001100 xxxxxx
				
					BattleMap[index - 31] &= 0x3F; // Keep bits status
					//BattleMap[index - 31] |= 0x00;
				
					BattleMap[index - 48] &= 0xF0; // Keep bits status
					BattleMap[index - 48] |= 0x03; // xxxx 001100 xxxxxx
				
					BattleMap[index - 47] &= 0x3F; // Keep bits status
					//BattleMap[index - 47] |= 0x00;
				
					BattleMap[index - 64] &= 0xF0; // Keep bits status
					BattleMap[index - 64] |= 0x04; // xxxx 010010 xxxxxx
					
					BattleMap[index - 63] &= 0x3F; // Keep bits status
					BattleMap[index - 63] |= 0x80;
					
					BattleMap[index - 80] &= 0xF0; // Keep bits status
					//BattleMap[index - 80] |= 0x00; // xxxx 000000 xxxxxx
					
					BattleMap[index - 79] &= 0x3F; // Keep bits status
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
				
					// fill 'destroy - x' symbol inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xC0; // Keep bits status
					//BattleMap[index] 			|= 0x00; // xx000000  // mask 2HH
				
					BattleMap[index - 16] &= 0xD2; // Keep bits status
					BattleMap[index - 16] |= 0x12; // xx010010
				
					BattleMap[index - 32] &= 0xCC; // Keep bits status
					BattleMap[index - 32] |= 0x0C; // xx001100
				
					BattleMap[index - 48] &= 0xCC; // Keep bits status
					BattleMap[index - 48] |= 0x0C; // xx001100
				
					BattleMap[index - 64] &= 0xD2; // Keep bits status
					BattleMap[index - 64] |= 0x12; // xx010010
				
					BattleMap[index - 80] &= 0xC0; // Keep bits status
					//BattleMap[index - 80] |= 0x00; // xx000000
			}
			
			else if (x_coor == 2 || x_coor == 6)
			{
				if(x_coor == 2) 
					index = Mx2_coor_index - six_lines*y_coor;
				else if (x_coor == 6)
					index = Mx6_coor_index - six_lines*y_coor;
				
					// fill 'destroy - x' symbol inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0x03; // Keep bits status
					//BattleMap[index] 			|= 0x00; // 000000xx // mask 2LL
					
					BattleMap[index - 16] &= 0x4B; // Keep bits status
					BattleMap[index - 16] |= 0x48; // 010010xx  
				
					BattleMap[index - 32] &= 0x33; // Keep bits status
					BattleMap[index - 32] |= 0x30; // 001100xx
					
					BattleMap[index - 48] &= 0x33; // Keep bits status
					BattleMap[index - 48] |= 0x30; // 001100xx
				
					BattleMap[index - 64] &= 0x4B; // Keep bits status
					BattleMap[index - 64] |= 0x48; // 010010xx
				
					BattleMap[index - 80] &= 0x03; // Keep bits status
					//BattleMap[index - 80] |= 0x00; // 000000xx
			}
			
			else if (x_coor == 3 || x_coor == 7)
			{
				if(x_coor == 3) 
					index = Mx3_coor_index - six_lines*y_coor;
				else if (x_coor == 7)
					index = Mx7_coor_index - six_lines*y_coor;
				
					// fill 'destroy - x' symbol inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xFC;	// Keep bits status
					//BattleMap[index] 			|= 0x00;	// xxxxxx 000000 xxxx   // mask 4H-2LH and 4L
				
					BattleMap[index + 1] 	&= 0x0F;	// Keep bits status
					//BattleMap[index + 1] 	|= 0x00;	
					
					BattleMap[index - 16] &= 0xFD;	// Keep bits status
					BattleMap[index - 16] |= 0x01;	// xxxxxx 010010 xxxx
				
					BattleMap[index - 15] &= 0x2F;	// Keep bits status
					BattleMap[index - 15] |= 0x20;	
				
					BattleMap[index - 32] &= 0xFC;	// Keep bits status
					//BattleMap[index - 32] |= 0x00;	// xxxxxx 001100 xxxx
				
					BattleMap[index - 31] &= 0xCF;	// Keep bits status
					BattleMap[index - 31] |= 0xC0;	
				
					BattleMap[index - 48] &= 0xFC;	// Keep bits status
					//BattleMap[index - 48] |= 0x00;	// xxxxxx 001100 xxxx
					
					BattleMap[index - 47] &= 0xCF;	// Keep bits status
					BattleMap[index - 47] |= 0xC0;	
					
					BattleMap[index - 64] &= 0xFD;	// Keep bits status
					BattleMap[index - 64] |= 0x01;	// xxxxxx 010010 xxxx
					
					BattleMap[index - 63] &= 0x2F;	// Keep bits status
					BattleMap[index - 63] |= 0x20;	
					
					BattleMap[index - 80] &= 0xFC;	// Keep bits status
					//BattleMap[index - 80] |= 0x00;	// xxxxxx 000000 xxxx
					
					BattleMap[index - 79] &= 0x0F;	// Keep bits status
					//BattleMap[index - 79] |= 0x00;	
			}
			
			Display_Graphic(BattleMap);
}


void Set_Enemy_Ship_XY(uint8_t x_coor,uint8_t y_coor)//(uint8_t position)
{	
		//uint8_t x_coor = position/10; // Get x_coor
		//uint8_t y_coor = position%10; // Get y_coor
	
		int index = 0;
		int iloop = 0;
	// 0 -> 9 is x_coordinate , about y i have to minus 16(col)*6(lines)*y_coor (16*6 is front block)
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
					// full fill block x block (6x6)
					BattleMap[index] 		|= 0xFC;	// 111111 xx // Mask 2L
					index = index - 16;
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
					// full fill block x block (6x6)
					BattleMap[index] 		|= 0x03;	// xx 111111 xxxx // Mask 2HH and 4L
					BattleMap[index + 1]|= 0xF0;
					index = index - 16;
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
					// full fill block x block (6x6)
					BattleMap[index] 		 |= 0x0F;	// xxxx 111111 xx // Mask 4H and 2L
					BattleMap[index + 1] |= 0xC0;
					index = index - 16;
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
					// full fill block x block (6x6)
					BattleMap[index] 		|= 0x3F;	// xx 111111 // Mask 2H
					index = index - 16;
				}
			}
			
			Display_Graphic(BattleMap);
}


void Destroy_Enemy_Ship_XY(uint8_t x_coor, uint8_t y_coor)
{
	int index = 0;
	// 0 -> 9 is x_coordinate , about y i have to minus 16(col)*6(lines)*y_coor (16*6 is front block)
	if(x_coor == 0 || x_coor == 4 || x_coor == 8)
			{
				if(x_coor == 0) 
					index = Ex0_coor_index - six_lines*y_coor;
				else if (x_coor == 4)
					index = Ex4_coor_index - six_lines*y_coor;
				else if (x_coor == 8)
					index = Ex8_coor_index - six_lines*y_coor;
					
					// fill 'destroy - x' symbol inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0x03; // Keep bits status
					//BattleMap[index] 			|= 0x00; // 000000 xx // Mask 4L
				
					BattleMap[index - 16] &= 0x4B; // Keep bits status
					BattleMap[index - 16] |= 0x48; // 010010 xx
				
					BattleMap[index - 32] &= 0x33; // Keep bits status
					BattleMap[index - 32] |= 0x30; // 001100 xx
				
					BattleMap[index - 48] &= 0x33; // Keep bits status
					BattleMap[index - 48] |= 0x30; // 001100 xx
				
					BattleMap[index - 64] &= 0x4B; // Keep bits status
					BattleMap[index - 64] |= 0x48; // 010010 xx
				
					BattleMap[index - 80] &= 0x03; // Keep bits status
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
				
					// fill 'destroy - x' symbol inside block x block (6x6)			
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xFC; // Keep bits status
					//BattleMap[index] 			|= 0x00; // xxxxxx 000000 xxxx // mask 4H-2LH and 4L
				
					BattleMap[index+1] 		&= 0x0F; // Keep bits status
					//BattleMap[index+1] 		|= 0x00; 
				
					BattleMap[index - 16] &= 0xFD; // Keep bits status
					BattleMap[index - 16] |= 0x01; // xxxxxx 010010 xxxx
				
					BattleMap[index - 15] &= 0x2F; // Keep bits status
					BattleMap[index - 15] |= 0x20;
					
					BattleMap[index - 32] &= 0xFC; // Keep bits status
					//BattleMap[index - 32] |= 0x00; // xxxxxx 001100 xxxx
				
					BattleMap[index - 31] &= 0xCF; // Keep bits status
					BattleMap[index - 31] |= 0xC0;
				
					BattleMap[index - 48] &= 0xFC; // Keep bits status
					//BattleMap[index - 48] |= 0x00; // xxxxxx 001100 xxxx
					
					BattleMap[index - 47] &= 0xCF; // Keep bits status
					BattleMap[index - 47] |= 0xC0;
					
					BattleMap[index - 64] &= 0xFD; // Keep bits status
					BattleMap[index - 64] |= 0x01; // xxxxxx 010010 xxxx
					
					BattleMap[index - 63] &= 0x2F; // Keep bits status
					BattleMap[index - 63] |= 0x20;
					
					BattleMap[index - 80] &= 0xFC; // Keep bits status
					//BattleMap[index - 80] |= 0x00; // xxxxxx 000000 xxxx
					
					BattleMap[index - 79] &= 0x0F; // Keep bits status
					//BattleMap[index - 79] |= 0x00;
			}
			
			else if (x_coor == 2 || x_coor == 6)
			{
				if(x_coor == 2) 
					index = Ex2_coor_index - six_lines*y_coor;
				else if (x_coor == 6)
					index = Ex6_coor_index - six_lines*y_coor;
					
					// fill 'destroy - x' symbol inside block x block (6x6)				
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xF0; // Keep bits status
					//BattleMap[index] 			|= 0x00; // xxxx 000000 xxxxxx // mask 4H and 2HL-4L
				
					BattleMap[index+1] 		&= 0x3F; // Keep bits status 
					//BattleMap[index+1] 		|= 0x00; 
				
					BattleMap[index - 16] &= 0xF4; // Keep bits status
					BattleMap[index - 16] |= 0x04; // xxxx 010010 xxxxxx 
				
					BattleMap[index - 15] &= 0xBF; // Keep bits status
					BattleMap[index - 15] |= 0x80;
				
					BattleMap[index - 32] &= 0xF3; // Keep bits status
					BattleMap[index - 32] |= 0x03; // xxxx 001100 xxxxxx
				
					BattleMap[index - 31] &= 0x3F; // Keep bits status
					//BattleMap[index - 31] |= 0x00;
				
					BattleMap[index - 48] &= 0xF3; // Keep bits status
					BattleMap[index - 48] |= 0x03; // xxxx 001100 xxxxxx
					
					BattleMap[index - 47] &= 0x3F; // Keep bits status
					//BattleMap[index - 47] |= 0x00;
					
					BattleMap[index - 64] &= 0xF4; // Keep bits status
					BattleMap[index - 64] |= 0x04; // xxxx 010010 xxxxxx
					
					BattleMap[index - 63] &= 0xBF; // Keep bits status
					BattleMap[index - 63] |= 0x80;
					
					BattleMap[index - 80] &= 0xF0; // Keep bits status
					//BattleMap[index - 80] |= 0x00; // xxxx 000000 xxxxxx
					
					BattleMap[index - 79] &= 0x3F; // Keep bits status
					//BattleMap[index - 79] |= 0x00;
					
			}
			
			else if (x_coor == 3 || x_coor == 7)
			{
				if(x_coor == 3) 
					index = Ex3_coor_index - six_lines*y_coor;
				else if (x_coor == 7)
					index = Ex7_coor_index - six_lines*y_coor;
				
					// fill 'destroy - x' symbol inside block x block (6x6)
					// Comment 0x00 to be faster
				
					BattleMap[index] 			&= 0xC0; // Keep bits status
					//BattleMap[index] 			|= 0x00; // xx 000000 //Mask 2HH 
				
					BattleMap[index - 16] &= 0xD2; // Keep bits status
					BattleMap[index - 16] |= 0x12; // xx 010010
				
					BattleMap[index - 32] &= 0xCC; // Keep bits status
					BattleMap[index - 32] |= 0x0C; // xx 001100
				
					BattleMap[index - 48] &= 0xCC; // Keep bits status
					BattleMap[index - 48] |= 0x0C; // xx 001100
				
					BattleMap[index - 64] &= 0xD2; // Keep bits status
					BattleMap[index - 64] |= 0x12; // xx 010010
				
					BattleMap[index - 80] &= 0xC0; // Keep bits status
					//BattleMap[index - 80] |= 0x00; // xx 000000
			}
			
			Display_Graphic(BattleMap);
}

void Display_Select_XY_Number(uint8_t x, uint8_t y)
{
	uint8_t X[6];
	uint8_t Y[6];
			switch(x)
			{
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
	
				BattleMap[13] =  X[0];
				BattleMap[14] &= 0x01;
				BattleMap[14] |= X[1];
	
				BattleMap[29] =  X[2];
				BattleMap[30] &= 0x01;
				BattleMap[30] |= X[3];
	
				BattleMap[45] = X[4];
				BattleMap[46] &= 0x01;
				BattleMap[46] |= X[5];
			
				BattleMap[14] &= 0xE0;
				BattleMap[14] |= Y[0];
				BattleMap[15] = Y[1];
				
				BattleMap[30] &= 0xE0;
				BattleMap[30] |= Y[2];
				BattleMap[31] = Y[3];
				
				BattleMap[46] &= 0xE0;
				BattleMap[46] |= Y[4];
				BattleMap[47] = Y[5];
				
				
				Display_Graphic(BattleMap);
			
}

void Display_Turn(bool myturn)
{
	if(myturn == false)
		{
			BattleMap[5] = 0xEA;
			BattleMap[6] = 0x6E;
			BattleMap[21] = 0x4A;
			BattleMap[22] = 0x8A;
			BattleMap[37] = 0x4E;
			BattleMap[38] = 0x8A;
			
			BattleMap[9] = 0x00;
			BattleMap[10] = 0x00;
			BattleMap[25] = 0x00;
			BattleMap[26] = 0x00;	
			BattleMap[41] = 0x00;
			BattleMap[42] = 0x00;
			
		}
	else
		{
			BattleMap[9] = 0xEA;
			BattleMap[10] = 0x6E;
			BattleMap[25] = 0x4A;
			BattleMap[26] = 0x8A;		
			BattleMap[41] = 0x4E;
			BattleMap[42] = 0x8A;
			
			BattleMap[5] = 0x00;
			BattleMap[6] = 0x00;
			BattleMap[21] = 0x00;
			BattleMap[22] = 0x00;	
			BattleMap[37] = 0x00;
			BattleMap[38] = 0x00;
		}
		
		Display_Graphic(BattleMap);
}
