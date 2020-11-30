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

const uint8_t characters[] = {
//    ABCDEFG  Segments  7-segment map:
	0b1111110, // 0   "0"          AAA
	0b0110000, // 1   "1"         F   B
	0b1101101, // 2   "2"         F   B
	0b1111001, // 3   "3"          GGG
	0b0110011, // 4   "4"         E   C
	0b1011011, // 5   "5"         E   C
	0b1011111, // 6   "6"          DDD
	0b1110000, // 7   "7"
	0b1111111, // 8   "8"
	0b1111011, // 9   "9"

	0b1110111, // 65  'A'
	0b1111111, // 66  'B'
	0b1001110, // 67  'C'
	0b1111100, // 68  'D'
	0b1001111, // 69  'E'
	0b1000111, // 70  'F'
	0b1011110, // 71  'G'
	0b0110111, // 72  'H'
	0b0110000, // 73  'I'
	0b0111100, // 74  'J'
	0b1010111, // 75  'K'
	0b0001110, // 76  'L'
	0b1101010, // 77  'M'
	0b1110110, // 78  'N'
	0b1111110, // 79  'O'
	0b1100111, // 80  'P'
	0b1101011, // 81  'Q'
	0b1101111, // 82  'R'
	0b1011011, // 83  'S'
	0b1000110, // 84  'T'
	0b0111110, // 85  'U'
	0b0111010, // 86  'V'
	0b1011100, // 87  'W'
	0b1001001, // 88  'X'
	0b0101011, // 89  'Y'
	0b1101101, // 90  'Z'

	0b1111101, // 65  'a'
	0b0011111, // 66  'b'
	0b0001101, // 67  'c'
	0b0111101, // 68  'd'
	0b0001100, // 69  'e'
	0b0000111, // 70  'f'
	0b1001101, // 71  'g'
	0b0010111, // 72  'h'
	0b1001100, // 73  'i'
	0b1011000, // 74  'j'
	0b1001011, // 75  'k'
	0b0000110, // 76  'l'
	0b1010101, // 77  'm'
	0b0010101, // 78  'n'
	0b0011101, // 79  'o'
	0b1100111, // 80  'p'
	0b1110011, // 81  'q'
	0b0000101, // 82  'r'
	0b0011000, // 83  's'
	0b0001111, // 84  't'
	0b0011100, // 85  'u'
	0b0011000, // 86  'v'
	0b0101010, // 87  'w'
	0b0001001, // 88  'x'
	0b0111011, // 89  'y'
	0b0001010, // 90  'z'

	0b0001000, // 95  '_'
};

const uint8_t ascii_to_mask(char character) {
	if ('0' <= character && character <= '9') return characters[character - '0'];
	else if ('A' <= character && character <= 'Z') return characters[character - 'A' + 10];
	else if ('a' <= character && character <= 'z') return characters[character - 'a' + 36];
	else if ('_' == character) return characters[character - '_' + 62];

	return 0;
}


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

