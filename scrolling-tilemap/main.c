#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"
#include <string.h>
#include <stdbool.h>

#define TILEMAP_WIDTH 160
#define TILEMAP_HEIGHT 120
#define TILEMAP_SIZE (TILEMAP_WIDTH * TILEMAP_HEIGHT)

const U8 tilemap[TILEMAP_SIZE];

U16 view_x = 0;
U16 view_y = 0;

U8 last_keycode;
bool is_keycode_pending = FALSE;

U0 KeyGet()
{
	if (*mail_flag != 0)
	{
		last_keycode = *mail_box;
		*mail_flag = 0;
		is_keycode_pending = TRUE;
	}
}

U0 KeyHandle()
{
	if (!is_keycode_pending)
		return;

	switch (last_keycode)
	{
		case 0x77: // w
			if (view_y > 0)
				view_y--;
			break;
		case 0x73: // s
			view_y++;
			break;
		case 0x61: // a
			if (view_x > 0)
				view_x--;
			break;
		case 0x64: // d
			view_x++;
			break;
	}
	is_keycode_pending = FALSE;
}


U0 TilemapSet(U16 x, U16 y, U8 tile)
{
	tilemap[TILEMAP_WIDTH * y + x] = tile;
}

U0 TilemapInit()
{
	U16 x;
	U16 y;

	memset(tilemap, 0x20, TILEMAP_SIZE);
	
	y = 0;
	for (x = 0; x < TILEMAP_WIDTH; x++)
	{
		TilemapSet(x, y, 0x8);
	}
	x = 0;
	for (y = 0; y < TILEMAP_HEIGHT; y++)
	{
		TilemapSet(x, y, 0x8);
	}
	TilemapSet(13, 13, 0xD);
	TilemapSet(17, 20, 0x1A);
	TilemapSet(17, 21, 0x1A);
	TilemapSet(18, 20, 0x1A);
	TilemapSet(18, 21, 0x1A);
	TilemapSet(24, 32, 0xD);
	TilemapSet(42, 9, 0xD);
}

U0 TilemapDraw()
{
	U16 i;
	U16 j;
	U16 tile_index = TILEMAP_WIDTH * view_y + view_x;

	for (i = 0, j = 0; i < VIDEO_RAM_SIZE; i++, j++)
	{
		if (j >= 40)
		{
			tile_index += TILEMAP_WIDTH - 40;
			j = 0;
		}
		video_ram[i] = tilemap[tile_index++];
	}
}

int main()
{
	TilemapInit();

	while (TRUE)
	{
		KeyGet();
		KeyHandle();
		TilemapDraw();
	}

	return 0;
}
