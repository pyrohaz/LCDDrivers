#ifndef MAX7219_H
#define MAX7219_H

#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_spi.h>

#define NoOp 0<<8
#define DecMode 9<<8
#define Intensity 10<<8
#define ScanLimit 11<<8
#define Shutdown 12<<8
#define DispTest 15<<8

#define Clk GPIO_Pin_5
#define CS GPIO_Pin_6
#define DIn GPIO_Pin_7

#define ClkPS GPIO_PinSource5
#define DInPS GPIO_PinSource5

#define IOGPIO GPIOA

void InitSetup(void);
int8_t SetIntensity(uint8_t);
void SWd(uint32_t);
void PrintBuf(void);

void SetBuf(uint8_t);
void ClrBuf(void);
void WriteBuf(uint8_t);

void WritePix(uint8_t, uint8_t, uint8_t);
void SetPix(uint8_t, uint8_t);
void ClrPix(uint8_t, uint8_t);

void WriteCol(uint8_t, uint8_t);
void SetCol(uint8_t);
void ClrCol(uint8_t);

void WriteRow(uint8_t, uint8_t);
void SetRow(uint8_t);
void ClrRow(uint8_t);

void Delay(uint32_t);

#define XPix 8
#define YPix 8
#define GBufS (XPix*YPix)/8

extern uint8_t GBuf[GBufS];

#endif
