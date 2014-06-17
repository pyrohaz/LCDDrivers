#include <MAX7219.h>

GPIO_InitTypeDef G;
SPI_InitTypeDef S;

uint8_t GBuf[8];

void InitSetup(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	G.GPIO_Pin = Clk | DIn;
	G.GPIO_Mode = GPIO_Mode_AF;
	G.GPIO_Speed = GPIO_Speed_50MHz;
	G.GPIO_OType = GPIO_OType_PP;
	G.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(IOGPIO, &G);

	G.GPIO_Pin = CS;
	G.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(IOGPIO, &G);

	GPIO_PinAFConfig(IOGPIO, ClkPS, GPIO_AF_0);
	GPIO_PinAFConfig(IOGPIO, DInPS, GPIO_AF_0);

	S.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	S.SPI_CPHA = SPI_CPHA_1Edge;
	S.SPI_CPOL = SPI_CPOL_Low;
	S.SPI_DataSize = SPI_DataSize_16b;
	S.SPI_Direction = SPI_Direction_1Line_Tx;
	S.SPI_FirstBit - SPI_FirstBit_MSB;
	S.SPI_Mode = SPI_Mode_Master;
	S.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &S);
	SPI_Cmd(SPI1, ENABLE);

	SWd(Shutdown|1);
	SWd(DecMode|0);
	SWd(ScanLimit|7);
	SWd(DispTest|0);
	SWd(Intensity|0);

	//Clear display
	ClrBuf();
	PrintBuf();
}

int8_t SetIntensity(uint8_t IV){
	if(IV>15) return -1;
	SWd(Intensity|IV);

	return 0;
}

void SWd(uint32_t Word){
	//int32_t Cnt;

	GPIO_ResetBits(IOGPIO, CS);
	/*
	for(Cnt = 15; Cnt>-1; Cnt--){
		GPIO_ResetBits(GPIOB, Clk);
		if(Word&(1<<Cnt)) GPIO_SetBits(GPIOB, DIn);
		else GPIO_ResetBits(GPIOB, DIn);
		GPIO_SetBits(GPIOB, Clk);
	}
	*/
	//GPIO_ResetBits(GPIOB, Clk);
	SPI_I2S_SendData16(SPI1, Word);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	GPIO_SetBits(IOGPIO, CS);
}

void PrintBuf(void){
	uint8_t BCnt;

	for(BCnt = 1; BCnt<9; BCnt++){
		SWd((BCnt<<8)|GBuf[BCnt-1]);
	}
}

void WritePix(uint8_t X, uint8_t Y, uint8_t V){

	uint8_t CPixV = GBuf[Y];

	if(V) CPixV |= 1<<X;
	else CPixV &= ~(1<<X);

	GBuf[Y] = CPixV;
}

void WriteBuf(uint8_t V){
	uint8_t n;
	for(n = 0; n<8; n++) GBuf[n] = V;
}

void SetBuf(uint8_t V){
	WriteBuf(V);
}

void ClrBuf(){
	WriteBuf(0);
}

void SetPix(uint8_t X, uint8_t Y){
	WritePix(X, Y, 1);
}

void ClrPix(uint8_t X, uint8_t Y){
	WritePix(X, Y, 0);
}

void WriteCol(uint8_t Col, uint8_t V){
	GBuf[Col] = V;
}

void SetCol(uint8_t Col){
	GBuf[Col] = 255;
}

void ClrCol(uint8_t Col){
	GBuf[Col] = 0;
}

void WriteRow(uint8_t Row, uint8_t V){
	uint8_t CntB;
	for(CntB = 0; CntB<8; CntB++){
		if(V&(1<<CntB)) SetPix(Row, CntB);
		else ClrPix(Row, CntB);
	}
}

void SetRow(uint8_t Row){
	uint8_t Cnt;
	for(Cnt = 0; Cnt<8; Cnt++){
		SetPix(Row, Cnt);
	}
}

void ClrRow(uint8_t Row){
	uint8_t Cnt;
	for(Cnt = 0; Cnt<8; Cnt++){
		ClrPix(Row, Cnt);
	}
}

void Delay(uint32_t ms){
	uint32_t n;
	ms*=650;
	for(n = 0; n<ms; n++);
}
