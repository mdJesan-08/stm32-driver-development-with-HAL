/*
 * seven_segment.c
 *
 *  Created on: Sep 28, 2026
 *      Author: princess
 */


#include "seven_segment.h"

static const uint8_t display[10][7] =
{
//   a  b  c  d  e  f  g
    {1, 1, 1, 1, 1, 1, 0},  // 0
    {0, 1, 1, 0, 0, 0, 0},  // 1
    {1, 1, 0, 1, 1, 0, 1},  // 2
    {1, 1, 1, 1, 0, 0, 1},  // 3
    {0, 1, 1, 0, 0, 1, 1},  // 4
    {1, 0, 1, 1, 0, 1, 1},  // 5
    {1, 0, 1, 1, 1, 1, 1},  // 6
    {1, 1, 1, 0, 0, 0, 0},  // 7
    {1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 0, 1, 1}   // 9
};

void seven_segment_set_digit(const seven_seg_pin *pinSetUp, uint8_t digit, disp_type type)
{
//	uint8_t target = display[]
//	const uint8_t *row = display[digit];
//	common annode vcc is shared
	GPIO_PinState trigger_state = (type == COMMON_ANNODE ? GPIO_PIN_RESET : GPIO_PIN_SET );// a paper pointing at one row of the table

	for (uint8_t i = 0; i < 7; i++)
	{
		if( trigger_state) HAL_GPIO_WritePin(pinSetUp[i].port, pinSetUp[i].pin,display[digit][i] ? GPIO_PIN_RESET : GPIO_PIN_SET);

		else HAL_GPIO_WritePin(pinSetUp[i].port, pinSetUp[i].pin, display[digit][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);


	}
}
