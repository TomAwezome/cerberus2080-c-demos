#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include <stdint.h>
#include "types.h"

U8 *video_ram = 0xF800;
U16 video_ram_size = 0xFCAF - 0xF800;
U64 *character_ram = 0xF000;
U16 character_ram_size = 0xF7FF - 0xF000;

U16 charmap_draw_x = 3;
U16 charmap_draw_y = 6;

//U16 bitmap_draw_x = charmap_draw_x + 18;
U16 bitmap_draw_x = 3 + 18;

U16 bitmap_draw_y = 3;

U16 current_char = 0;

U8 blink;

U0 DrawCharMap()
{
	U16 i;
	U16 x = charmap_draw_x;
	U16 y = charmap_draw_y;

	for (i = 0; i < 256; i++)
	{
		video_ram[40 * y + x] = i;
		if (current_char == i && blink & 1)
			video_ram[40 * y + x] = 0x0;
		x++;
		if (x >= charmap_draw_x + 16)
		{
			y++;
			x = charmap_draw_x;
		}
	}
}

U0 DrawCurrentChar()
{
	U16 i;
	U16 x = bitmap_draw_x;
	U16 y = bitmap_draw_y;
	U64 c = character_ram[current_char];
	U8 bits = c;
	
	for (i = 0; i < 64; i++)
	{
		if (bits & 1)
			video_ram[40 * y + x] = 0x0; // test
//		bits = bits >> 1;
		x++;
		if (x >= bitmap_draw_x + 8)
		{
			y++;
			x = bitmap_draw_x;
//			bits = c >> (i - 1);
		}
	}
}

int main()
{
	while (TRUE)
	{
		DrawCharMap();
		DrawCurrentChar();
		blink++;
	}

	return 0;
}
