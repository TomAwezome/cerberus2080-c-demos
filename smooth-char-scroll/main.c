#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include <stdint.h>
#include "types.h"

#define VIDEO_RAM 0xF800
#define VIDEO_RAM_SIZE (40 * 30)
#define VIDEO_RAM_END (VIDEO_RAM + VIDEO_RAM_SIZE - 1)
#define CHARACTER_RAM 0xF000
#define CHARACTER_RAM_SIZE (8 * 256)
#define CHARACTER_RAM_END (CHARACTER_RAM + CHARACTER_RAM_SIZE - 1)

int main()
{
	U64 *current_char;
	U8 *current_byte = VIDEO_RAM;
	U8 character = 0x0;
	U64 char_bitmap;
	U64 wrap;

	// fill screen with some characters
	while (current_byte <= VIDEO_RAM_END)
		*current_byte++ = character++;

	// scroll character bitmaps
	while (TRUE)
	{
		for (current_char = CHARACTER_RAM; current_char <= CHARACTER_RAM_END; current_char++)
		{
			char_bitmap = *current_char;
			wrap = char_bitmap & 1;
			char_bitmap = wrap << 63 | char_bitmap >> 1;
			*current_char = char_bitmap;
		}
	}

	return 0;
}
