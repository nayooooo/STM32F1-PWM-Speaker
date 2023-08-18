#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "player_type.h"
#include "player_io.h"

int player_init(
	Player_t player, const char *name, uint8_t beat,
	void *music, uint8_t tone_size, uint32_t music_len
);

#endif /* __PLAYER_H__ */
