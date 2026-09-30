/*
 * seven_segment.h
 *
 *  Created on: Sep 30, 2026
 *      Author: princess
 */

#ifndef INC_SEVEN_SEGMENT_H_
#define INC_SEVEN_SEGMENT_H_

#include "main.h"

typedef struct {
	//here I am using the HAL defined struct
	GPIO_TypeDef *port;
	uint16_t pin;
} seven_seg_pin;

typedef enum {
	COMMON_CATHODE,
	COMMON_ANNODE
} disp_type;


void seven_segment_set_digit(const seven_seg_pin *display, uint8_t digit, disp_type type);

#endif /* INC_SEVEN_SEGMENT_H_ */
