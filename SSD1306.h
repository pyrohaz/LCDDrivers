#ifndef SSD1306_H
#define SSD1306_H

#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_spi.h>
#include <string.h>
#include <inttypes.h>

//Non-SPI
//#define Clk GPIO_Pin_0
//#define DIn GPIO_Pin_1
#define Clk GPIO_Pin_5
#define DIn GPIO_Pin_7
#define DC GPIO_Pin_2
#define CE GPIO_Pin_3
#define RS GPIO_Pin_4
#define VCC GPIO_Pin_0

#define ClkPS GPIO_PinSource5
#define DInPS GPIO_PinSource7

#define IOGPIO GPIOA

#define NormDisp 0xA6
#define InvDisp 0xA7
#define DispOnRAM 0xA4
#define DispOnAll 0xA5
#define DispOff 0xAE
#define DispOn 0xAF
#define Contrast 0x81

#define SetRemap 0xA0
#define SetMuxRatio 0xA8
#define SetComScanDir 0xC0
#define SetDispOff 0xD3
#define SetComPinHW 0xDA
#define SetDispFreq 0xD5
#define SetPreChrg 0xD9
#define ChargePump 0x8D
#define SetComHLvl 0xDB

#define SetDispOffset 0xD3
#define SetDispStartLine 0x40

#define SetLowAdd 0x00
#define SetHighAdd 0x10
#define MemAddMode 0x20
#define MModeH 0x00
#define MModeV 0x01
#define MModeP 0x02
#define PageStrtAdd 0xB0
#define SetColAdd 0x21
#define SetPageAdd 0x22

#define XPix 128
#define YPix 64
#define GBufS (XPix*YPix)/8

void InitSetup(void);

void TestScreenMode(uint8_t);
void InvertMode(void);
void NormalMode(void);

void SB(uint8_t, uint8_t, uint8_t);
void Delay(uint32_t);

uint8_t WritePix(int16_t, int16_t, uint8_t);
void PScrn(void);
void ClrBuf(void);

extern uint8_t GBuf[GBufS];

#endif
