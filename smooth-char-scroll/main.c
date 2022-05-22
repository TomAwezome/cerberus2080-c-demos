#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include <stdint.h>
#include "types.h"

U64 *character_ram = 0xF000;
U16 character_ram_size = 0xF7FF - 0xF000;
U8 *video_ram = 0xF800;
U16 video_ram_size = 0xFCAF - 0xF800;

int main()
{
	U16 i = 0;
	U8 character = 0xFF;
	U64 *current;
	U64 char_bitmap;
	U64 wrap;

	// fill screen with some characters
	while (i < video_ram_size)
	{
		*(video_ram + i++) = character++;
	}

	// scroll character bitmaps
	while (TRUE)
	{
		for (i = 0; i <= character_ram_size / 8; i++)
		{
			current = character_ram + i;
			char_bitmap = *current;
			wrap = char_bitmap & 1;
			char_bitmap = wrap << 63 | char_bitmap >> 1;
			*current = char_bitmap;
		}
	}

	return 0;
}
