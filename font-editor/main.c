#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include <stdint.h>
#include <stdbool.h>
#include "types.h"

U64 *character_ram = 0xF000;
U16 character_ram_size = 0xF7FF - 0xF000;
U8 *video_ram = 0xF800;
U16 video_ram_size = 0xFCAF - 0xF800;

U16 charmap_draw_x = 3;
U16 charmap_draw_y = 6;

//U16 bitmap_draw_x = charmap_draw_x + 18;
U16 bitmap_draw_x = 3 + 18;
U16 bitmap_draw_y = 3;

U16 current_char = 0;

U8 blink;

U8 *mail_flag = 0x200;
U8 *mail_box = 0x201;

U8 last_keycode;
bool is_keycode_pending = FALSE;

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

U0 ClearCharBitmapArea()
{
	U16 i = 0;
	U16 x = bitmap_draw_x;
	U16 y = bitmap_draw_y;

	for (i = 0; i < 64; i++)
	{
		video_ram[40 * y + x] = 0x20; // empty character
		x++;
		if (x > bitmap_draw_x + 7)
		{
			y++;
			x = bitmap_draw_x;
		}
	}
}

U0 DrawCurrentChar()
{
	U16 i = 0;
	U8 *current = character_ram + current_char;
	U8 current_bits = *current;
	U16 x = bitmap_draw_x;
	U16 y = bitmap_draw_y;

	for (i = 0; i < 64; i++)
	{
		if (current_bits & 0x80)
			video_ram[40 * y + x] = 0xA0;

		x++;
		current_bits = current_bits << 1;

		if (x > bitmap_draw_x + 7)
		{
			current++;
			current_bits = *current;
			y++;
			x = bitmap_draw_x;
		}
	}

	return 0;
}

U0 MoveCharacterCursorRight()
{
	if (current_char < 255)
		current_char++;
}

U0 MoveCharacterCursorLeft()
{
	if (current_char > 0)
		current_char--;
}

U0 MoveCharacterCursorUp()
{
	U16 c = 0;

	while (current_char > 0 && c < 16)
	{
		current_char--;
		c++;
	}
}

U0 MoveCharacterCursorDown()
{
	U16 c = 0;

	while (current_char < 255 && c < 16)
	{
		current_char++;
		c++;
	}
}

U0 GetKey()
{
	if (*mail_flag != 0)
	{
		last_keycode = *mail_box;
		*mail_flag = 0;
		is_keycode_pending = TRUE;
	}
}

U0 HandleKey()
{
	if (!is_keycode_pending)
		return;

	ClearCharBitmapArea();

	switch (last_keycode)
	{
		case 0x17: // Ctrl-W
			MoveCharacterCursorUp();
			break;
		case 0x13: // Ctrl-S
			MoveCharacterCursorDown();
			break;
		case 0x01: // Ctrl-A
			MoveCharacterCursorLeft();
			break;
		case 0x04: // Ctrl-D
			MoveCharacterCursorRight();
			break;

		case 0x20: // Space(?)
		case 0x0D: // Enter
//			WriteCharToFile(0x0A);
			break;

		case 0x05: // Delete
//			DeleteCharFromFile();
			break;

		// Backspace key doesn't send to Mail Flag/Box... Use a key nearby :^)
		case 0x00: // F12
		case 0x1C: // Ctrl-Backslash
		case 0x1D: // Ctrl-]
		case 0x1B: // Ctrl-[
//			BackspaceCharFromFile();

	}
	is_keycode_pending = FALSE;
}

int main()
{
	while (TRUE)
	{
		DrawCharMap();
		DrawCurrentChar();
		GetKey();
		HandleKey();
		blink++;
	}

	return 0;
}

