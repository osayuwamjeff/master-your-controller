/*
 * Adafruit_ST7789_mod.h
 *
 *  Created on: Dec 25, 2024
 *      Author: User
 */

#ifndef ADAFRUIT_ST7789_MOD_H_
#define ADAFRUIT_ST7789_MOD_H_


#include "Adafruit_ST77xx_mod.h"

/// Subclass of ST77XX type display for ST7789 TFT Driver
class Adafruit_ST7789 : public Adafruit_ST77xx {
public:
  explicit Adafruit_ST7789(int8_t cs = _CS, int8_t dc = _DC);

  void setRotation(uint8_t m);
  void init(uint16_t width, uint16_t height);

protected:
  uint8_t _colstart2 = 0, ///< Offset from the right
      _rowstart2 = 0;     ///< Offset from the bottom

private:
  uint16_t windowWidth;
  uint16_t windowHeight;
};



#endif /* ADAFRUIT_ST7789_MOD_H_ */
