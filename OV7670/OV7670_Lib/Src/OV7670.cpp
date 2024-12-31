/*
 * OV7670.cpp
 *
 *  Created on: Nov 16, 2024
 *      Author: Jxentech
 */

#include "OV7670.h"


OV7670::OV7670(uint8_t addr, I2C_HandleTypeDef &rhi2c)
    : i2c_address(addr & 0x7f), hi2c(rhi2c),
      buffer(NULL), buffer_size(0) {}

OV7670::~OV7670() {
  if (buffer) {
    free(buffer);
  }
}

// CAMERA INIT AND CONFIG FUNCTIONS ----------------------------------------

OV7670_status OV7670::begin(OV7670_colorspace colorspace,
                                     OV7670_size size, float fps,
                                     uint32_t bufsiz) {

  _width = 640 >> (int)size;  // 640, 320, 160, 80, 40
  _height = 480 >> (int)size; // 480, 240, 120, 60, 30
  space = colorspace;

  // Allocate buffer for camera row Line
  buffer_size = bufsiz ? bufsiz : _width * sizeof(uint16_t);
  buffer = (uint16_t *)malloc(buffer_size);
  if (buffer == NULL) {
    buffer_size = 0;
    return OV7670_STATUS_ERR_MALLOC;
  }

  return OV7670_begin(colorspace, size, fps); // Device-specific setup
}

uint8_t OV7670::readRegister(uint8_t reg) {
	uint8_t data;
	HAL_I2C_Master_Transmit(&hi2c, OV7670_ADDR, (uint8_t *)&reg, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c, OV7670_ADDR, &data, 1, HAL_MAX_DELAY);
	return data;
}

void OV7670::writeRegister(uint8_t reg, uint8_t value) {
	HAL_I2C_Master_Transmit(&hi2c, OV7670_ADDR, &reg, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Transmit(&hi2c, OV7670_ADDR, &value, 1, HAL_MAX_DELAY);
}

OV7670_status OV7670::setSize(OV7670_size size, OV7670_realloc allo) {
  uint16_t new_width = 640 >> (int)size;
  uint16_t new_height = 480 >> (int)size;
  uint32_t new_buffer_size = new_width * sizeof(uint16_t);
  bool ra = false;

  switch (allo) {
  case OV7670_REALLOC_NONE:
    if (new_buffer_size > buffer_size) { // New size won't fit
      // Don't realloc. Keep current camera settings, return error.
      return OV7670_STATUS_ERR_MALLOC;
    }
    break;
  case OV7670_REALLOC_CHANGE:
    ra = (new_buffer_size != buffer_size); // Realloc on size change
    break;
  case OV7670_REALLOC_LARGER:
    ra = (new_buffer_size > buffer_size); // Realloc on size increase
    break;
  }

  if (ra) { // Reallocate?
    uint16_t *new_buffer = (uint16_t *)realloc(buffer, new_buffer_size);
    if (new_buffer == NULL) { // FAIL
      _width = _height = buffer_size = 0;
      buffer = NULL;
      // Calling code had better poll width(), height() or getBuffer() in
      // this case so it knows the camera buffer is gone, doesn't attempt
      // to read camera data into unknown RAM.
      return OV7670_STATUS_ERR_MALLOC;
    }
    buffer_size = new_buffer_size;
  }

  _width = new_width;
  _height = new_height;

  OV7670_set_size(size);

  return OV7670_STATUS_OK;
}

void OV7670::Y2RGB565(void) {
  OV7670_Y2RGB565(buffer, _width * _height);
}


void OV7670::captureLine(void) {
  OV7670_captureLine(buffer, _width);
}

// C-ACCESSIBLE FUNCTIONS --------------------------------------------------

// These functions are declared in an extern "C" block in Adafruit_OV7670.h
// so that arch/*.c code can access them here. The Doxygen comments in the
// .h explain their use in more detail.

//void OV7670_print(char *str) { Serial.print(str); }

uint8_t OV7670_read_register(uint8_t reg) {
	uint8_t data;
	HAL_I2C_Master_Transmit(&HI2C, OV7670_ADDR, &reg, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&HI2C, OV7670_ADDR, &data, 1, HAL_MAX_DELAY);
	return data;
}

void OV7670_write_register(uint8_t reg, uint8_t value) {
	HAL_I2C_Master_Transmit(&HI2C, OV7670_ADDR, &reg, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Transmit(&HI2C, OV7670_ADDR, &value, 1, HAL_MAX_DELAY);
}
