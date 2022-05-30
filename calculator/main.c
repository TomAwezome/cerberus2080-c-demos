#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define BG_CHAR 0x8
#define BUTTONS_X 8
#define BUTTONS_Y 7

#define KEY_ENTER 0x0D

const U8 view_area[VIDEO_RAM_SIZE];

U8 last_keycode;
bool is_keycode_pending = FALSE;
bool is_push_draw_pending = FALSE;

I16 result_value = 0;
I16 current_value = 0;

U0 ViewPrint(U8 x, U8 y, U8 *str)
{ // Write bytes of str to view area at position x,y.
	U16 i;
	U16 l = strlen(str);

	for (i = 0; i < l; i++)
		view_area[40 * y + x++] = str[i];
}

U0 ButtonDraw(U8 x, U8 y, U8 symbol)
{
	U8 video_x;
	U8 video_y;
	U8 i_x = 0;
	U8 i_y = 0;

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
			view_area[40 * video_y + video_x] = BG_CHAR;
			i_x++;
		}
		i_x = 0;
		i_y++;
	}
	i_x = 0;
	i_y = 0;
	if (is_push_draw_pending && last_keycode == symbol)
	{
		x++;
		y++;
		is_push_draw_pending = FALSE;
	}
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

U0 DigitsLineDraw(U8 x, U8 y)
{
	U8 str[128];

	sprintf(str, "%-6d", result_value);
	ViewPrint(x, y, str);

	sprintf(str, "%-6d", current_value);
	ViewPrint(x, y + 2, str);
}

U0 CalculatorDraw()
{
	DigitsLineDraw(BUTTONS_X + 9, BUTTONS_Y - 4);

	ButtonDraw(BUTTONS_X, BUTTONS_Y, '7');
	ButtonDraw(BUTTONS_X + 5, BUTTONS_Y, '8');
	ButtonDraw(BUTTONS_X + 10, BUTTONS_Y, '9');

	ButtonDraw(BUTTONS_X, BUTTONS_Y + 5, '4');
	ButtonDraw(BUTTONS_X + 5, BUTTONS_Y + 5, '5');
	ButtonDraw(BUTTONS_X + 10, BUTTONS_Y + 5, '6');

	ButtonDraw(BUTTONS_X, BUTTONS_Y + 10, '1');
	ButtonDraw(BUTTONS_X + 5, BUTTONS_Y + 10, '2');
	ButtonDraw(BUTTONS_X + 10, BUTTONS_Y + 10, '3');
	ButtonDraw(BUTTONS_X + 5, BUTTONS_Y + 15, '0');

	ButtonDraw(BUTTONS_X + 10, BUTTONS_Y + 15, '.');
	ButtonDraw(BUTTONS_X + 15, BUTTONS_Y + 5, '-');
	ButtonDraw(BUTTONS_X + 15, BUTTONS_Y + 10, '+');
	ButtonDraw(BUTTONS_X + 15, BUTTONS_Y + 15, '=');
	ButtonDraw(BUTTONS_X + 15, BUTTONS_Y, '*');
	ButtonDraw(BUTTONS_X + 20, BUTTONS_Y, '/');
	ButtonDraw(BUTTONS_X + 20, BUTTONS_Y + 5, '%');
	ButtonDraw(BUTTONS_X + 20, BUTTONS_Y + 10, '^');

}

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

	if (last_keycode == KEY_ENTER)
		last_keycode = '='; // override

	is_keycode_pending = FALSE;
	is_push_draw_pending = TRUE;
}

int main()
{
	memset(view_area, 0x8, VIDEO_RAM_SIZE);
	while (TRUE)
	{
		KeyGet();
		KeyHandle();
		CalculatorDraw();
		memcpy(VIDEO_RAM, view_area, VIDEO_RAM_SIZE);
	}

	return 0;
}
