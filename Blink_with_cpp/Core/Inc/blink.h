/*
 * blink.h
 *
 *  Created on: Oct 25, 2024
 *      Author: User
 */

#ifndef INC_BLINK_H_
#define INC_BLINK_H_

#include <cstdint>
#include "stm32f1xx_hal.h"


class blink{
public:
	void on(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
	void off(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
};



#endif /* INC_BLINK_H_ */
