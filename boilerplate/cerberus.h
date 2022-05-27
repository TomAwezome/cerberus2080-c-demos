#ifndef _STDINT_H
#include <stdint.h>
#endif
#define U8	uint8_t
#define U16	uint16_t
#define U32	uint32_t
#define U64	uint64_t
#define I8	int8_t
#define I16	int16_t
#define I32	int32_t
#define I64	int64_t
#define U0	void
#define TRUE	1
#define FALSE	0
#define NULL    0
#define MIN_U8  0x00
#define MAX_U8  0xFF
#define MIN_I8  -0x80
#define MAX_I8  0x7F
#define MIN_U16 0x0000
#define MAX_U16 0xFFFF
#define MIN_I16 -0x8000
#define MAX_I16 0x7FFF
#define MIN_U32 0x0000
#define MAX_U32 0xFFFFFFFF
#define MIN_I32 -0x80000000
#define MAX_I32 0x7FFFFFFF
#define MIN_U64 0x0000000000000000
#define MAX_U64 0xFFFFFFFFFFFFFFFF
#define MIN_I64 -0x8000000000000000
#define MAX_I64 0x7FFFFFFFFFFFFFFF

#define MAIL_FLAG 0x200
#define MAIL_BOX 0x201
#define CHARACTER_RAM 0xF000
#define CHARACTER_RAM_SIZE (8 * 256)
#define CHARACTER_RAM_END (CHARACTER_RAM + CHARACTER_RAM_SIZE - 1)
#define VIDEO_RAM 0xF800
#define VIDEO_RAM_SIZE (40 * 30)
#define VIDEO_RAM_END (VIDEO_RAM + VIDEO_RAM_SIZE - 1)

const U8 *video_ram = VIDEO_RAM;
const U8 *mail_flag = MAIL_FLAG;
const U8 *mail_box = MAIL_BOX;
const U64 *character_ram = CHARACTER_RAM;
