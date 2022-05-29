#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"
#include <string.h>

#define BG_CHAR 0x8
#define BUTTONS_X 5
#define BUTTONS_Y 7

const U8 view_area[VIDEO_RAM_SIZE];

U0 ButtonDraw(U16 x, U16 y, U8 symbol)
{
	U16 video_x;
	U16 video_y;
	U16 i_x = 0;
	U16 i_y = 0;

	for (video_y = y + 1; i_y < 3 && video_y < 30; video_y++)
	{
		for (video_x = x + 1; i_x < 3 && video_x < 40; video_x++)
		{
			view_area[40 * video_y + video_x] = 0x20;
			i_x++;
		}
		i_x = 0;
		i_y++;
	}
	i_x = 0;
	i_y = 0;
	for (video_y = y; i_y < 3 && video_y < 30; video_y++)
	{
		for (video_x = x; i_x < 3 && video_x < 40; video_x++)
		{
			view_area[40 * video_y + video_x] = 0xA0;
			i_x++;
		}
		i_x = 0;
		i_y++;
	}

	view_area[40 * (y + 1) + x + 1] = symbol;
}

U0 CalculatorDraw()
{
	ButtonDraw(BUTTONS_X, BUTTONS_Y, '7');
	ButtonDraw(BUTTONS_X + 5, BUTTONS_Y, '8');
	ButtonDraw(BUTTONS_X + 10, BUTTONS_Y, '9');

	ButtonDraw(BUTTONS_X, BUTTONS_Y + 5, '4');
	ButtonDraw(BUTTONS_X + 5, BUTTONS_Y + 5, '5');
	ButtonDraw(BUTTONS_X + 10, BUTTONS_Y + 5, '6');

	ButtonDraw(BUTTONS_X, BUTTONS_Y + 10, '1');
	ButtonDraw(BUTTONS_X + 5, BUTTONS_Y + 10, '2');
	ButtonDraw(BUTTONS_X + 10, BUTTONS_Y + 10, '3');

}

int main()
{
	memset(view_area, 0x8, VIDEO_RAM_SIZE);
	while (TRUE)
	{
		CalculatorDraw();
		memcpy(VIDEO_RAM, view_area, VIDEO_RAM_SIZE);
	}

	return 0;
}
