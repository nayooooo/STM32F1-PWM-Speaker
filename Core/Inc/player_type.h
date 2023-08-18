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
	uint8_t tone_size;  // ���������ֽڴ�С
	uint32_t music_len;  // ����������
	
	void *music_freq_buff;  // ת����Ƶ�ʵ�����
	uint8_t *music_freq_lenght;  // ת����Ƶ��֮��ÿ������������
	uint32_t music_freq_len;  // ת�����Ƶ�׳���
	
	void		(*play)				(struct Player *player);
	void		(*convert)			(struct Player *player);
	
	void*		(*free_freq)		(struct Player *player);
};
typedef struct Player *Player_t;

#endif /* __PLAYER_TYPE_H__ */
