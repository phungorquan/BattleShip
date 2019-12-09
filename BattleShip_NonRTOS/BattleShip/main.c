#include "LCDST7920.h" // Get NUC131 and LCD functions
#include "BattleMap.h" // Get ship functions
#include "ENC28J60.h"
#include "ip_arp_udp_tcp.h"
#include "net.h"
#include "stdbool.h"
#define PLL_CLOCK   50000000
#define PSTR(s) s

#define LEFTUP PA10
#define RIGHTUP PE5
#define LEFTDOWN PA11
#define RIGHTDOWN PA15
#define OK PB15

#define Board1
//#define Board2

#ifdef Board1
#define MySrcPort 5001
#define MyDesPort 0x64
#define DesPort 0x65
#endif

#ifdef Board2
#define MySrcPort 5002
#define MyDesPort 0x65
#define DesPort 0x64
#endif


#ifdef Board1
static uint8_t mymac[6] ={0x00,0x1b,0x77,0x71,0x48,0xf1}; 			//dia chi mac enc28j60
static uint8_t myip[4] ={192, 168, 0, 116};                    //dia chi IP cua enc28j60

//static uint8_t dis_mac[6] = {0x1C,0x65,0x9D,0x58,0x97,0xF3}; // dia chi mac may tinh // "ipconfig /all" roi tim cai MAC cua card WIfi ay
static uint8_t dis_mac[6] ={0x00,0x1b,0x77,0x71,0x48,0xf0};		
static uint8_t dis_ip[4] = {192,168,0,100};                   // IP may tinh
#endif

#ifdef Board2
static uint8_t mymac[6] ={0x00,0x1b,0x77,0x71,0x48,0xf0}; 			//dia chi mac enc28j60
static uint8_t myip[4] ={192, 168, 0, 100};                    //dia chi IP cua enc28j60

static uint8_t dis_mac[6] ={0x00,0x1b,0x77,0x71,0x48,0xf1};		
static uint8_t dis_ip[4] = {192,168,0,116};                   // IP may tinh
#endif




static uint8_t sendbuf[100 + 1];
static uint8_t receivebuf[100 + 1];

bool waiting_state = true;
bool setup_state = true;
bool start_state = true;
bool My_turn = false;
uint8_t Num_of_Blocks_To_Finish = 16;

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

uint8_t BattleMap[]=
{
	
	// 1 Array block = 256 elements - ( 0->255)
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 15
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 31
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 47
0x00,0x00,0x00,0xFF,0x1F,0xFC,0x7F,0xFB,0x9F,0xFE,0x3F,0xF8,0xFF,0x00,0x00,0x00, // 63
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 79
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 95
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 111
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 127
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 143
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 159
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 175
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 191
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 207
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 223
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 239
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 255
	
	// 256 -> 511
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 271
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 287
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 303
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 319
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 335
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 351
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 367
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 383
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 399
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 415
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 431
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 447
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 463
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 479
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 495
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 511

	// 512 -> 767
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 527
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 543
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 559
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 575
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 591
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 607
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 623
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 639
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 655
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 671
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 687
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 703
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 719
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 735
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 751
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
	
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 783
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 799
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 815
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 831
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 847
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 863
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 879
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 895
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 911
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 927
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 943
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 959
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 975
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 991
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 1007
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 1023

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
		
		 /* Select HCLK as the clock source of SPI0 */
    CLK_SetModuleClock(SPI0_MODULE, CLK_CLKSEL1_SPI0_S_HCLK, MODULE_NoMsk);

		/* Enable SPI0 peripheral clock */
    CLK_EnableModuleClock(SPI0_MODULE);

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
		GPIO_SetMode(PA,BIT14,GPIO_PMD_OUTPUT); //CS	
		
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

bool Find_My_Ship_Position(uint8_t pos_x, uint8_t pos_y)
{
	unsigned int i = 0;
	if(pos_x == Ship1_Positions[0])
		if(pos_y == Ship1_Positions[1])
		{
			Num_of_Blocks_To_Finish--;
			return true;
		}
	
	for(i = 0 ; i<= 10 ; i=i+2)
		{
			if(pos_x == Ship2_Positions[i])
				if(pos_y == Ship2_Positions[i+1])
					{
						Num_of_Blocks_To_Finish--;
						return true;
					}
		}
	
	for(i = 0 ; i<=16 ; i=i+2)
		{
			if(pos_x == Ship3_Positions[i])
				if(pos_y == Ship3_Positions[i+1])
					{
						Num_of_Blocks_To_Finish--;
						return true;
					}
		}
		
	return false;
}

void Setup_Myship()
{				
	uint8_t Ship_Block_Num = 1; // Start setup for Ship 1_1st
	short x = 0;	// Position from button
	short y = 0;	// Position from button
	unsigned int i = 12;	// Array element of SHIP INDICATION  -(1) --(2) ---(3)
	bool Set_Default_Ships = true;
	
	BattleMap[12] = 0x01; // Display SHIP INDICATION 1 
	Select_XY_Number(x,y); // Display position 0 0 for the first time
		
	while(Ship_Block_Num <= 16)	// 16 block ( Ship1:1block , Ship2: (1block * 2) * 3 , Ship3: (1block * 3) * 3)
		{
			if(LEFTUP == 0)		// Increase X by one
			{
				Set_Default_Ships = false;
				while(LEFTUP == 0){}
					x++;
				
				if(x == 10)
					x = 0;
				Select_XY_Number(x,y);
			}
			
			else if(LEFTDOWN == 0)// Decrease X by one
			{
				Set_Default_Ships = false;
				while(LEFTDOWN == 0){}
					x--;
				
				if(x < 0)
					x = 0;
				Select_XY_Number(x,y);
			}
			
			else if(RIGHTUP == 0) // Increase Y by one
			{
				Set_Default_Ships = false;
				while(RIGHTUP == 0){}
					y++;
				
				if(y == 10)
					y = 0;
				
				Select_XY_Number(x,y);
			}
			
			else if(RIGHTDOWN == 0) // Decrease Y by one
			{
				Set_Default_Ships = false;
				while(RIGHTDOWN == 0){}
					y--;
				
				if(y < 0)
					y = 0;
				
				Select_XY_Number(x,y);
			}
			
			else if(OK == 0 && Set_Default_Ships == false) // Press OK to setup that position
			{
				while(OK == 0){}				
					Set_My_Ship_XY(x,y);	// Display ship on MAP
					x = 0;			// Reset position for next ship
					y = 0;			// Reset position for next ship
					Ship_Block_Num++;	// Increase to next block of next ship
					if(Ship_Block_Num < 8) // Ignored SHIP INDICATION 1 because displayed above, 
						BattleMap[12] = 0x03;	// Display SHIP INDICATION 2
					else BattleMap[12] = 0x07; // Display SHIP INDICATION 3
					
					Select_XY_Number(x,y); // Display position
			}
			else if(OK == 0 && Set_Default_Ships == true)
			{
				//Setup Ship1
				Set_My_Ship_XY(Ship1_Positions[0],Ship1_Positions[1]);
				
				//Setup 3Ship2
				for(i=0;i<=10;i=i+2)
					Set_My_Ship_XY(Ship2_Positions[i],Ship2_Positions[i+1]);
				
				//Setup 3Ship3
				for(i=0;i<=16;i=i+2)
					Set_My_Ship_XY(Ship3_Positions[i],Ship3_Positions[i+1]);
				
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
	
		volatile	uint16_t plen = 0,payloadlen=0;
    volatile uint8_t cmd_pos = 0;
    char cmdval[30];
		short int x = 0;
		short int y = 0;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();
	
    /* Lock protected registers */
    SYS_LockReg();
		
		SPI_Init();
		GPIO_Init();
		
		// Reset ETHERNET
		PB11 = 1;
    delay(1000);
    PB11 = 0;
    delay(1000);
    PB11 = 1;
    delay(1000);
		
		enc28j60Init(mymac);
		delay(1500);
		enc28j60PhyWrite(PHLCON, 0x7a4);//0x476);
		enc28j60clkout(2); // Thay doi clkout 6.25MHz den 12.5MHz
		
		delay(1500);// giu cham de khi ngat nguon va bat lai khong phai an nut NRST
	
		init_ip_arp_udp_tcp(mymac, myip);
		enc28j60getrev();
		
		
		//UART0_Init();
		//UART_EnableInt(UART0, UART_IER_RDA_IEN_Msk);	
	
		InitLCD_Graphic();	   
		
		Display_Graphic(BattleMap);
		
		PB13=0;
		while(1)
		{
			
			if(My_turn == true)
			{
				Select_XY_Number(0,0);
				while(My_turn == true)
			{
			
			if(LEFTUP == 0)
			{
				while(LEFTUP == 0){}
					x++;
				
				if(x == 10)
					x = 0;
				Select_XY_Number(x,y);
			}
			
			else if(RIGHTUP == 0)
			{
				while(RIGHTUP == 0){}
					y++;
				
				if(y == 10)
					y = 0;
				Select_XY_Number(x,y);
			}
			
			else if(LEFTDOWN == 0)
			{
				while(LEFTDOWN == 0){}
					x--;
				
				if(x < 0)
					x = 0;
				Select_XY_Number(x,y);
			}
			
			else if(RIGHTDOWN == 0)
			{
				while(RIGHTDOWN == 0){}
					y--;
				
				if(y < 0)
					y = 0;
				Select_XY_Number(x,y);
			}
			
			else if(OK == 0)
			{
				while(OK == 0){}				
				sendbuf[UDP_DATA_P]='S';
				sendbuf[UDP_DATA_P+1]='H';
				sendbuf[UDP_DATA_P+2]=x + '0';
				sendbuf[UDP_DATA_P+3]=y + '0';
				send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);	
				send_udp_transmit(sendbuf,4);
				Destroy_Enemy_Ship_XY(x,y);
				My_turn = false;
				Display_Turn(My_turn);	
			}
				
			}
			}
			
				if(waiting_state == true)
			{
				sendbuf[UDP_DATA_P]='w';
				sendbuf[UDP_DATA_P+1]='a';
				sendbuf[UDP_DATA_P+2]='i';
				sendbuf[UDP_DATA_P+3]='t';
				sendbuf[UDP_DATA_P+4]='i';
				sendbuf[UDP_DATA_P+5]='n';
				sendbuf[UDP_DATA_P+6]='g';
				send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);	
				send_udp_transmit(sendbuf,7);
				delay(20000);
			}

				plen = enc28j60PacketReceive(100, receivebuf);
				
			 if (plen == 0)
      {
         continue;
      }
      // arp is broadcast if unknown but a host may also
      // verify the mac address by sending it to 
      // a unicast address.
      if (eth_type_is_arp_and_my_ip(receivebuf, plen))
      {
         make_arp_answer_from_request(receivebuf);

         continue;
      }

      // check if ip packets are for us:
      if (eth_type_is_ip_and_my_ip(receivebuf, plen) == 0)
      {
         continue;
      }


      if (receivebuf[IP_PROTO_P] == IP_PROTO_ICMP_V &&
         receivebuf[ICMP_TYPE_P] == ICMP_TYPE_ECHOREQUEST_V)
      {
         // a ping packet, let's send pong	
         make_echo_reply_from_request(receivebuf, plen);
         continue;
      }
			
			if (receivebuf[IP_PROTO_P] == IP_PROTO_UDP_V &&
         receivebuf[UDP_DST_PORT_H_P] == 0 &&
         receivebuf[UDP_DST_PORT_L_P] == MyDesPort)
      {
					PB13=0;
					delay(1000);
					PB13=1;
					delay(1000);
				
         payloadlen = receivebuf[UDP_LEN_L_P] - UDP_HEADER_LEN;
				 cmd_pos=0;

					while (cmd_pos < payloadlen)
            {
							 cmdval[cmd_pos]=receivebuf[UDP_DATA_P + cmd_pos]; 
               cmd_pos++;
						}
				
						if(cmdval[0] == 'R')
						{
							if(cmdval[3] == '1')
								Set_Enemy_Ship_XY(x,y);
							else if(cmdval[3] == '0')
								Destroy_Enemy_Ship_XY(x,y);
							x=0;
							y=0;
							My_turn = false;
							Display_Turn(My_turn);
						}
						
						else if(cmdval[0] == 'S')
						{
							volatile uint8_t x_pos_receive = cmdval[2] - '0';
							volatile uint8_t y_pos_receive = cmdval[3] - '0';
							if(Find_My_Ship_Position( x_pos_receive,y_pos_receive) == true)
								sendbuf[UDP_DATA_P+3]='1';
							else 
								sendbuf[UDP_DATA_P+3]='0';
								
								if(Num_of_Blocks_To_Finish == 0)
								{
									sendbuf[UDP_DATA_P]='W';
									sendbuf[UDP_DATA_P+1]='I';
									sendbuf[UDP_DATA_P+2]='N';
									send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);	
									send_udp_transmit(sendbuf,3);
									
									InitLCD_BasicMode();
									Display_BasicMode("LOSE");
								}
								else 
								{
									sendbuf[UDP_DATA_P]='R';
									sendbuf[UDP_DATA_P+1]='E';
									sendbuf[UDP_DATA_P+2]='S';
								
									send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);	
									send_udp_transmit(sendbuf,4);
							
									Destroy_My_Ship_XY(x_pos_receive,y_pos_receive);
									x=0;
									y=0;
									My_turn = true;
									Display_Turn(My_turn);
								}
								
						}
						
						
						
						else if(cmdval[0] == 'w')// && waiting_state == true)
						{
							waiting_state = false;
							sendbuf[UDP_DATA_P]='a';
							sendbuf[UDP_DATA_P+1]='c';
							sendbuf[UDP_DATA_P+2]='c';
							sendbuf[UDP_DATA_P+3]='e';
							sendbuf[UDP_DATA_P+4]='p';
							sendbuf[UDP_DATA_P+5]='t';
							sendbuf[UDP_DATA_P+6]='e';
							sendbuf[UDP_DATA_P+7]='d';
							send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);		
							send_udp_transmit(sendbuf,8);
						}
//						else if(cmdval[0] == 'w' && waiting_state == false)
//						{
//							sendbuf[UDP_DATA_P]='a';
//							sendbuf[UDP_DATA_P+1]='c';
//							sendbuf[UDP_DATA_P+2]='c';
//							sendbuf[UDP_DATA_P+3]='e';
//							sendbuf[UDP_DATA_P+4]='p';
//							sendbuf[UDP_DATA_P+5]='t';
//							sendbuf[UDP_DATA_P+6]='e';
//							sendbuf[UDP_DATA_P+7]='d';
//							send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);	
//							send_udp_transmit(sendbuf,8);
//						}
						else if(cmdval[0] == 'a')// && setup_state == true)
						{
							//setup_state = false;
							sendbuf[UDP_DATA_P]='s';
							sendbuf[UDP_DATA_P+1]='e';
							sendbuf[UDP_DATA_P+2]='t';
							sendbuf[UDP_DATA_P+3]='u';
							sendbuf[UDP_DATA_P+4]='p';
							send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);		
							send_udp_transmit(sendbuf,5);
						}
//						else if(cmdval[0] == 'a' && setup_state == false)
//						{
//							sendbuf[UDP_DATA_P]='s';
//							sendbuf[UDP_DATA_P+1]='e';
//							sendbuf[UDP_DATA_P+2]='t';
//							sendbuf[UDP_DATA_P+3]='u';
//							sendbuf[UDP_DATA_P+4]='p';
//							send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);		
//							send_udp_transmit(sendbuf,5);
//						}
						
						else if(cmdval[1] == 'e' && setup_state == true)
						{
							setup_state = false;
							//SET UP SHIP HERE
							Setup_Myship();
							
							sendbuf[UDP_DATA_P]='s';
							sendbuf[UDP_DATA_P+1]='e';
							sendbuf[UDP_DATA_P+2]='t';
							sendbuf[UDP_DATA_P+3]='u';
							sendbuf[UDP_DATA_P+4]='p';
							send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);		
							send_udp_transmit(sendbuf,5);
							
							// ASSUME AFTER SETUP WE PRESS OK TO SEND START
							sendbuf[UDP_DATA_P]='s';
							sendbuf[UDP_DATA_P+1]='t';
							sendbuf[UDP_DATA_P+2]='a';
							sendbuf[UDP_DATA_P+3]='r';
							sendbuf[UDP_DATA_P+4]='t';
							send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);		
							send_udp_transmit(sendbuf,5);
							//start_state = false;

						}
						
						else if(cmdval[1] == 'e' && setup_state == false)
						{
							
							sendbuf[UDP_DATA_P]='s';
							sendbuf[UDP_DATA_P+1]='t';
							sendbuf[UDP_DATA_P+2]='a';
							sendbuf[UDP_DATA_P+3]='r';
							sendbuf[UDP_DATA_P+4]='t';
							send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);		
							send_udp_transmit(sendbuf,5);
						}
						
						else if(cmdval[1] == 't' && setup_state == false)
						{
							delay(10000);
							sendbuf[UDP_DATA_P]='m';
							sendbuf[UDP_DATA_P+1]='y';
							sendbuf[UDP_DATA_P+2]='t';
							sendbuf[UDP_DATA_P+3]='u';
							sendbuf[UDP_DATA_P+4]='r';
							sendbuf[UDP_DATA_P+5]='n';
							send_udp_prepare(sendbuf,MySrcPort,dis_ip,DesPort,dis_mac);		
							send_udp_transmit(sendbuf,6);
							
							//Support_GetMyturn = true;
							My_turn = true;
							Display_Turn(My_turn);
						}
						
//						else if(cmdval[0] == 'm')
//						{
//							My_turn = false;
//							Display_Turn(My_turn);
//						}
						else if(cmdval[0] == 'W')
						{
							InitLCD_BasicMode();
							Display_BasicMode("WIN");
						}
					
				}
			
		}
}
