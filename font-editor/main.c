#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02
#pragma output CLIB_MALLOC_HEAP_SIZE = 0xFFF

#include "cerberus.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define CHARMAP_X 0
#define CHARMAP_Y 0

#define BITMAP_FRAME_X (CHARMAP_X + 16 + 1)
#define BITMAP_FRAME_Y 0

#define BITMAP_X (BITMAP_FRAME_X + 1)
#define BITMAP_Y (BITMAP_FRAME_Y + 1)

#define HEXCODE_X 0
#define HEXCODE_Y 19

U16 current_char = 0;

U8 current_bitmap_index = 0;

U8 blink;

U8 last_keycode;
bool is_keycode_pending = FALSE;


U0 ScreenPrint(U16 x, U16 y, U8 *str)
{ // Write bytes of str to video ram at position x,y.
	U16 i;
	U16 l = strlen(str);

	for (i = 0; i < l; i++)
		video_ram[40 * y + x++] = str[i];
}

U0 CharMapDraw()
{
	U16 i;
	U16 x = CHARMAP_X;
	U16 y = CHARMAP_Y;

	for (i = 0; i < 256; i++)
	{
		video_ram[40 * y + x] = i;
		if (current_char == i && blink & 1)
			video_ram[40 * y + x] = 0x0;
		x++;
		if (x >= CHARMAP_X + 16)
		{
			y++;
			x = CHARMAP_X;
		}
	}
}

U0 CharBitmapFrameDraw()
{
	U16 i = 0;
	U16 x = BITMAP_FRAME_X;
	U16 y = BITMAP_FRAME_Y;

	for (i = 0; i < 100; i++)
	{
		video_ram[40 * y + x] = 0x8;
		x++;
		if (x > BITMAP_FRAME_X + 9)
		{
			y++;
			x = BITMAP_FRAME_X;
		}
	}
}

U0 CharBitmapAreaClear()
{
	U16 i = 0;
	U16 x = BITMAP_X;
	U16 y = BITMAP_Y;

	for (i = 0; i < 64; i++)
	{
		video_ram[40 * y + x] = 0x20; // empty character
		x++;
		if (x > BITMAP_X + 7)
		{
			y++;
			x = BITMAP_X;
		}
	}
}

U0 CurrentCharDraw()
{
	U16 i = 0;
	U8 *current = character_ram + current_char;
	U8 current_bits = *current;
	U16 x = BITMAP_X;
	U16 y = BITMAP_Y;

	for (i = 0; i < 64; i++)
	{
		if (current_bits & 0x80)
			video_ram[40 * y + x] = 0xA0;
		if (i == current_bitmap_index && blink & 1)
			video_ram[40 * y + x] = 0x0;
		x++;
		current_bits = current_bits << 1;

		if (x > BITMAP_X + 7)
		{
			current++;
			current_bits = *current;
			y++;
			x = BITMAP_X;
		}
	}

	return 0;
}

U0 CharHexAreaClear()
{
	ScreenPrint(HEXCODE_X, HEXCODE_Y, "                       ");
}

U0 CurrentCharHexDraw()
{
	U8 str[128];
	U8 i = 0;
	U8 *current = character_ram + current_char;
	U8 current_bits = *current;

	while (i < 8)
	{
		sprintf(str, "%02X", current_bits);
		ScreenPrint(HEXCODE_X + i * 3, HEXCODE_Y, str);
		current++;
		current_bits = *current;
		i++;
	}
}

U0 CharacterCursorMoveRight()
{
	if (current_char < 255)
		current_char++;
}

U0 CharacterCursorMoveLeft()
{
	if (current_char > 0)
		current_char--;
}

U0 CharacterCursorMoveUp()
{
	U16 c = 0;

	while (current_char > 0 && c < 16)
	{
		current_char--;
		c++;
	}
}

U0 CharacterCursorMoveDown()
{
	U16 c = 0;

	while (current_char < 255 && c < 16)
	{
		current_char++;
		c++;
	}
}

U0 BitmapCursorMoveRight()
{
	if (current_bitmap_index < 63)
		current_bitmap_index++;
}

U0 BitmapCursorMoveLeft()
{
	if (current_bitmap_index > 0)
		current_bitmap_index--;
}

U0 BitmapCursorMoveUp()
{
	U16 c = 0;

	while (current_bitmap_index > 0 && c < 8)
	{
		current_bitmap_index--;
		c++;
	}
}

U0 BitmapCursorMoveDown()
{
	U16 c = 0;

	while (current_bitmap_index < 63 && c < 8)
	{
		current_bitmap_index++;
		c++;
	}
}

U0 CurrentCharBitDraw()
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
U0 CurrentCharBitDelete()
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

	CharBitmapAreaClear();
	CharHexAreaClear();

	switch (last_keycode)
	{
		case 0x17: // Ctrl-w
			CharacterCursorMoveUp();
			break;
		case 0x13: // Ctrl-s
			CharacterCursorMoveDown();
			break;
		case 0x01: // Ctrl-a
			CharacterCursorMoveLeft();
			break;
		case 0x04: // Ctrl-d
			CharacterCursorMoveRight();
			break;

		case 0x77: // w
			BitmapCursorMoveUp();
			break;
		case 0x73: // s
			BitmapCursorMoveDown();
			break;
		case 0x61: // a
			BitmapCursorMoveLeft();
			break;
		case 0x64: // d
			BitmapCursorMoveRight();
			break;

		case 0x20: // Space
		case 0x0D: // Enter
			CurrentCharBitDraw();
			break;

		// Backspace key doesn't send to Mail Flag/Box... Use a key nearby :^)
		case 0x00: // F12
		case 0x1C: // Ctrl-Backslash
		case 0x1D: // Ctrl-]
		case 0x1B: // Ctrl-[
		case 0x05: // Delete
			CurrentCharBitDelete();
		break;
	}
	is_keycode_pending = FALSE;
}

int main()
{
	CharBitmapFrameDraw();
	CharBitmapAreaClear();

	ScreenPrint(0, 22, "DELETE/F12 to erase.");
	ScreenPrint(0, 23, "SPACE/ENTER to draw.");
	ScreenPrint(0, 24, "WASD to move bitmap cursor.");
	ScreenPrint(0, 25, "CTRL-WASD to move charmap cursor.");
	ScreenPrint(0, 26, "THE QUICK BROWN FOX");
	ScreenPrint(0, 27, "JUMPS OVER A LAZY DOG!?");
	ScreenPrint(0, 28, "the quick brown fox");
	ScreenPrint(0, 29, "jumps over a lazy dog.  :-)");

	while (TRUE)
	{
		CurrentCharDraw();
		CharMapDraw();
		CurrentCharHexDraw();
		KeyGet();
		KeyHandle();
		blink++;
	}

	return 0;
}
