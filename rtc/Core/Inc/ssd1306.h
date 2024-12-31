/*
 * SSD1306.h
 *
 *  Created on: Dec 16, 2023
 *      Author: HP
 */

#ifndef INC_SSD1306_H
#define INC_SSD1306_H

//includes
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "flip_font.h"


//we can invert the color without sending a command
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SSD1306_COLOR_BLUE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;

//Macros
#define PRINT_TOP_HALF 		0x01
#define PRINT_BOTTOM_HALF  	0x02
#define PRINT_BOTH			0x03

#define SCALED_BY_ONE 		0x01
#define SCALED_BY_TWO 		0x02
#define SCALED_BY_THREE		0x03

//Function prototypes
void ssd1306_I2C_Write(uint8_t addr, uint8_t control_byte, uint8_t data);
uint8_t SSD1306_Init(void);
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);
char SSD1306_customPutc(char ch, FontDef_t* Font,uint8_t size, uint8_t format, SSD1306_COLOR_t color);
char SSD1306_customPuts(char* str, FontDef_t* Font, uint8_t scale, uint8_t format, SSD1306_COLOR_t color);
char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);
void SSD1306_Fill(SSD1306_COLOR_t color);
void SSD1306_GotoXY(uint16_t x, uint16_t y);
void SSD1306_UpdateScreen(void);
void SSD1306_Clear (void);
void SSD1306_ON(void);
void SSD1306_OFF(void);
void SSD1306_ToggleInvert(void);
void SSD1306_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, SSD1306_COLOR_t c);
void SSD1306_DrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, SSD1306_COLOR_t c);
void SSD1306_DrawFilledRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, SSD1306_COLOR_t c);
void SSD1306_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, SSD1306_COLOR_t color);

#endif /* INC_SSD1306_H_ */
