#include <PCD8544F0.h>
uint8_t GBuf[GBufS];

GPIO_InitTypeDef G;
SPI_InitTypeDef S;

void PCD8544_InitSetup(){
	static uint8_t Init = 1;

	if(Init==1){
		Init = 0;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

		G.GPIO_Pin = DC | CE | RS;
		G.GPIO_Mode = GPIO_Mode_OUT;
		G.GPIO_OType = GPIO_OType_PP;
		G.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(IOGPIO, &G);

		G.GPIO_Pin = VCC;
		GPIO_Init(VCCGPIO, &G);

		G.GPIO_Pin = Clk | DIn;
		G.GPIO_Mode = GPIO_Mode_AF;
		GPIO_Init(IOGPIO, &G);

		GPIO_PinAFConfig(GPIOA, ClkPS, GPIO_AF_0);
		GPIO_PinAFConfig(GPIOA, DInPS, GPIO_AF_0);

		G.GPIO_Pin = BL1 | BL2;
		G.GPIO_Mode = GPIO_Mode_OUT;
		G.GPIO_OType = GPIO_OType_OD;
		GPIO_Init(IOGPIO, &G);

		S.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
		S.SPI_CPHA = SPI_CPHA_1Edge;
		S.SPI_CPOL = SPI_CPOL_Low;
		S.SPI_DataSize = SPI_DataSize_8b;
		S.SPI_FirstBit = SPI_FirstBit_MSB;
		S.SPI_Mode = SPI_Mode_Master;
		S.SPI_NSS = SPI_NSS_Soft;
		SPI_Init(SPI1, &S);
		SPI_Cmd(SPI1, ENABLE);
	}

	GPIO_ResetBits(IOGPIO, DC|CE|RS);
	//Turn on screen
	GPIO_SetBits(VCCGPIO, VCC);
	Delay(50);
	GPIO_SetBits(IOGPIO, RS);
	GPIO_SetBits(IOGPIO, CE);
	Delay(100);
	SB(0x21, 0, 1);			//Power screen on, extended instruction
	SB(0x06, 0, 1);			//Temp co 2
	SB(0x13, 0, 1);			//Bias system 3
	SB(0x80|70, 0, 1);		//Contrast 55-5v, 64-3v

	SB(0x20, 0, 1);
	SB(0x80, 0, 1);			//X Address = 0
	SB(0x0C, 0, 1);			//Normal display
	//SB(0b1001, 0, 1);		//All display

	ClrBuf();					//Clear Display
	PScrn();					//Write display
}

void ScreenOn(void){
	PCD8544_InitSetup();
}

void ScreenOff(void){
	ClrBuf();					//Clear Display
	PScrn();					//Write display
	BacklightIO(0);
	GPIO_ResetBits(IOGPIO, Clk|DIn|DC|CE|RS);
	GPIO_ResetBits(IOGPIO, VCC);
}

void NormalMode(void){
	SB(0x0C, 0, 1);			//Normal display
}

void InvertMode(void){
	SB(0x0D, 0, 1);			//Normal display
}

uint8_t BacklightIO(uint8_t BL){
	//BL1 220ohm
	//BL2 1kohm
	switch(BL){
	case 0:
		GPIO_SetBits(IOGPIO, BL1 | BL2);
		break;
	case 1:
		GPIO_ResetBits(IOGPIO, BL2);
		GPIO_SetBits(IOGPIO, BL1);
		break;
	case 2:
		GPIO_ResetBits(IOGPIO, BL1);
		GPIO_SetBits(IOGPIO, BL2);
		break;
	case 3:
		GPIO_ResetBits(IOGPIO, BL1);
		GPIO_ResetBits(IOGPIO, BL2);
		break;
	}
	return BL;
}

void SB(uint8_t Dat, uint8_t CD, uint8_t SC){
	uint16_t Cnt;

	GPIO_WriteBit(IOGPIO, DC, CD);
	GPIO_ResetBits(IOGPIO, Clk|DIn);
	if(SC) GPIO_ResetBits(IOGPIO, CE);
	/*

	for(Cnt = 128; Cnt>0; Cnt>>=1){
		if(Dat&Cnt) GPIO_SetBits(IOGPIO, DIn);
		else GPIO_ResetBits(IOGPIO, DIn);

		GPIO_SetBits(IOGPIO, Clk);
		GPIO_ResetBits(IOGPIO, Clk);
	}
	 */
	SPI_SendData8(SPI1, Dat);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

	GPIO_ResetBits(IOGPIO, Clk|DIn);
	if(SC)GPIO_SetBits(IOGPIO, CE);
}

void PScrn(void){
	uint16_t Cnt;

	SB(0x40, 0, 1);
	SB(0x80, 0, 1);
	GPIO_ResetBits(IOGPIO, CE);
	for(Cnt = 0; Cnt<((XPix*YPix)>>3); Cnt++){
		SB(GBuf[Cnt], 1, 0);
	}
	GPIO_SetBits(IOGPIO, CE);
	SB(0x40, 0, 1);
	SB(0x80, 0, 1);
}

void Delay(uint32_t ms){
	volatile uint32_t cnt;
	ms*=650;
	for(cnt = 0; cnt<ms; cnt++) __NOP;
}
