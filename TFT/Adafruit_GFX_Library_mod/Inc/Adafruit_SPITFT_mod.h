/*
 * Adafruit_SPITFT_mod.h
 *
 *  Created on: Dec 13, 2024
 *      Author: Jxentech
 */

#ifndef ADAFRUIT_SPITFT_MOD_H_
#define ADAFRUIT_SPITFT_MOD_H_


/*!
 * @file Adafruit_SPITFT.h
 *
 * Part of Adafruit's GFX graphics library. Originally this class was
 * written to handle a range of color TFT displays connected via SPI,
 * but over time this library and some display-specific subclasses have
 * mutated to include some color OLEDs as well as parallel-interfaced
 * displays. The name's been kept for the sake of older code.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor "ladyada" Fried for Adafruit Industries,
 * with contributions from the open source community.
 *
 * BSD license, all text here must be included in any redistribution.
 */


#include "Adafruit_GFX_mod.h"
#include "stm32f1xx_hal.h"

// Compatibility

#define _CS_GPIO 		GPIOA
#define _CS 			GPIO_PIN_3
#define _DC_GPIO 		GPIOA
#define _DC				GPIO_PIN_6

// Ends here


extern SPI_HandleTypeDef hspi1;

// CLASS DEFINITION --------------------------------------------------------

/*!
  @brief  Adafruit_SPITFT is an intermediary class between Adafruit_GFX
          and various hardware-specific subclasses for different displays.
          It handles certain operations that are common to a range of
          displays (address window, area fills, etc.). Originally these were
          all color TFT displays interfaced via SPI, but it's since expanded
          to include color OLEDs and parallel-interfaced TFTs. THE NAME HAS
          BEEN KEPT TO AVOID BREAKING A LOT OF SUBCLASSES AND EXAMPLE CODE.
          Many of the class member functions similarly live on with names
          that don't necessarily accurately describe what they're doing,
          again to avoid breaking a lot of other code. If in doubt, read
          the comments.
*/
class Adafruit_SPITFT : public Adafruit_GFX {

public:
  // CONSTRUCTORS --------------------------------------------------------

  // Software SPI constructor: expects width & height (at default rotation
  // setting 0), 4 signal pins (cs, dc, mosi, sclk), 2 optional pins
  // (reset, miso). cs argument is required but can be -1 if unused --
  // rather than moving it to the optional arguments, it was done this way
  // to avoid breaking existing code (-1 option was a later addition).
  Adafruit_SPITFT(uint16_t w, uint16_t h, int8_t cs = _CS, int8_t dc = _DC);

  // DESTRUCTOR ----------------------------------------------------------

  ~Adafruit_SPITFT(){};

  // CLASS MEMBER FUNCTIONS ----------------------------------------------


  /*!
      @brief  Set up the specific display hardware's "address window"
              for subsequent pixel-pushing operations.
      @param  x  Leftmost pixel of area to be drawn (MUST be within
                 display bounds at current rotation setting).
      @param  y  Topmost pixel of area to be drawn (MUST be within
                 display bounds at current rotation setting).
      @param  w  Width of area to be drawn, in pixels (MUST be >0 and,
                 added to x, within display bounds at current rotation).
      @param  h  Height of area to be drawn, in pixels (MUST be >0 and,
                 added to x, within display bounds at current rotation).
  */
  virtual void setAddrWindow(uint16_t x, uint16_t y, uint16_t w,
                             uint16_t h) = 0;

  // Remaining functions do not need to be declared in subclasses
  // unless they wish to provide hardware-specific optimizations.
  // Brief comments here...documented more thoroughly in .cpp file.

  // Subclass' begin() function invokes this to initialize hardware.
  // freq=0 to use default SPI speed. spiMode must be one of the SPI_MODEn
  // values defined in SPI.h, which are NOT the same as 0 for SPI_MODE0,
  // 1 for SPI_MODE1, etc...use ONLY the SPI_MODEn defines! Only!
  // Name is outdated (interface may be parallel) but for compatibility:
  void startWrite(void);
  // Chip deselect and/or hardware SPI transaction end as needed:
  void endWrite(void);
  void sendCommand(uint8_t commandByte, uint8_t *dataBytes,
                   uint8_t numDataBytes);
  void sendCommand(uint8_t commandByte, const uint8_t *dataBytes = NULL,
                   uint8_t numDataBytes = 0);
  void sendCommand16(uint16_t commandWord, const uint8_t *dataBytes = NULL,
                     uint8_t numDataBytes = 0);
  uint8_t readcommand8(uint8_t commandByte, uint8_t index = 0);
  uint16_t readcommand16(uint16_t addr);

  // These functions require a chip-select and/or SPI transaction
  // around them. Higher-level graphics primitives might start a
  // single transaction and then make multiple calls to these functions
  // (e.g. circle or text rendering might make repeated lines or rects)
  // before ending the transaction. It's more efficient than starting a
  // transaction every time.
  void writePixel(int16_t x, int16_t y, uint16_t color);
  void writePixels(uint16_t *colors, uint32_t len, bool block = true,
                   bool bigEndian = false);
  void writeColor(uint16_t color, uint32_t len);
  void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                     uint16_t color);
  void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  // This is a new function, similar to writeFillRect() except that
  // all arguments MUST be onscreen, sorted and clipped. If higher-level
  // primitives can handle their own sorting/clipping, it avoids repeating
  // such operations in the low-level code, making it potentially faster.
  // CALLING THIS WITH UNCLIPPED OR NEGATIVE VALUES COULD BE DISASTROUS.
  inline void writeFillRectPreclipped(int16_t x, int16_t y, int16_t w,
                                      int16_t h, uint16_t color);
  // Another new function, companion to the new non-blocking
  // writePixels() variant.
  void swapBytes(uint16_t *src, uint32_t len, uint16_t *dest = NULL);

  // These functions are similar to the 'write' functions above, but with
  // a chip-select and/or SPI transaction built-in. They're typically used
  // solo -- that is, as graphics primitives in themselves, not invoked by
  // higher-level primitives (which should use the functions above).
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  // A single-pixel push encapsulated in a transaction. I don't think
  // this is used anymore (BMP demos might've used it?) but is provided
  // for backward compatibility, consider it deprecated:
  void pushColor(uint16_t color);

  using Adafruit_GFX::drawRGBBitmap; // Check base class first
  void drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w,
                     int16_t h);

  void invertDisplay(bool i);
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

  // Despite parallel additions, function names kept for compatibility:
  void spiWrite(uint8_t b);          // Write single byte as DATA
  void writeCommand(uint8_t cmd);    // Write single byte as COMMAND
  uint8_t spiRead(void);             // Read single byte of data
  void write16(uint16_t w);          // Write 16-bit value as DATA
  void writeCommand16(uint16_t cmd); // Write 16-bit value as COMMAND
  uint16_t read16(void);             // Read single 16-bit value

  // Most of these low-level functions were formerly macros in
  // Adafruit_SPITFT_Macros.h. Some have been made into inline functions
  // to avoid macro mishaps. Despite the addition of code for a parallel
  // display interface, the names have been kept for backward
  // compatibility (some subclasses may be invoking these):
  void SPI_WRITE16(uint16_t w); // Not inline
  void SPI_WRITE32(uint32_t l); // Not inline
  // Old code had both a spiWrite16() function and SPI_WRITE16 macro
  // in addition to the SPI_WRITE32 macro. The latter two have been
  // made into functions here, and spiWrite16() removed (use SPI_WRITE16()
  // instead). It looks like most subclasses had gotten comfortable with
  // SPI_WRITE16 and SPI_WRITE32 anyway so those names were kept rather
  // than the less-obnoxious camelcase variants, oh well.

  // Placing these functions entirely in the class definition inlines
  // them implicitly them while allowing their use in other code:

  /*!
      @brief  Set the chip-select line HIGH. Does NOT check whether CS pin
              is set (>=0), that should be handled in calling function.
              Despite function name, this is used even if the display
              connection is parallel.
  */
  void SPI_CS_HIGH(void) {
	HAL_GPIO_WritePin(_CS_GPIO,_cs,GPIO_PIN_SET);
  }

  /*!
      @brief  Set the chip-select line LOW. Does NOT check whether CS pin
              is set (>=0), that should be handled in calling function.
              Despite function name, this is used even if the display
              connection is parallel.
  */
  void SPI_CS_LOW(void) {
	HAL_GPIO_WritePin(_CS_GPIO,_cs,GPIO_PIN_RESET);
  }

  /*!
      @brief  Set the data/command line HIGH (data mode).
  */
  void SPI_DC_HIGH(void) {
	HAL_GPIO_WritePin(_DC_GPIO,_dc,GPIO_PIN_SET);
  }

  /*!
      @brief  Set the data/command line LOW (command mode).
  */
  void SPI_DC_LOW(void) {
	HAL_GPIO_WritePin(_DC_GPIO,_dc,GPIO_PIN_RESET);
  }

protected:

  int8_t _cs;              ///< Chip select pin # (or -1)
  int8_t _dc;              ///< Data/command pin #

  int16_t _xstart = 0;          ///< Internal framebuffer X offset
  int16_t _ystart = 0;          ///< Internal framebuffer Y offset
  uint8_t invertOnCommand = 0;  ///< Command to enable invert mode
  uint8_t invertOffCommand = 0; ///< Command to disable invert mode

};





#endif /* ADAFRUIT_SPITFT_MOD_H_ */
