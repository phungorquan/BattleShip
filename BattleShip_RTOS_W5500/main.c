#include "LCDST7920.h" // Get NUC131 and LCD functions
#include "BattleMap.h" // Get ship functions
#include "socket.h"
#include "FreeRTOSv10.2.1/FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOSv10.2.1/FreeRTOS/Source/include/task.h"
#include "FreeRTOSv10.2.1/FreeRTOS/Source/include/semphr.h"
#define PSTR(s) s
#define PLL_CLOCK   50000000


#define LEFTUP PA10
#define RIGHTUP PE5
#define LEFTDOWN PA11
#define RIGHTDOWN PA15
#define OK PB15


bool _SERVER	= false;
bool _CLIENT = true;
bool SERVER_SETUP = false;
bool CLIENT_SETUP = false;
bool MODE_DONE = false;
bool IMCLIENT = false;
uint8_t sectochangemode = 0;
uint8_t SENDBUF[20];
int32_t revbuf[10];
uint8_t distIPK[]={192,168,1,117};
uint8_t rcvBuf[20], bufSize[] = {2, 2, 2, 2};


wiz_NetInfo netInfotest = { {0x00, 0, 0, 0, 0, 0},	// Mac address
                          {0, 0, 0, 0},					// IP address
                          {0, 0, 0, 0},					// Subnet mask
                          {0, 0, 0, 0}};					// Gateway address

													
	
wiz_NetInfo netInfoSV = { {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},	// Mac address
                          {192, 168, 1, 117},					// IP address
                          {255, 255, 255, 0},					// Subnet mask
                          {192, 168, 1, 1}};					// Gateway address													

	
wiz_NetInfo netInfoCL = { {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xee},	// Mac address
                          {192, 168, 1, 116},					// IP address
                          {255, 255, 255, 0},					// Subnet mask
                          {192, 168, 1, 1}};					// Gateway address											
													

void TMR0_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER0) == 1)
    {
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER0);
				if(sectochangemode == 5)
				{
					sectochangemode = 0;
					PB13 ^= 1;
					if(_SERVER == false)
					{
						_SERVER = true;
						_CLIENT = false;
					}
					else
					{						
						if(_CLIENT == false)
						{
							_CLIENT = true;
							_SERVER = false;
						}
					}	
				}
				sectochangemode++;
        //g_au32TMRINTCount[0]++;
    }
}
	

int32_t loopback_tcpss(uint8_t sn, uint8_t* buf, uint16_t port)
{
   int32_t ret;
printf("CHECKING SERVER");
   switch(getSn_SR(sn))
   {
      case SOCK_ESTABLISHED :
         if(getSn_IR(sn) & Sn_IR_CON)
         {
						setSn_IR(sn,Sn_IR_CON);
						TIMER_Stop(TIMER0);
					 printf("ESTABLISH");
						_CLIENT = true;
					 _SERVER = true;
					 MODE_DONE = true;
						printf("I'M SERVER");
					 
         }
				 
         break;
				 
      case SOCK_CLOSE_WAIT :
         if((ret = disconnect(sn)) != SOCK_OK) return ret;
				printf("CLOSE WAIT");
				PB13 = 0;
         break;
			
      case SOCK_INIT : // Execute this when there are not any connection
         if( (ret = listen(sn)) != SOCK_OK) return ret;
				printf("IN IT");
				PB13 = 1;
         break;
			
      case SOCK_CLOSED:
         if((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn) return ret;
				printf("CLOSE");
					PB13 = 0;
         break;
			
      default:
         break;
   }
   return 1;
}

int32_t loopback_tcpcc(uint8_t sn, uint8_t* buf, uint8_t* destip, uint16_t destport)
{
	
   int32_t ret; // return value for SOCK_ERRORs
	
   // Port number for TCP client (will be increased)
   static uint16_t any_port = 	50000;
   // Socket Status Transitions
   // Check the W5500 Socket n status register (Sn_SR, The 'Sn_SR' controlled by Sn_CR command or Packet send/recv status)
   
	printf("CHECKING CLIENT");
	switch(getSn_SR(sn))
   {
      case SOCK_ESTABLISHED :
         if(getSn_IR(sn) & Sn_IR_CON)	// Socket n interrupt register mask; TCP CON interrupt = connection with peer is successful
         {
			setSn_IR(sn, Sn_IR_CON);  // this interrupt should be write the bit cleared to '1'
					 TIMER_Stop(TIMER0);
					 printf("ESTABLISH");
					 _CLIENT = true;
					 _SERVER = true;
					 MODE_DONE = true;
					 IMCLIENT = true;
					 	printf("I'M CLIENT");
					 
         }

         break;

      case SOCK_CLOSE_WAIT : // Execute this case when disconnect to port

         if((ret=disconnect(sn)) != SOCK_OK)
				 {
					 
					 return ret;
				 }
				 printf("CLOSE WAIT");
         break;

      case SOCK_INIT : // Execute this case when connect to port and ret = 1
    	 if( (ret = connect(sn, destip, destport)) != SOCK_OK) 
			 {
				 return ret;	//	Try to TCP connect to the TCP server (destination)
			 }
			 printf("INIT");
			 PB13 = 1; 
         break;

      case SOCK_CLOSED: // Execute this case when disconnect to port
    	  close(sn);
    	  if((ret=socket(sn, Sn_MR_TCP, any_port++, 0x00)) != sn){
         if(any_port == 0xffff) any_port = 50000;
         return ret; // TCP socket open with 'any_port' port number
        }
				printf("CLOSE");
				PB13 = 0;		
         break;
      default:
         break;
   }
   return 1;
}

				
volatile short x = 0;
volatile short y = 0;

bool waiting_state = true;
bool passstep = true;
bool My_turn = false;
uint8_t Num_of_Blocks_To_Finish = 16;
// Because position in map only from 00 -> 99 , are different from 100 , so i put 100 to compare 
uint8_t Check_Num_of_Blocks_To_Finish[16] = {100,100,100,100,100,100,
																						100,100,100,100,100,100,
																						100,100,100,100};
short Index_Support_Check_Num_of_Blocks_To_Finish = 0;
uint8_t Arr_Support_Check_Num_of_Blocks_To_Finish[16];

void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void vTask3( void *pvParameters );

// Handling send waiting when start
const char *pvTask1  = "Task1 is running.";

// Handling select x,y to SHOOT enemy
const char *pvTask2  = "Task2 is running.";

// Handling receive and feedback + display
const char *pvTask3  = "Task3 is running.";
		
SemaphoreHandle_t xSemaphore = NULL; // ##############Add this line if use semaphore


// 7 Ships (1:1x1 , 3:2x1 , 3:3x1) with 1block is(6x6 pixels)
uint8_t Ship1_Positions[2]=
{
	0,0					// x0y0
};

uint8_t Ship2_Positions[12]=
{
	1,5, 1,6,			// x1y5 x1y6
	9,7, 9,8,			// x7y7 x7y8
	1,9, 2,9			// x0y9 x1y9
};

uint8_t Ship3_Positions[18]=
{
	3,3, 3,4, 3,5,	// x3y3 x3y4 x3y5
	6,5, 6,6, 6,7,	// x5y5 x5y6 x5y7
	5,0, 6,0, 7,0		// x5y2 x6y2 x7y2
};


uint8_t rb(void)        
{
	SPI_WRITE_TX(SPI0, (uint8_t)0xff);
	/* Trigger SPI data transfer */
	SPI_TRIGGER(SPI0);
	/* Check SPI0 busy status */
	while(SPI_IS_BUSY(SPI0));
	return SPI_READ_RX(SPI0);
}

/**
 * @brief Default function to write in SPI interface.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
//void 	wizchip_spi_writebyte(uint8_t wb) {};
void 	wb(uint8_t wb)
 {
	/* Write to TX register */
  SPI_WRITE_TX(SPI0, (uint8_t)wb);
  /* Trigger SPI data transfer */
  SPI_TRIGGER(SPI0);
  /* Check SPI0 busy status */
  while(SPI_IS_BUSY(SPI0));
 }
 
void cs_sel() {
	PA14=0; //CS LOW
}
 
void cs_desel() {
	PA14=1; //CS HIGH
}

uint8_t BattleMap[]=
{
	
	// 1 Array block = 256 elements - ( 0->255)
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00, // 15
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00, // 31
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00, // 47
0x00,0x00,0x00,0xFF,0x1F,0xFC,0x7F,0xFF,0xFF,0xFE,0x3F,0xF8,0xFF,0x00,0x00,0x00, // 63
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 79
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 95
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 111
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 127
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 143
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 159
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 175
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 191
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 207
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 223
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 239
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 255
	
	// 256 -> 511
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 271
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 287
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 303
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 319
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 335
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 351
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 367
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 383
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 399
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 415
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 431
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 447
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 463
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 479
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 495
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 511

	// 512 -> 767
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 527
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 543
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 559
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 575
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 591
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 607
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 623
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 639
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 655
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 671
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 687
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 703
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 719
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 735
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 751
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 767

	// 768 -> 1023
	// x0 -> 1016(4L) 1017(2HH)
	// y0 -> 1000(4L) 1001(2HH), 984(4L) 985(2HH), 968(4L) 969(2HH), 952(4L) 953(2HH), 936(4L) 937(2HH), //920(4L) 921(2HH)
	// y1 -> 920(4L) 921(2HH), 904(4L) 905(2HH), 888(4L) 889(2HH), 872(4L) 873(2HH), 856(4L) 857(2HH), 830(4L) 831(2HH)
	
	// x1 -> 1017(2HL 4L)
	// y0 -> 1001(2HL 4L), 985(2HL 4L), 969(2HL 4L), 953(2HL 4L), 937(2HL 4L), //921(2HL 4L)
	// y1 -> 921,(2HL 4L), 905(2HL 4L), 889(2HL 4L), 873(2HL 4L), 857(2HL 4L), 841(2HL 4L)
	
	// x2 -> 1018(4H 2LH)
	// y0 -> 1002(4H 2LH), 986(4H 2LH), 970(2HL 4L), 954(2HL 4L), 938(2HL 4L), //922(2HL 4L)
	
	// x3 -> 1018(2LL) 1019(4H)
	// y0 -> 1002(2LL) 1003(4H), 986(2LL) 987(4H), 970(2LL) 971(4H), 954(2LL) 955(4H), 938(2LL) 939(4H), //922(2LL) 923(4H)
	
	// x4 -> 1019(4L) 1020(2HH)
	// y0 -> 1003(4L) 1004(2HH), 987(4L) 988(2HH), 971(4L) 972(2HH), 955(4L) 956(2HH), 939(4L) 940(2HH), //923(4L) 924(2HH)
	
	// x5 -> 1020(2HL 4L)
	// y0 -> 1004(2HL 4L), 988(2HL 4L), 972(2HL 4L), 956(2HL 4L), 940(2HL 4L), //924(2HL 4L)
	
	// x6 -> 1021(4H 2LH)
	// y0 -> 1005(4H 2LH), 989(4H 2LH), 973(4H 2LH), 957(4H 2LH), 941(4H 2LH), //925(4H 2LH)
	
	// x7 -> 1021(2LL) 1022(4H)
	// y0 -> 1005(2LL) 1006(4H), 989(2LL) 990(4H), 973(2LL) 974(4H), 957(2LL) 985(4H), 941(2LL) 942(4H), //925(2LL) 926(4H)
	
	// x8 -> 1022(4L) 1023(2HH)
	// y0 -> 1006(4L) 1007(2HH), 990(4L) 991(2HH), 974(4L) 975(2HH), 958(4L) 959(2HH), 942(4L) 943(2HH), //926(4L) 927(2HH)
	
	// x9 -> 1023(2HL 4L)
	// y0 -> 1007(2HL 4L), 991(2HL 4L), 975(2HL 4L), 959(2HL 4L), 943(2HL 4L), 927(2HL 4L)
	
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 783
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 799
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 815
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 831
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 847
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 863
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 879
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 895
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 911
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 927
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 943
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 959
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 975
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 991
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 1007
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 1023

};



void SYS_Init(void)
{
    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);
	
		CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HCLK_DIV2);
	
		CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));
	
		 /* Select HCLK as the clock source of SPI0 */
    CLK_SetModuleClock(SPI0_MODULE, CLK_CLKSEL1_SPI0_S_HCLK, MODULE_NoMsk);

		CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0);

		/* Enable SPI0 peripheral clock */
    CLK_EnableModuleClock(SPI0_MODULE);
		CLK_EnableModuleClock(UART0_MODULE);	
		CLK_EnableModuleClock(TMR0_MODULE);
		
		/* Set PB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
    SYS->GPB_MFP |= (SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD);
		
		 /* Setup SPI0 multi-function pins */
    SYS->GPC_MFP &= ~(SYS_GPC_MFP_PC0_Msk | SYS_GPC_MFP_PC1_Msk | SYS_GPC_MFP_PC2_Msk | SYS_GPC_MFP_PC3_Msk);
    SYS->GPC_MFP |= SYS_GPC_MFP_PC0_SPI0_SS0 | SYS_GPC_MFP_PC1_SPI0_CLK | SYS_GPC_MFP_PC2_SPI0_MISO0 | SYS_GPC_MFP_PC3_SPI0_MOSI0;
    SYS->ALT_MFP &= ~(SYS_ALT_MFP_PC0_Msk | SYS_ALT_MFP_PC1_Msk | SYS_ALT_MFP_PC2_Msk | SYS_ALT_MFP_PC3_Msk);
    SYS->ALT_MFP |= SYS_ALT_MFP_PC0_SPI0_SS0 | SYS_ALT_MFP_PC1_SPI0_CLK | SYS_ALT_MFP_PC2_SPI0_MISO0 | SYS_ALT_MFP_PC3_SPI0_MOSI0;


		SystemCoreClockUpdate();
}





void SPI_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init SPI                                                                                                */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Configure as a master, clock idle low, 32-bit transaction, drive output on falling clock edge and latch input on rising edge. */
    /* Set IP clock divider. SPI clock rate = 8MHz */
    SPI_Open(SPI0, SPI_MASTER, SPI_MODE_0, 8, 8000000);

    /* Enable the automatic hardware slave select function. Select the SS pin and configure as low-active. */
    //SPI_EnableAutoSS(SPI0, SPI_SS0, SPI_SS_ACTIVE_LOW);
		SPI_DisableAutoSS(SPI0);
}

void GPIO_Init()
{
		GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);	// Led
		
		// ####### BUTTON
		GPIO_SetMode(PA, BIT10,	GPIO_PMD_INPUT);
		GPIO_SetMode(PA, BIT11,	GPIO_PMD_INPUT);
		GPIO_SetMode(PA, BIT15,	GPIO_PMD_INPUT);
		GPIO_SetMode(PE, BIT5,	GPIO_PMD_INPUT);
		GPIO_SetMode(PB, BIT15,	GPIO_PMD_INPUT);
	
		// ####### ETHERNET
		GPIO_SetMode(PB,BIT11,GPIO_PMD_OUTPUT); //RST
		GPIO_SetMode(PA,BIT14,GPIO_PMD_OUTPUT);// CS	
		
		// ####### If you change LCD pins connection, you have also change define pin in LCDST7920.h ########
		GPIO_SetMode(PA, BIT9, GPIO_PMD_OUTPUT);	// RESET
		GPIO_SetMode(PA, BIT8, GPIO_PMD_OUTPUT);	// EN
		GPIO_SetMode(PA, BIT12, GPIO_PMD_OUTPUT);	// RWE
		GPIO_SetMode(PA, BIT13, GPIO_PMD_OUTPUT);	// RS
		
		GPIO_SetMode(PA, BIT0, GPIO_PMD_OUTPUT);	// D0
		GPIO_SetMode(PA, BIT1, GPIO_PMD_OUTPUT);	// D1
		GPIO_SetMode(PA, BIT2, GPIO_PMD_OUTPUT);	// D2
		GPIO_SetMode(PA, BIT3, GPIO_PMD_OUTPUT);	// D3
		GPIO_SetMode(PA, BIT4, GPIO_PMD_OUTPUT);	// D4
		GPIO_SetMode(PA, BIT5, GPIO_PMD_OUTPUT);	// D5
		
		GPIO_SetMode(PF, BIT4, GPIO_PMD_OUTPUT);	// D6
		GPIO_SetMode(PF, BIT5, GPIO_PMD_OUTPUT);	// D7
}







bool Find_My_Ship_Position(uint8_t pos_x, uint8_t pos_y)
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int combine_posx_posy_to_decimal = pos_x * 10 + pos_y;
	for(i = 0 ; i < 16; i++)
	{
		if(Check_Num_of_Blocks_To_Finish[i] == combine_posx_posy_to_decimal) // If this is one of ship position
		{
			for(j = 0 ; j < 16; j++)
			{
				if (Arr_Support_Check_Num_of_Blocks_To_Finish[j] == combine_posx_posy_to_decimal) // If this had existed before
					return true; // return true to display a 'BLOCK' for ENEMY instead of X
			}
			 // If had not existed before
			Num_of_Blocks_To_Finish--; // Counter ship --
			
			// Save to Arr Support for condition above
			Arr_Support_Check_Num_of_Blocks_To_Finish[Index_Support_Check_Num_of_Blocks_To_Finish] = combine_posx_posy_to_decimal;
			
			// Point to next element
			Index_Support_Check_Num_of_Blocks_To_Finish++;
			return true;
		}
	}
	return false;
}

bool Support_Avoid_Setup_Ship_Asthesame_Positions(uint8_t pos_x, uint8_t pos_y)
{
	unsigned int combine_posx_posy_to_decimal = pos_x * 10 + pos_y;
	unsigned int i = 0;
	for(i = 0; i < 16; i++)
		if(Check_Num_of_Blocks_To_Finish[i] == combine_posx_posy_to_decimal)
			return false;
	
	return true;
}

void My_UDP_Str_Send(uint8_t *str)
{
	int i = 0 ;
	while(str[i] != 0x00)
		i++;

	send(0,str,i);
}

void Setup_Myship()
{				
	uint8_t Ship_Block_Num = 1; // Start setup for Ship 1_1st
	short _x = 0;	// Position from button
	short _y = 0;	// Position from button
	unsigned int i = 0;
	bool Auto_Set_Ships = true;
	short index_ship2 = 0;
	short index_ship3 = 0;
	
	BattleMap[12] = 0x01; // Display SHIP INDICATION 1 
	Display_Select_XY_Number(_x,_y); // Display position 0 0 for the first time
		
	while(Ship_Block_Num <= 16)	// 16 block ( Ship1:1block , Ship2: (1block * 2) * 3 , Ship3: (1block * 3) * 3)
		{
			if(LEFTUP == 0)		// Increase X by one
			{
				Auto_Set_Ships = false;
				while(LEFTUP == 0){}
					_x++;
				
				if(_x == 10)
					_x = 0;
				Display_Select_XY_Number(_x,_y);
			}
			
			else if(LEFTDOWN == 0)// Decrease X by one
			{
				Auto_Set_Ships = false;
				while(LEFTDOWN == 0){}
					_x--;
				
				if(_x < 0)
					_x = 9;
				Display_Select_XY_Number(_x,_y);
			}
			
			else if(RIGHTUP == 0) // Increase Y by one
			{
				Auto_Set_Ships = false;
				while(RIGHTUP == 0){}
					_y++;
				
				if(_y == 10)
					_y = 0;
				
				Display_Select_XY_Number(_x,_y);
			}
			
			else if(RIGHTDOWN == 0) // Decrease Y by one
			{
				Auto_Set_Ships = false;
				while(RIGHTDOWN == 0){}
					_y--;
				
				if(_y < 0)
					_y = 9;
				
				Display_Select_XY_Number(_x,_y);
			}
			
			else if(OK == 0 && Auto_Set_Ships == false) // Press OK to setup that position
			{
				while(OK == 0){}
					if(Support_Avoid_Setup_Ship_Asthesame_Positions(_x,_y) == true)
					{
						Set_My_Ship_XY(_x,_y);	// Display ship on MAP
						if (Ship_Block_Num < 2)
						{
							BattleMap[12] = 0x03; // Display NEXT SHIP INDICATION
							Ship1_Positions[0] = _x; // Save x position to Ship1
							Ship1_Positions[1] = _y; // Save y position to Ship1
							Check_Num_of_Blocks_To_Finish[0] = _x * 10 + _y;
						}
						else if(Ship_Block_Num > 1 && Ship_Block_Num < 8)
						{
							Ship2_Positions[index_ship2] = _x; // Save x position to Ship1
							Ship2_Positions[index_ship2 + 1] = _y; // Save y position to Ship1
							Check_Num_of_Blocks_To_Finish[Ship_Block_Num - 1] = _x * 10 + _y;
							index_ship2 += 2;
							if(index_ship2 >= 12)
								BattleMap[12] = 0x07;	// Display NEXT SHIP INDICATION
							else BattleMap[12] = 0x03;	// Display SHIP INDICATION 2
						}
						else 
						{
							BattleMap[12] = 0x07; // Display SHIP INDICATION 3
							Ship3_Positions[index_ship3] = _x; // Save x position to Ship1
							Ship3_Positions[index_ship3 + 1] = _y; // Save y position to Ship1
							Check_Num_of_Blocks_To_Finish[Ship_Block_Num - 1] = _x * 10 + _y;
							index_ship3 += 2;
						}
						Ship_Block_Num++;	// Increase to next block of next ship
						Display_Select_XY_Number(_x,_y); // Display position selection
					}
			}
			else if(OK == 0 && Auto_Set_Ships == true)
			{
				//Setup 1Ship1
				Set_My_Ship_XY(Ship1_Positions[0],Ship1_Positions[1]);
				Check_Num_of_Blocks_To_Finish[0] = Ship1_Positions[0] * 10 + Ship1_Positions[1];
				
				//Setup 3Ship2
				for(i=0;i<=10;i=i+2)
					{
						Set_My_Ship_XY(Ship2_Positions[i],Ship2_Positions[i+1]);
						Check_Num_of_Blocks_To_Finish[Ship_Block_Num] = Ship2_Positions[i] * 10 + Ship2_Positions[i+1];
						Ship_Block_Num++; //Re-use this value to save RAM
					}
				
				//Setup 3Ship3
				for(i=0;i<=16;i=i+2)
					{
						Set_My_Ship_XY(Ship3_Positions[i],Ship3_Positions[i+1]);
						Check_Num_of_Blocks_To_Finish[Ship_Block_Num] = Ship3_Positions[i] * 10 + Ship3_Positions[i+1];
						Ship_Block_Num++; //Re-use this value to save RAM
					}
				
				
				break;
			}
		}
		
		// After setup ship, Hide the display of SHIP INDICATION
		for(i = 12 ; i < 45 ; i=i+16)
				BattleMap[i] = 0x00;
		
			Display_Graphic(BattleMap);
			
}



int main(void)
{		
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();
	
    /* Lock protected registers */
    SYS_LockReg();
		
		SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART0, 9600);
	
		SPI_Init();
		GPIO_Init();
	
		TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1);
    TIMER_EnableInt(TIMER0);				
		NVIC_EnableIRQ(TMR0_IRQn);
		TIMER_Start(TIMER0);
		

		InitLCD_Graphic();	
		Display_Graphic(BattleMap);	
	
		reg_wizchip_cs_cbfunc(cs_sel, cs_desel);
		reg_wizchip_spi_cbfunc(rb, wb);
	
		wizchip_init(bufSize, bufSize);
		
		while(MODE_DONE == false) // loop here until reach to SERVER or CLIENT
		{
			while(_SERVER == false)
			{
				if(SERVER_SETUP == false)
				{
					// RESET W5500
					PB11 = 1;
					delay(1000);
					PB11 = 0;
					delay(1000);
					PB11 = 1;
					delay(1000);


  wizchip_setnetinfo(&netInfoSV);
  wizchip_getnetinfo(&netInfotest);
					SERVER_SETUP = true;
					CLIENT_SETUP = false;
				}
				loopback_tcpss(0, (uint8_t *)"MASK", 5000);
				delay(2000);
			}
			
			while(_CLIENT == false)
			{
				if(CLIENT_SETUP == false)
				{
					// RESET W5500
					PB11 = 1;
					delay(1000);
					PB11 = 0;
					delay(1000);
					PB11 = 1;
					delay(1000);

  wizchip_setnetinfo(&netInfoCL);
  wizchip_getnetinfo(&netInfotest);
	
	CLIENT_SETUP = true;
	SERVER_SETUP = false;
				}
				loopback_tcpcc(0, (uint8_t *)"MASK", distIPK, 5000);
				delay(2000);
			}
		}
		
		// After get SERVER or CLIENT, system will operate FREERTOS to handle 3 task
		xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, (void*)pvTask1, 1, NULL );								
		xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, (void*)pvTask2, 1, NULL );
		xTaskCreate( vTask3, "Task 3", configMINIMAL_STACK_SIZE, (void*)pvTask3, 1, NULL );
		
		
	/*###############  Add 2 line below to use semaphore*/
	xSemaphore = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphore);

  vTaskStartScheduler();
		
		while(1)
		{
		}
	return 0;

}



void vTask1( void *pvParameters )
{
		while(1)
		{
			xSemaphoreTake(xSemaphore,(TickType_t) portMAX_DELAY);
			
			if(waiting_state == true)
				My_UDP_Str_Send((uint8_t *)"waiting");
			
			xSemaphoreGive(xSemaphore);
      vTaskDelay(4000/portTICK_RATE_MS);
    }
}

/*-----------------------------------------------------------*/
void vTask2( void *pvParameters )
{
		while(1)
		{
			xSemaphoreTake(xSemaphore,(TickType_t) portMAX_DELAY);			
			
			if(My_turn == true)
			{
				Display_Select_XY_Number(x,y);
				while(My_turn == true)
			{
			
			if(LEFTUP == 0)
			{
				while(LEFTUP == 0){}
					x++;
				
				if(x == 10)
					x = 0;
				Display_Select_XY_Number(x,y);
			}
			
			else if(RIGHTUP == 0)
			{
				while(RIGHTUP == 0){}
					y++;
				
				if(y == 10)
					y = 0;
				Display_Select_XY_Number(x,y);
			}
			
			else if(LEFTDOWN == 0)
			{
				while(LEFTDOWN == 0){}
					x--;
				
				if(x < 0)
					x = 9;
				Display_Select_XY_Number(x,y);
			}
			
			else if(RIGHTDOWN == 0)
			{
				while(RIGHTDOWN == 0){}
					y--;
				
				if(y < 0)
					y = 9;
				Display_Select_XY_Number(x,y);
			}
			
			else if(OK == 0)
			{
				while(OK == 0){}		
				SENDBUF[0] = 'S'; 
				SENDBUF[1] = 'H';
				SENDBUF[2] = x + '0';
				SENDBUF[3] = y + '0';
				My_UDP_Str_Send(SENDBUF);
				Destroy_Enemy_Ship_XY(x,y);
				My_turn = false;
				Display_Turn(My_turn);	
			}
				
			}
			}

			xSemaphoreGive(xSemaphore);
      vTaskDelay(1/portTICK_RATE_MS);  
    }
} 

void vTask3( void *pvParameters )
{
    uint8_t datarev[10];
		uint8_t i = 0;
		while(1)
		{

			xSemaphoreTake(xSemaphore,(TickType_t) portMAX_DELAY);			
			
				recv(0, datarev, 10);	
			

						if(datarev[0] == 'R')
						{
						
							if(datarev[3] == '1')
								Set_Enemy_Ship_XY(x,y);
							else if(datarev[3] == '0')
								Destroy_Enemy_Ship_XY(x,y);
							My_turn = false;
							Display_Turn(My_turn);
						}
						
						else if(datarev[0] == 'S')
						{
							uint8_t x_pos_receive = datarev[2] - '0';
							uint8_t y_pos_receive = datarev[3] - '0';	printf("RESNE");
							if(Find_My_Ship_Position( x_pos_receive,y_pos_receive) == true)
								My_UDP_Str_Send((uint8_t *)"RES1");
							else 
								My_UDP_Str_Send((uint8_t *)"RES0");
								
								if(Num_of_Blocks_To_Finish == 0)
								{
									My_UDP_Str_Send((uint8_t *)"WIN");
									
									InitLCD_BasicMode();
									Display_BasicMode("LOSE");
								}
								else 
								{
								
									Destroy_My_Ship_XY(x_pos_receive,y_pos_receive);
									My_turn = true;
									Display_Turn(My_turn);
								}
								
						}
						
						if(datarev[0] == 'w' && passstep == true)
						{
							waiting_state = false;
							My_UDP_Str_Send((uint8_t *)"accepted");
						}
						else if(datarev[0] == 'a' && passstep == true)
						{
							passstep = false; // Support for ETHERNET RECEIVING below
							My_UDP_Str_Send((uint8_t *)"setup");
							Setup_Myship();
							My_turn = true;
							Display_Turn(My_turn);
						}
						else if(datarev[0] == 's' && passstep == true)
						{
							Setup_Myship();
							My_turn = false;
							Display_Turn(My_turn);
						}
						else if(datarev[0] == 'W')
						{
							InitLCD_BasicMode();
							Display_BasicMode("WIN");
						}
						
						// Clear buffer, but it doesn't work :)
						for(i = 0 ; i < 10 ; i++)
							datarev[i] = '0';

			xSemaphoreGive(xSemaphore);
      vTaskDelay(1000/portTICK_RATE_MS);  
    }
}
