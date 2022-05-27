#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"
#include <string.h>

const U8 bitmap_area[VIDEO_RAM_SIZE];

U0 ScreenBitmapClear()
{
	U16 i;
	for (i = 0; i < VIDEO_RAM_SIZE; i++)
		bitmap_area[i] = 0;
}

U0 ScreenBitmapDraw(U16 x, U16 y)
{
	U16 cell_x = x / 2;
	U16 cell_y = y / 2;
	U16 quad_x = x % 2;
	U16 quad_y = y % 2;
	U8 byte = bitmap_area[40 * cell_y + cell_x];

	switch (quad_x)
	{
		case 0:
			switch (quad_y)
			{
				case 0:
					bitmap_area[40 * cell_y + cell_x] = byte | 1;
					break;
				case 1:
					bitmap_area[40 * cell_y + cell_x] = byte | 4;
					break;
			}
			break;
		case 1:
			switch (quad_y)
			{
				case 0:
					bitmap_area[40 * cell_y + cell_x] = byte | 2;
					break;
				case 1:
					bitmap_area[40 * cell_y + cell_x] = byte | 8;
					break;
			}
			break;
	}
}
// 0000b empty
// 0001b top left
// 0010b top right
// 0100b bottom left
// 1000b bottom right
// 0011b top
// 1100b bottom
// 0101b left
// 1010b right
// 1001b top left bottom right
// 0110b bottom left top right
// 1110b all but top left
// 1101b all but top right
// 1011b all but bottom left
// 0111b all but bottom right
// 1111b is all
U0 ScreenBitmapRender()
{
	U16 i;
	U8 byte;
	
	for (i = 0; i < VIDEO_RAM_SIZE; i++)
	{
		byte = bitmap_area[i];
		switch (byte)
		{
			case 0:
				video_ram[i] = 0x20; // space char
				break;
			case 1:
				video_ram[i] = 0x01; // top left char
				break;
			case 2:
				video_ram[i] = 0x02; // top right char
				break;
			case 4:
				video_ram[i] = 0x04; // bottom left char
				break;
			case 8:
				video_ram[i] = 0x87; // bottom right char
				break;
			case 3:
				video_ram[i] = 0x03; // top char
				break;
			case 12:
				video_ram[i] = 0x83; // bottom char
				break;
			case 5:
				video_ram[i] = 0x05;// left char
				break;
			case 10:
				video_ram[i] = 0x85;// right char
				break;
			case 9:
				video_ram[i] = 0x86;// top left bottom right char
				break;
			case 6:
				video_ram[i] = 0x06; // bottom left top right char
				break;
			case 14:
				video_ram[i] = 0x81; // all but top left char
				break;
			case 13:
				video_ram[i] = 0x82; // all but top right char
				break;
			case 11:
				video_ram[i] = 0x84; // all but bottom left char
				break;
			case 7:
				video_ram[i] = 0x07; // all but bottom right char
				break;
			case 15:
				video_ram[i] = 0xA0; // all char
				break;
		}
	}

}

int main()
{
	U16 x = 0;
	U16 y = 7;
	ScreenBitmapClear();
	while (TRUE)
	{
		ScreenBitmapDraw(x, y);
		if (x & 1)
			ScreenBitmapDraw(x, y + 1);
		ScreenBitmapRender();
		x++;
		if (x >= 80)
		{
			x = 0;
			y++;
		}
	}

	return 0;
}
