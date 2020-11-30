/*
 * display.c
 *
 *  Created on: Nov 23, 2020
 *      Author: Stancoj
 */

#include "display.h"

extern uint64_t disp_time;
uint64_t disp_time_saved = 0;
uint8_t buffer[DISPLAY_SIZE];

void updateDisplay(void);
void setDigit(uint8_t pos);

/*Reset (turn-off) all the segments of display*/
void resetSegments(void)
{
	LL_GPIO_SetOutputPin(SEGMENT_A_PORT, 1 << SEGMENT_A_PIN);
	LL_GPIO_SetOutputPin(SEGMENT_B_PORT, 1 << SEGMENT_B_PIN);
	LL_GPIO_SetOutputPin(SEGMENT_C_PORT, 1 << SEGMENT_C_PIN);
	LL_GPIO_SetOutputPin(SEGMENT_D_PORT, 1 << SEGMENT_D_PIN);
	LL_GPIO_SetOutputPin(SEGMENT_E_PORT, 1 << SEGMENT_E_PIN);
	LL_GPIO_SetOutputPin(SEGMENT_F_PORT, 1 << SEGMENT_F_PIN);
	LL_GPIO_SetOutputPin(SEGMENT_G_PORT, 1 << SEGMENT_G_PIN);
	LL_GPIO_SetOutputPin(SEGMENT_DP_PORT, 1 << SEGMENT_DP_PIN);
}

/*Set (turn-on) all the segments of display*/
void setSegments(void)
{
	LL_GPIO_ResetOutputPin(SEGMENT_A_PORT, 1 << SEGMENT_A_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_B_PORT, 1 << SEGMENT_B_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_C_PORT, 1 << SEGMENT_C_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_D_PORT, 1 << SEGMENT_D_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_E_PORT, 1 << SEGMENT_E_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_F_PORT, 1 << SEGMENT_F_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_G_PORT, 1 << SEGMENT_G_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_DP_PORT, 1 << SEGMENT_DP_PIN);
}

/* Reset (turn-off) all digits*/
void resetDigits(void)
{
	DIGIT_1_OFF;
	DIGIT_2_OFF;
	DIGIT_3_OFF;
	DIGIT_4_OFF;
	DIGIT_TIME_OFF;
}

/* Reset (turn-on) all digits*/
void setDigits(void)
{
	DIGIT_1_ON;
	DIGIT_2_ON;
	DIGIT_3_ON;
	DIGIT_4_ON;
	DIGIT_TIME_ON;
}

void setDecimalPoint(void)
{
	LL_GPIO_ResetOutputPin(SEGMENT_DP_PORT, 1 << SEGMENT_DP_PIN);
}

void fill_buffer(uint8_t *buffer, uint8_t buffer_size, uint8_t *str, uint8_t len, uint8_t idx) {
	for (uint8_t n = 0; n < buffer_size; n++) {
		buffer[n] = str[(n + idx) % len];
	}
}

void displayMessage(uint8_t *msg, uint8_t len) {
	static uint8_t idx = 0;

  	fill_buffer(buffer, DISPLAY_SIZE, msg, len, idx);

  	idx++;
  	if (idx == len) idx = 0;
}

void displayCharacter(uint8_t mask) {
	LL_GPIO_ResetOutputPin(SEGMENT_A_PORT, ((mask >> 6) & 1) << SEGMENT_A_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_B_PORT, ((mask >> 5) & 1) << SEGMENT_B_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_C_PORT, ((mask >> 4) & 1) << SEGMENT_C_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_D_PORT, ((mask >> 3) & 1) << SEGMENT_D_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_E_PORT, ((mask >> 2) & 1) << SEGMENT_E_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_F_PORT, ((mask >> 1) & 1) << SEGMENT_F_PIN);
	LL_GPIO_ResetOutputPin(SEGMENT_G_PORT, ((mask >> 0) & 1) << SEGMENT_G_PIN);
}

/*
 * Turns required digit ON
 */
void setDigit(uint8_t pos)
{
	switch(pos)
	{
		case 0:
			DIGIT_1_ON;
			break;
		case 1:
			DIGIT_2_ON;
			break;
		case 2:
			DIGIT_3_ON;
			break;
		case 3:
			DIGIT_4_ON;
			break;
	}
}


void resetDigit(uint8_t pos)
{
	switch(pos)
	{
		case 0:
			DIGIT_1_OFF;
			break;
		case 1:
			DIGIT_2_OFF;
			break;
		case 2:
			DIGIT_3_OFF;
			break;
		case 3:
			DIGIT_4_OFF;
			break;
	}
}

/**
 * Display data in dDisplayData.
 * Sets every digit to display its value and decimal point.
 */
void updateDisplay(void) {
	for(uint8_t i = 0; i < DISPLAY_SIZE; i++) {
		setDigit(i);
		displayCharacter(ascii_to_mask(buffer[i]));

		disp_time_saved = disp_time;
		while ((disp_time_saved + 2) > disp_time) {};

		resetDigit(i);
		resetSegments();
	}
}

//Update displayed data and keep display ON
void TIM3_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIM3))
	{
		updateDisplay();
	}

	LL_TIM_ClearFlag_UPDATE(TIM3);
}

