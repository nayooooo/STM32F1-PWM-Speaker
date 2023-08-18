#include "player_io.h"

#include "stm32f1xx_hal.h"
#include "tim.h"

void player_write_tone_16bit(uint16_t tone, uint8_t tone_lenght, uint8_t beat)
{
//	float pulse_pow = 30.0 / 92;
	float pulse_pow = 2.0 / 9;
	
	uint32_t per_beat = 1000 / beat;
	TIM_OC_InitTypeDef sConfigOC = {
		.OCMode = TIM_OCMODE_PWM1,
		.OCPolarity = TIM_OCPOLARITY_HIGH,
		.OCFastMode = TIM_OCFAST_DISABLE,
	};
	htim2.Init.Period = tone;
	HAL_TIM_Base_Init(&htim2);
	sConfigOC.Pulse = tone * pulse_pow;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_Delay(per_beat * tone_lenght);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
}

static void player_play_16bit(uint16_t *freq, uint8_t *freq_lenght, uint32_t freq_len, uint8_t beat)
{
	uint32_t per_beat = 1000 / beat / 10;
	
	for (uint32_t i = 0; i < freq_len; i++) {
		player_write_tone_16bit(freq[i], freq_lenght[i], beat);
		HAL_Delay(per_beat);
	}
}

void player_play(void *freq, uint8_t *freq_lenght, uint8_t tone_size, uint32_t freq_len, uint8_t beat)
{
	if (tone_size == 2) {
		player_play_16bit((uint16_t*)freq, freq_lenght, freq_len, beat);
	} else {
		return;
	}
}
