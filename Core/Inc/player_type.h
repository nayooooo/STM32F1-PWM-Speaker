#ifndef __PLAYER_TYPE_H__
#define __PLAYER_TYPE_H__

#include <stdint.h>

#ifndef NULL
#	define NULL			(0)
#endif  // !NULL

#include "player_conf.h"

struct Player
{
	uint8_t name[PLAYER_NAME_LEN_MAX];
	
	uint8_t flag;
	
	uint8_t beat;
	
	void *music;
	uint8_t tone_size;  // 单个音调字节大小
	uint32_t music_len;  // 乐谱音调数
	
	void *music_freq_buff;  // 转换成频率的乐谱
	uint8_t *music_freq_lenght;  // 转换成频率之后，每个音调的音长
	uint32_t music_freq_len;  // 转换后的频谱长度
	
	void		(*play)				(struct Player *player);
	void		(*convert)			(struct Player *player);
	
	void*		(*free_freq)		(struct Player *player);
};
typedef struct Player *Player_t;

#endif /* __PLAYER_TYPE_H__ */
