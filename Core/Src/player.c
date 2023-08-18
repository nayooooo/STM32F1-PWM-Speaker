#include "player.h"

#include <stdlib.h>
#include <string.h>

static struct Player _player;

static void *_player_free(void *p)
{
	if (p == NULL) return NULL;
	free(p);
	p = NULL;
	return p;
}

static void _player_music_to_frequency_16bit(
	uint8_t *music, uint32_t music_len,
	uint16_t **freq, uint8_t **freq_lenght, uint32_t *freq_len,
	uint8_t beat
)
{
	extern const uint16_t player_tone_table_8mhz_16bit[];
	static const uint16_t *music_16bit_table = player_tone_table_8mhz_16bit;
	static const uint8_t music_C_index[8] = {  // 只使用后面7个
		12, 12, 14, 16, 17, 19, 21, 23,
	};
	
	// 申请频谱空间
	uint32_t music_digit_num = 0;  // 乐谱中数字数目
	for (uint32_t i = 0; i < 2 * music_len; i++) {
		if ((music[i] >= '0') && (music[i] <= '7')) {
			music_digit_num++;
		}
	}
	*freq = malloc(music_digit_num * sizeof(uint16_t));
	*freq_lenght = malloc(music_digit_num * sizeof(uint8_t));
	if (*freq == NULL || *freq_lenght == NULL) return;
	memset(*freq, 0, music_digit_num * sizeof(uint16_t));
	memset(*freq_lenght, 0, music_digit_num * sizeof(uint8_t));
	
	// 将乐谱转换成频谱
	uint8_t freq_beat;
	int freq_ptr;
	for (uint32_t i = 0, n = 0; n < music_digit_num; n++) {
		freq_ptr = 0;
		freq_beat = beat;
		
		// 是'|'
		if ((music[i] == '|') || (music[i] == ' ')) {
			i++;
		}
		
		// 是高低音
		switch(music[i])
		{
			case ',':  // 低音
				freq_ptr -= 12;
				i++;
				break;
			case '`':  // 高音
				freq_ptr += 12;
				i++;
				break;
		}
		
		// 是数字
		if ((music[i] >= '0') && (music[i] <= '7')) {
			if (music[i] == '0') {
				*freq_len = n + 1;
				return;
			}
			freq_ptr += music_C_index[music[i] - '0'];
			i++;
		}
		
		// 修正
	PLAYER_CONVERT_FREQ_LENGHT:
		switch(music[i])
		{
			case '#':  // 有半音，加一个音阶
				i++;
				freq_ptr++;
				goto PLAYER_CONVERT_FREQ_LENGHT;
			case '-':  // 有一个音节加长
				i++;
				freq_beat += 4;
				goto PLAYER_CONVERT_FREQ_LENGHT;
			case '_':  // 有一个音节缩短
				i++;
				freq_beat /= 2;
				goto PLAYER_CONVERT_FREQ_LENGHT;
			case '.':  // 有一个加半拍
				i++;
				freq_beat += freq_beat / 2;
				goto PLAYER_CONVERT_FREQ_LENGHT;
		}
		
		(*freq)[n] = music_16bit_table[freq_ptr];
		(*freq_lenght)[n] = freq_beat;
	}
	*freq_len = music_digit_num;
}

static void _player_music_to_frequency(
	uint8_t *music, uint32_t music_len,
	void **freq, uint8_t **freq_lenght, uint32_t *freq_len,
	uint8_t tone_size, uint8_t beat
)
{
	_player_free(*freq);
	_player_free(*freq_lenght);
	
	if (tone_size == 2) {
		_player_music_to_frequency_16bit(music, music_len, (uint16_t**)freq, freq_lenght, freq_len, beat);
	} else {
		return;
	}
}

static void _player_convert(Player_t player)
{
	if (player == NULL) {
		player = &_player;
		if (player->music == NULL) return;
	}
	_player_music_to_frequency(
		player->music, player->music_len,
		&player->music_freq_buff, &player->music_freq_lenght, &player->music_freq_len,
		player->tone_size, player->beat
	);
}

static void _player_play(Player_t player)
{
	if (player == NULL) {
		player = &_player;
		if (player->music_freq_buff == NULL) return;
	}
	player_play(player->music_freq_buff, player->music_freq_lenght, player->tone_size, player->music_freq_len, player->beat);
}

static void *_player_free_freq(Player_t player)
{
	if (player == NULL) {
		player = &_player;
		if (player->music_freq_buff == NULL) return NULL;
	}
	
	_player_free(player->music_freq_buff);
	_player_free(player->music_freq_lenght);
	
	return player;
}

int player_init(
	Player_t player, const char *name, uint8_t beat,
	void *music, uint8_t tone_size, uint32_t music_len
)
{
	if (player == NULL) {
		player = &_player;
		name = "default";
	}
	
	uint32_t name_len = strlen(name);
	if (name_len > PLAYER_NAME_LEN_MAX) name_len = PLAYER_NAME_LEN_MAX;
	else if (name_len == 0) return -1;
	player->beat = beat;
	
	memcpy(player->name, name, name_len);
	player->music = music;
	player->tone_size = tone_size;
	player->music_len = music_len;
	
	player->music_freq_buff = NULL;
	player->music_freq_lenght = NULL;
	player->music_freq_len = 0;
	
	player->flag = 0X00;
	
	player->play = _player_play;
	player->convert = _player_convert;
	
	player->free_freq = _player_free_freq;
	
	return 0;
}
