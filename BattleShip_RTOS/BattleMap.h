#include "NUC131.h" // Avoid uint8_t errors
#include <string.h> // Use memcpy (Copy Array to an Array)
#include <stdbool.h> // Use bool type

// The number are elements inside BattleMap array
#define Ex0_coor_index 1008
#define Ex1_coor_index 1008 
#define Ex2_coor_index 1009
#define Ex3_coor_index 1010
#define Ex4_coor_index 1011
#define Ex5_coor_index 1011
#define Ex6_coor_index 1012
#define Ex7_coor_index 1013
#define Ex8_coor_index 1014
#define Ex9_coor_index 1014

#define Mx0_coor_index 1016
#define Mx1_coor_index 1017 
#define Mx2_coor_index 1018
#define Mx3_coor_index 1018
#define Mx4_coor_index 1019
#define Mx5_coor_index 1020
#define Mx6_coor_index 1021
#define Mx7_coor_index 1021
#define Mx8_coor_index 1022
#define Mx9_coor_index 1023

#define six_lines 96


// Extern to use this variable inside functions below and can be re-declare inside main.c at the same time 
extern uint8_t BattleMap[];	

// Create ships symbol by blocks(6x6 pixels) - full fill
void Set_My_Ship_XY(uint8_t x_coor,uint8_t y_coor);

// Create destructions symbol by blocks(6x6 pixels) - 'x'
void Destroy_My_Ship_XY(uint8_t x_coor, uint8_t y_coor);

// Create ships symbol by blocks(6x6 pixels) - full fill
void Set_Enemy_Ship_XY(uint8_t x_coor,uint8_t y_coor);

// Create destructions symbol by blocks(6x6 pixels) - 'x'
void Destroy_Enemy_Ship_XY(uint8_t x_coor, uint8_t y_coor);

// Count Up or Down XY position at mid-top 
void Display_Shoot_XY_Position(uint8_t x, uint8_t y);

// Display who can be shot
void Display_Turn(bool myturn);

// Display ship setup index E.g: 1/1 - 1/3 2/3 3/3 - 1/3 2/3 3/3
void Display_Setup_NumofShip(uint8_t pos, uint8_t num);

// Display "SETTING SHIP X"
void Display_Word_SettingShip(void);

// Clear "SETTING SHIP X" buffer and ship setup index
void Display_None(void);
