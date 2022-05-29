#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"
#include <string.h>

const U8 bitmap_area[VIDEO_RAM_SIZE];

U0 ScreenBitmapPixelDraw(U16 x, U16 y)
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

U16 MinU16(U16 a, U16 b)
{
	if (a < b)
		return a;
	else
		return b;
}

U0 ScreenBitmapLineDraw(U16 x1, U16 y1, U16 x2, U16 y2)
{
	U16 x_start = MinU16(x1, x2);
	U16 x_end;
	U16 y_start;
	U16 y_end;
	U16 x;
	U16 y;
	U16 dx;
	U16 dy;
	U16 c = 0;

	if (x_start == x1)
	{
		y_start = y1;
		x_end = x2;
		y_end = y2;
	}
	else // x2
	{
		y_start = y2;
		x_end = x1;
		y_end = y1;
	}

	x = x_start;
	y = y_start;
	dx = x_end - x_start;

	if (y_end < y_start)
	{
		dy = y_start - y_end;

		if (dx >= dy)
		{
			while (x <= x_end)
			{
				ScreenBitmapPixelDraw(x, y);
				c += dy;
				if (c >= dx)
				{
					c -= dx;
					y--;
				}
				x++;
			}
		}
		else
		{
			while (y > y_end)
			{
				ScreenBitmapPixelDraw(x, y);
				c += dx;
				if (c >= dy)
				{
					c -= dy;
					x++;
				}
				y--;
			}
		}
	}
	else
	{
		dy = y_end - y_start;

		if (dx >= dy)
		{
			while (x <= x_end)
			{
				ScreenBitmapPixelDraw(x, y);
				c += dy;
				if (c >= dx)
				{
					c -= dx;
					y++;
				}
				x++;
			}
		}
		else
		{
			while (y <= y_end)
			{
				ScreenBitmapPixelDraw(x, y);
				c += dx;
				if (c >= dy)
				{
					c -= dy;
					x++;
				}
				y++;
			}
		}
	}
}

U16 cur_x = 10;
U16 cur_y = 13;

U0 ScreenBitmapFrameDraw()
{
	memset(bitmap_area, 0x20, VIDEO_RAM_SIZE);
	ScreenBitmapLineDraw(0, 0, cur_x, cur_y);
	ScreenBitmapLineDraw(78, 58, cur_x + 9, cur_y + 9);
	ScreenBitmapLineDraw(0, 58, cur_x, cur_y + 9);
	ScreenBitmapLineDraw(78, 0, cur_x + 9, cur_y);
	ScreenBitmapLineDraw(cur_x, cur_y, cur_x + 9, cur_y);
	ScreenBitmapLineDraw(cur_x, cur_y, cur_x, cur_y + 9);
	ScreenBitmapLineDraw(cur_x, cur_y + 9, cur_x + 9, cur_y + 9);
	ScreenBitmapLineDraw(cur_x + 9, cur_y, cur_x + 9, cur_y + 9);
	cur_x++;
	cur_y++;
	if (cur_x >= 80)
		cur_x = 0;
	if (cur_y >= 60)
		cur_y = 0;
}

int main()
{
	memset(bitmap_area, 0x20, VIDEO_RAM_SIZE);
	while (TRUE)
	{
		ScreenBitmapFrameDraw();
		memcpy(VIDEO_RAM, bitmap_area, VIDEO_RAM_SIZE);
	}

	return 0;
}
