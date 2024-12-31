/*
 * SSD1306.c
 *
 *  Created on: Dec 16, 2023
 *      Author: Osayuwamen Jeffery
 */

//LSB MSB Left to Right

#include "ssd1306.h"
#define SSD1306_COMMAND(CMD)	ssd1306_I2C_Write(SSD1306_ADDR,COMMAND_BYTE,CMD)
#define SSD1306_DATA(DATA)		ssd1306_I2C_Write(SSD1306_ADDR,DATA_BYTE,DATA)
//MACROS
//OLED Slave address
#define SSD1306_ADDR 								0x78

#define SSD1306_HEIGHT 								64  //No. of rows
#define SSD1306_WIDTH 								128 //No. of columns

//Control bytes
#define DATA_STREAM 								0x40
#define DATA_BYTE 									0xC0
#define COMMAND_BYTE 								0x80
#define COMMAND_STREAM 								0x00

//
#define SET_MUX_RATIO								0xA8
#define SET_DISPLAY_OFFSET							0xD3
#define SEGMENT_NO_REMAP							0xA0
#define SEGMENT_REMAP								0xA1
#define COM_OUTPUT_SCAN_DIR_NORMAL 					0xC0
#define COM_OUTPUT_SCAN_DIR_REMAP 					0xC8
#define SET_CONTRAST_CONTROL						0x81
#define DISABLE_ENTIRE_DISPLAY_ON					0xA4 //resume from GDDRAM
#define ENABLE_ENTIRE_DISPLAY_ON					0xA5 //Ignore RAM
#define SET_CHARGE_PUMP								0x8D
#define ENABLE_CHARGE_PUMP							0x14
#define DISABLE_CHARGE_PUMP							0x10
#define DISPLAY_ON									0xAF
#define DISPLAY_OFF									0xAE
#define SET_DISPLAY_CLOCK_DIVIDE_RATIO_AND_OSC_FREQ 				0xD5
#define SET_COM_PINS_HARDWARE_CONFIG								0xDA
#define COM_PINS_HW_CONFIG_SEQ_DISABLE_REMAP 						0x02
#define COM_PINS_HW_CONFIG_SEQ_ENABLE_REMAP 						0x22
#define COM_PINS_HW_CONFIG_ALTCOM_DISABLE_REMAP 					0x12
#define COM_PINS_HW_CONFIG_ALTCOM_ENABLE_REMAP 						0x32


typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

static SSD1306_t SSD1306;

extern I2C_HandleTypeDef hi2c1;
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

void ssd1306_I2C_Write(uint8_t addr, uint8_t control_byte, uint8_t data){
	uint8_t data_reg[2];
	data_reg[0] = control_byte;
	data_reg[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1,SSD1306_ADDR,data_reg,2,10);
}

uint8_t SSD1306_Init(void){
	//Set Mux Ratio
	SSD1306_COMMAND(SET_MUX_RATIO);
	SSD1306_COMMAND(0x3F);					//(63d = 64MUX) 0d - 14d are invalid entry

	//Set Display Offset
	SSD1306_COMMAND(SET_DISPLAY_OFFSET);
	SSD1306_COMMAND(0x00);					//0d - 63d

	//Set Segment Re-map
	SSD1306_COMMAND(SEGMENT_REMAP);

	//Set COM Output Scan Direction
	SSD1306_COMMAND(COM_OUTPUT_SCAN_DIR_REMAP);

	//Set COM Pins Hardware Configuration
	SSD1306_COMMAND(SET_COM_PINS_HARDWARE_CONFIG);
	SSD1306_COMMAND(COM_PINS_HW_CONFIG_ALTCOM_DISABLE_REMAP );

	//Set Contract Control
	SSD1306_COMMAND(SET_CONTRAST_CONTROL);
	SSD1306_COMMAND(0x7F);

	//Disable Entire Display On
	SSD1306_COMMAND(DISABLE_ENTIRE_DISPLAY_ON);

	//Set Osc Frequency
	SSD1306_COMMAND(SET_DISPLAY_CLOCK_DIVIDE_RATIO_AND_OSC_FREQ);
	SSD1306_COMMAND(0xF0);

	//Enable Charge Regulator
	SSD1306_COMMAND(SET_CHARGE_PUMP);
	SSD1306_COMMAND(ENABLE_CHARGE_PUMP);

	//Set Memory Address Mode
	SSD1306_COMMAND(0x20);
	SSD1306_COMMAND(0x00);				//horizontal

	//Display On
	SSD1306_COMMAND(DISPLAY_ON);

	/* Clear screen */
	SSD1306_Fill(SSD1306_COLOR_BLACK);

	/* Update screen */
	SSD1306_UpdateScreen();

	/* Set default values */
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;

	SSD1306.Initialized = 1;

	/* Return OK */
	return 1;
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Error */
		return;
	}

	/* Check if pixels are inverted */
	if (SSD1306.Inverted) {
		color = (SSD1306_COLOR_t)!color;
	}

	/* Set color */
	if (color == SSD1306_COLOR_BLUE) {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} else {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

/*
  Handle control codes or write the char to the screen.
  Supported control codes are:

    \n		10		Goto first position of the next line. Line is marked for redraw.
    \r		13		Goto first position in the same line. Line is marked for redraw.
    \t		9		Jump to the next tab position
    any other char	Write char to screen. Line redraw mark depends on
				is_redraw_line_for_each_char flag.
*/
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;

	/* Check available space in LCD */
	if (
		SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}

	/* Go through font */
	switch(ch){
	case '\n':
		SSD1306.CurrentX = 0;
		SSD1306.CurrentY += Font->FontHeight;
		break;
	case '\r':
		SSD1306.CurrentX = 0;
		break;
	case '\t':
		SSD1306.CurrentX += Font->FontWidth * 2;
		break;
	default:
		for (i = 0; i < Font->FontHeight; i++) {
			b = Font->data[(ch - 32) * Font->FontHeight + i];
			for (j = 0; j < Font->FontWidth; j++) {
				if ((b << j) & 0x8000) {
					SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)color);
				} else {
					SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
				}
			}
		}
	}


	/* Increase pointer */
	SSD1306.CurrentX += Font->FontWidth;
	if((SSD1306.CurrentX + Font->FontWidth) >= SSD1306_WIDTH){
		SSD1306.CurrentX = 0;
		SSD1306.CurrentY += Font->FontHeight;
	}

	/* Return character written */
	return ch;
}

char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (SSD1306_Putc(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}

		/* Increase string pointer */
		str++;
	}

	/* Everything OK, zero should be returned */
	return *str;
}

void SSD1306_Fill(SSD1306_COLOR_t color) {
	/* Set memory */
	memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void SSD1306_GotoXY(uint16_t x, uint16_t y) {
	/* Set write pointers */
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

void SSD1306_UpdateScreen(void) {
	//Send data to OLED
	for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++){

		uint8_t temp[SSD1306_WIDTH + 1];
		temp[0] = DATA_STREAM;

		for(uint8_t j = 0; j < SSD1306_WIDTH; j++){
			temp[j + 1] = SSD1306_Buffer[j + i*SSD1306_WIDTH];
		}

		HAL_I2C_Master_Transmit(&hi2c1,SSD1306_ADDR,temp,SSD1306_WIDTH + 1,1000);
	}

}

void SSD1306_Clear (void)
{
	SSD1306_Fill (SSD1306_COLOR_BLACK);
    SSD1306_UpdateScreen();
}

void SSD1306_ON(void) {
	SSD1306_COMMAND(0x8D);
	SSD1306_COMMAND(0x14);
	SSD1306_COMMAND(0xAF);
}
void SSD1306_OFF(void) {
	SSD1306_COMMAND(0x8D);
	SSD1306_COMMAND(0x10);
	SSD1306_COMMAND(0xAE);
}

void SSD1306_ToggleInvert(void) {
	uint16_t i;

	/* Toggle invert */
	SSD1306.Inverted = !SSD1306.Inverted;

	/* Do memory toggle */
	for (i = 0; i < sizeof(SSD1306_Buffer); i++) {
		SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
	}
}

void SSD1306_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, SSD1306_COLOR_t c){
	/* Check input parameters */
	if((x0 < 0 || x0 >= SSD1306_WIDTH) || (x1 < 0 || x1 >= SSD1306_WIDTH) || (y0 < 0 || y0 >= SSD1306_HEIGHT) || (y1 < 0 || y1 >= SSD1306_HEIGHT))
		return;

	uint8_t dx, dy, sx, sy, err, e2, i, tmp;
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			SSD1306_DrawPixel(x0, i, c);
		}

		/* Return from function */
		return;
	}

	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			SSD1306_DrawPixel(i, y0, c);
		}

		/* Return from function */
		return;
	}

	while (1) {
		SSD1306_DrawPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

void SSD1306_DrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, SSD1306_COLOR_t c){
	/* Check input parameters */
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}

	/* Draw 4 lines */
	SSD1306_DrawLine(x, y, x + w, y, c);         /* Top line */
	SSD1306_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	SSD1306_DrawLine(x, y, x, y + h, c);         /* Left line */
	SSD1306_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

void SSD1306_DrawFilledRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, SSD1306_COLOR_t c) {
	uint8_t i;

	/* Check input parameters */
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}

	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		SSD1306_DrawLine(x, y + i, x + w, y + i, c);
	}
}

void SSD1306_DrawTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, SSD1306_COLOR_t color) {
	/* Draw lines */
	SSD1306_DrawLine(x1, y1, x2, y2, color);
	SSD1306_DrawLine(x2, y2, x3, y3, color);
	SSD1306_DrawLine(x3, y3, x1, y1, color);
}

void SSD1306_DrawFilledTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, SSD1306_COLOR_t color) {
	uint8_t dx = 0, dy = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
	curpixel = 0;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (dx >= dy){
		xinc1 = 0;
		yinc2 = 0;
		den = dx;
		num = dx / 2;
		numadd = dy;
		numpixels = dx;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = dy;
		num = dy / 2;
		numadd = dx;
		numpixels = dy;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		SSD1306_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void SSD1306_DrawCircle(uint8_t x0, uint8_t y0, uint8_t r, SSD1306_COLOR_t c){
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;

    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawPixel(x0 + x, y0 + y, c);
        SSD1306_DrawPixel(x0 - x, y0 + y, c);
        SSD1306_DrawPixel(x0 + x, y0 - y, c);
        SSD1306_DrawPixel(x0 - x, y0 - y, c);

        SSD1306_DrawPixel(x0 + y, y0 + x, c);
        SSD1306_DrawPixel(x0 - y, y0 + x, c);
        SSD1306_DrawPixel(x0 + y, y0 - x, c);
        SSD1306_DrawPixel(x0 - y, y0 - x, c);
    }
}

void SSD1306_DrawFilledCircle(uint8_t x0, uint8_t y0, uint8_t r, SSD1306_COLOR_t c){
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;

    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);
    SSD1306_DrawLine(x0 - r, y0, x0 + r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
        SSD1306_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

        SSD1306_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
        SSD1306_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}


void SSD1306_DrawEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, SSD1306_COLOR_t c){
	  uint8_t x, y;
	  int32_t xchg, ychg;
	  int32_t err;
	  int32_t rxrx2;
	  int32_t ryry2;
	  int32_t stopx, stopy;

	  rxrx2 = rx;
	  rxrx2 *= rx;
	  rxrx2 *= 2;

	  ryry2 = ry;
	  ryry2 *= ry;
	  ryry2 *= 2;

	  x = rx;
	  y = 0;

	  xchg = 1;
	  xchg -= rx;
	  xchg -= rx;
	  xchg *= ry;
	  xchg *= ry;

	  ychg = rx;
	  ychg *= rx;

	  err = 0;

	  stopx = ryry2;
	  stopx *= rx;
	  stopy = 0;

	  while( stopx >= stopy )
	  {
		SSD1306_DrawPixel(x0 + x, y0 - y, c);
		SSD1306_DrawPixel(x0 - x, y0 - y, c);
		SSD1306_DrawPixel(x0 + x, y0 + y, c);
		SSD1306_DrawPixel(x0 - x, y0 + y, c);
		y++;
		stopy += rxrx2;
		err += ychg;
		ychg += rxrx2;
		if ( 2*err+xchg > 0 )
		{
		  x--;
		  stopx -= ryry2;
		  err += xchg;
		  xchg += ryry2;
		}
	  }

	  x = 0;
	  y = ry;

	  xchg = ry;
	  xchg *= ry;

	  ychg = 1;
	  ychg -= ry;
	  ychg -= ry;
	  ychg *= rx;
	  ychg *= rx;

	  err = 0;

	  stopx = 0;

	  stopy = rxrx2;
	  stopy *= ry;


	  while( stopx <= stopy )
	  {
		SSD1306_DrawPixel(x0 + x, y0 - y, c);
		SSD1306_DrawPixel(x0 - x, y0 - y, c);
		SSD1306_DrawPixel(x0 + x, y0 + y, c);
		SSD1306_DrawPixel(x0 - x, y0 + y, c);
		x++;
		stopx += ryry2;
		err += xchg;
		xchg += ryry2;
		if ( 2*err+ychg > 0 )
		{
		  y--;
		  stopy -= rxrx2;
		  err += ychg;
		  ychg += rxrx2;
		}
	  }
	}

void SSD1306_DrawFilledEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, SSD1306_COLOR_t c){
	  uint8_t x, y;
	  int32_t xchg, ychg;
	  int32_t err;
	  int32_t rxrx2;
	  int32_t ryry2;
	  int32_t stopx, stopy;

	  rxrx2 = rx;
	  rxrx2 *= rx;
	  rxrx2 *= 2;

	  ryry2 = ry;
	  ryry2 *= ry;
	  ryry2 *= 2;

	  x = rx;
	  y = 0;

	  xchg = 1;
	  xchg -= rx;
	  xchg -= rx;
	  xchg *= ry;
	  xchg *= ry;

	  ychg = rx;
	  ychg *= rx;

	  err = 0;

	  stopx = ryry2;
	  stopx *= rx;
	  stopy = 0;

	  while( stopx >= stopy )
	  {
		SSD1306_DrawLine(x0+x, y0-y, x0+x, y0 -y + y+1, c);
		SSD1306_DrawLine(x0-x, y0-y, x0-x, y0 -y + y+1, c);
		SSD1306_DrawLine(x0+x, y0, x0+x, y0 + y+1, c);
		SSD1306_DrawLine(x0-x, y0, x0-x, y0 + y+1, c);
	    y++;
	    stopy += rxrx2;
	    err += ychg;
	    ychg += rxrx2;
	    if ( 2*err+xchg > 0 )
	    {
	      x--;
	      stopx -= ryry2;
	      err += xchg;
	      xchg += ryry2;
	    }
	  }

	  x = 0;
	  y = ry;

	  xchg = ry;
	  xchg *= ry;

	  ychg = 1;
	  ychg -= ry;
	  ychg -= ry;
	  ychg *= rx;
	  ychg *= rx;

	  err = 0;

	  stopx = 0;

	  stopy = rxrx2;
	  stopy *= ry;


	  while( stopx <= stopy )
	  {
	    SSD1306_DrawLine(x0+x, y0-y, x0+x, y0 - y + y+1, c);
	    SSD1306_DrawLine(x0-x, y0-y, x0-x, y0 - y + y+1, c);
	    SSD1306_DrawLine(x0+x, y0, x0+x, y0 + y+1, c);
	    SSD1306_DrawLine(x0-x, y0, x0-x, y0 + y+1, c);
	    x++;
	    stopx += ryry2;
	    err += xchg;
	    xchg += ryry2;
	    if ( 2*err+ychg > 0 )
	    {
	      y--;
	      stopy -= rxrx2;
	      err += ychg;
	      ychg += rxrx2;
	    }
	  }
}

void SSD1306_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, SSD1306_COLOR_t color)
{

    uint8_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for(uint8_t j=0; j<h; j++, y++)
    {
        for(uint8_t i=0; i<w; i++)
        {
            if(i & 7)
            {
               byte <<= 1;
            }
            else
            {
               byte = bitmap[j * byteWidth + i / 8];
            }
            if(byte & 0x80) SSD1306_DrawPixel(x+i, y, color);
        }
    }
}
