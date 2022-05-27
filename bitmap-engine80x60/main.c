#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"
#include <string.h>

const U8 bitmap_area[VIDEO_RAM_SIZE];

U0 ScreenBitmapDraw(U16 x, U16 y)
{
	U16 cell_x = x / 2;
	U16 cell_y = y / 2;
	U16 quad_x = x & 1;
	U16 quad_y = y & 1;
	U16 index = 40 * cell_y + cell_x;
	U8 byte = bitmap_area[index];

	switch (quad_x)
	{
		case 0:
			switch (quad_y)
			{
				case 0: // QUAD TOP LEFT
					switch (byte)
					{
						case 0x20: // empty char
							bitmap_area[index] = 0x01; // set top left char
							break;
						case 0x02: // top right char
							bitmap_area[index] = 0x03; // set top char
							break;
						case 0x87: // bottom right char
							bitmap_area[index] = 0x86; // set top left bottom right char
							break;
						case 0x04: // bottom left char
							bitmap_area[index] = 0x05; // set left char
							break;
						case 0x83: // bottom char
							bitmap_area[index] = 0x82; // set all but top right char
							break;
						case 0x85: // right char
							bitmap_area[index] = 0x84; // set all but bottom left char
							break;
						case 0x06: // top right bottom left char
							bitmap_area[index] = 0x07; // set all but bottom right char
							break;
						case 0x81: // all but top left char
							bitmap_area[index] = 0xA0; // set inverse empty char
							break;
					}
					break;
				case 1: // QUAD BOTTOM LEFT
					switch (byte)
					{
						case 0x20: // empty char
							bitmap_area[index] = 0x04; // set bottom left char
							break;
						case 0x02: // top right char
							bitmap_area[index] = 0x06; // set top right bottom left char
							break;
						case 0x87: // bottom right char
							bitmap_area[index] = 0x83; // set bottom char
							break;
						case 0x01: // top left char
							bitmap_area[index] = 0x05; // set left char
							break;
						case 0x03: // top char
							bitmap_area[index] = 0x07; // set all but bottom right char
							break;
						case 0x85: // right char
							bitmap_area[index] = 0x81; // set all but top left
							break;
						case 0x86: // bottom right top left char
							bitmap_area[index] = 0x82; // set all but top right char
							break;
						case 0x84: // all but bottom left char
							bitmap_area[index] = 0xA0; // set inverse empty char
							break;
					}
					break;
			}
			break;
		case 1:
			switch (quad_y)
			{
				case 0: // QUAD TOP RIGHT
					switch (byte)
					{
						case 0x20: // empty char
							bitmap_area[index] = 0x02; // set top right char
							break;
						case 0x01: // top left char
							bitmap_area[index] = 0x03; // set top char
							break;
						case 0x87: // bottom right char
							bitmap_area[index] = 0x85; // set right char
							break;
						case 0x04: // bottom left char
							bitmap_area[index] = 0x06; // set top right bottom left char
							break;
						case 0x83: // bottom char
							bitmap_area[index] = 0x81; // set all but top left char
							break;
						case 0x05: // left char
							bitmap_area[index] = 0x07; // set all but bottom right char
							break;
						case 0x86: // bottom right top left char
							bitmap_area[index] = 0x84; // set all but bottom left char
							break;
						case 0x82: // all but top right char
							bitmap_area[index] = 0xA0; // set inverse empty char
							break;
					}
					break;
				case 1: // QUAD BOTTOM RIGHT
					switch (byte)
					{
						case 0x20: // empty char
							bitmap_area[index] = 0x87; // set bottom right char
							break;
						case 0x02: // top right char
							bitmap_area[index] = 0x85; // set right char
							break;
						case 0x04: // bottom left char
							bitmap_area[index] = 0x83; // set bottom char
							break;
						case 0x01: // top left char
							bitmap_area[index] = 0x86; // set bottom right top left char
							break;
						case 0x03: // top char
							bitmap_area[index] = 0x84; // set all but bottom left char
							break;
						case 0x05: // left char
							bitmap_area[index] = 0x82; // set all but top right char
							break;
						case 0x06: // top right bottom left char
							bitmap_area[index] = 0x81; // set all but top left char
							break;
						case 0x07: // all but bottom right char
							bitmap_area[index] = 0xA0; // set inverse empty char
							break;
					}
					break;
			}
			break;
	}
}

int main()
{
	U16 x = 0;
	U16 y = 0;
	memset(bitmap_area, 0x20, VIDEO_RAM_SIZE);
	while (TRUE)
	{
		memset(bitmap_area, 0x20, VIDEO_RAM_SIZE);
		ScreenBitmapDraw(x, y);
		memcpy(VIDEO_RAM, bitmap_area, VIDEO_RAM_SIZE);
		x++;
		if (x >= 80)
		{
			x = 0;
			y++;
		}
		if (y >= 60)
			y = 0;
	}

	return 0;
}
