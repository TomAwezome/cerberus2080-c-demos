#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02
#pragma output CLIB_MALLOC_HEAP_SIZE = 0xFFF

#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include "types.h"

#define CHARMAP_X 3

U64 *character_ram = 0xF000;
U16 character_ram_size = 0xF7FF - 0xF000;
U8 *video_ram = 0xF800;
U16 video_ram_size = 0xFCAF - 0xF800;

U16 charmap_draw_x = CHARMAP_X;
U16 charmap_draw_y = 6;
U16 bitmap_draw_x = CHARMAP_X + 18;
U16 bitmap_draw_y = 3;

U16 current_char = 0;

U8 current_bitmap_index = 0;

U8 blink;

U8 *mail_flag = 0x200;
U8 *mail_box = 0x201;

U8 last_keycode;
bool is_keycode_pending = FALSE;


U0 ScreenPrint(U16 x, U16 y, U8 *str)
{ // Write bytes of str to video ram at position x,y.
	U16 i;
	U16 l = strlen(str);

	for (i = 0; i < l; i++)
		video_ram[40 * y + x++] = str[i];
}

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

U0 DrawCharBitmapFrame()
{
	U16 i = 0;
	U16 x = bitmap_draw_x - 1;
	U16 y = bitmap_draw_y - 1;

	for (i = 0; i < 100; i++)
	{
		video_ram[40 * y + x] = 0x8;
		x++;
		if (x > bitmap_draw_x - 1 + 9)
		{
			y++;
			x = bitmap_draw_x - 1;
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
		if (i == current_bitmap_index && blink & 1)
			video_ram[40 * y + x] = 0x0;
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

U0 MoveBitmapCursorRight()
{
	if (current_bitmap_index < 63)
		current_bitmap_index++;
}

U0 MoveBitmapCursorLeft()
{
	if (current_bitmap_index > 0)
		current_bitmap_index--;
}

U0 MoveBitmapCursorUp()
{
	U16 c = 0;

	while (current_bitmap_index > 0 && c < 8)
	{
		current_bitmap_index--;
		c++;
	}
}

U0 MoveBitmapCursorDown()
{
	U16 c = 0;

	while (current_bitmap_index < 63 && c < 8)
	{
		current_bitmap_index++;
		c++;
	}
}

U0 DrawBitToCurrentChar()
{
	U16 row = current_bitmap_index / 8;
	U8 *current = character_ram + current_char;
	U8 current_bits;
	U16 shift = current_bitmap_index - (row * 8);

	current += row;
	current_bits = *current;
	
	current_bits = current_bits | (1 << (7 - shift));

	*current = current_bits;
}
U0 DeleteBitFromCurrentChar()
{
	U16 row = current_bitmap_index / 8;
	U8 *current = character_ram + current_char;
	U8 current_bits;
	U16 shift = current_bitmap_index - (row * 8);

	current += row;
	current_bits = *current;
	
	current_bits = current_bits & ~(1 << (7 - shift));

	*current = current_bits;
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
		case 0x17: // Ctrl-w
			MoveCharacterCursorUp();
			break;
		case 0x13: // Ctrl-s
			MoveCharacterCursorDown();
			break;
		case 0x01: // Ctrl-a
			MoveCharacterCursorLeft();
			break;
		case 0x04: // Ctrl-d
			MoveCharacterCursorRight();
			break;

		case 0x77: // w
			MoveBitmapCursorUp();
			break;
		case 0x73: // s
			MoveBitmapCursorDown();
			break;
		case 0x61: // a
			MoveBitmapCursorLeft();
			break;
		case 0x64: // d
			MoveBitmapCursorRight();
			break;

		case 0x20: // Space
		case 0x0D: // Enter
			DrawBitToCurrentChar();
			break;

		// Backspace key doesn't send to Mail Flag/Box... Use a key nearby :^)
		case 0x00: // F12
		case 0x1C: // Ctrl-Backslash
		case 0x1D: // Ctrl-]
		case 0x1B: // Ctrl-[
		case 0x05: // Delete
			DeleteBitFromCurrentChar();
		break;
	}
	is_keycode_pending = FALSE;
}

int main()
{
	DrawCharBitmapFrame();
	ClearCharBitmapArea();
	ScreenPrint(0, 26, "THE QUICK BROWN FOX");
	ScreenPrint(0, 27, "JUMPS OVER A LAZY DOG!?");
	ScreenPrint(0, 28, "the quick brown fox");
	ScreenPrint(0, 29, "jumps over a lazy dog.  :-)");

	while (TRUE)
	{
		DrawCurrentChar();
		DrawCharMap();
		GetKey();
		HandleKey();
		blink++;
	}

	return 0;
}

