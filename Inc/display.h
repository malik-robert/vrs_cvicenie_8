/*
 * display.c
 *
 *  Created on: Nov 23, 2020
 *      Author: Stancoj
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "main.h"
#include <stdio.h>
#include <string.h>

#define SEGMENT_A_PIN	(uint8_t) 1
#define SEGMENT_A_PORT	GPIOA

#define SEGMENT_B_PIN	(uint8_t) 0
#define SEGMENT_B_PORT	GPIOA

#define SEGMENT_C_PIN	(uint8_t) 8
#define SEGMENT_C_PORT	GPIOA

#define SEGMENT_D_PIN	(uint8_t) 5
#define SEGMENT_D_PORT	GPIOB

#define SEGMENT_E_PIN 	(uint8_t) 11
#define SEGMENT_E_PORT	GPIOA

#define SEGMENT_F_PIN 	(uint8_t) 3
#define SEGMENT_F_PORT	GPIOA

#define SEGMENT_G_PIN	(uint8_t) 4
#define SEGMENT_G_PORT	GPIOB

#define SEGMENT_DP_PIN	(uint8_t) 1
#define SEGMENT_DP_PORT	GPIOB

#define DIGIT_1_ON		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5)
#define DIGIT_2_ON		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4)
#define DIGIT_3_ON		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_6)
#define DIGIT_4_ON		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2)
#define DIGIT_TIME_ON	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7)

#define DIGIT_1_OFF		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5)
#define DIGIT_2_OFF		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4)
#define DIGIT_3_OFF		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6)
#define DIGIT_4_OFF		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2)
#define DIGIT_TIME_OFF	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7)

#define DISPLAY_SIZE	4

const uint8_t ascii_to_mask(char character);

void resetDigits(void);
void setDigits(void);
void resetSegments(void);
void setSegments(void);
void setDecimalPoint(void);
void updateDisplay(void);

void resetDigit(uint8_t);
void setDigit(uint8_t);

void fill_buffer(uint8_t *, uint8_t, uint8_t*, uint8_t, uint8_t);
void displayCharacter(uint8_t);
void displayMessage(uint8_t *, uint8_t);

#endif
