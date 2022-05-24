#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02
#pragma output CLIB_MALLOC_HEAP_SIZE = 0x8000 // 32kb heap

#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include "types.h"

U8 *video_ram = 0xF800;
U16 video_ram_size = 0xFCAF - 0xF800;

U8 *mail_flag = 0x200;
U8 *mail_box = 0x201;

U8 last_keycode;
bool is_keycode_pending = FALSE;

U8 *file_area = 0xA000;
U16 file_area_size = 0xEFFF - 0xA000;
/*
	Load files into memory using the BIOS command 'load' to address A000.
	Example: "load test.txt A000".
	To save a file, hit ESC to go back to the BIOS, and use the 'save'
	command.
	Example: "save A000 EFFF test.txt"
	TODO: EFFF is the end of User Block 1, not the end location of file

	The 'file area size' (from A000 to the end of User Block 1) results
	in a file size max of about 20kb. Files over that will overwrite
	character and video RAM, and eventually the program stack.
	0xA000 is arbitrarily chosen to allow for the program's code, data,
	and bss+heap sections to have 40kb of breathing room.
*/

U16 view_cursor_raw_index = 0;
U16 cursor_raw_index = 0;

U8 view_area[40 * 30];

U8 blink;

U0 GetKey()
{
		if (*mail_flag != 0)
		{
			last_keycode = *mail_box;
			*mail_flag = 0;
			is_keycode_pending = TRUE;
		}
}

U0 WriteCharToFile(U8 character)
{
	U16 i = cursor_raw_index;
	U8 c = file_area[i];
	U8 *remaining_file_len = 0;
	U8 *shifted_text;

	while (c != 0xFF)
	{
		remaining_file_len++;
		c = file_area[++i];
	}

	shifted_text = malloc(remaining_file_len + 1);
	memcpy(shifted_text, file_area + cursor_raw_index, remaining_file_len + 1);
	file_area[cursor_raw_index++] = character;
	memcpy(file_area + cursor_raw_index, shifted_text, remaining_file_len + 1);

	free(shifted_text);
}

U0 DeleteCharFromFile()
{
	U16 i = cursor_raw_index;
	U8 c = file_area[i];
	U8 *remaining_file_len = 0;
	U8 *shifted_text;

	while (c != 0xFF)
	{
		remaining_file_len++;
		c = file_area[++i];
	}
	
	if (remaining_file_len == 0)
		return;

	shifted_text = malloc(remaining_file_len);
	memcpy(shifted_text, file_area + cursor_raw_index + 1, remaining_file_len);
	memcpy(file_area + cursor_raw_index, shifted_text, remaining_file_len);

	free(shifted_text);
}

U0 HandleKey()
{
		if (!is_keycode_pending)
			return;
		if (last_keycode >= 0x20 && last_keycode < 0xF0)
		{
			// type that key at cursor in file
			WriteCharToFile(last_keycode);

			is_keycode_pending = FALSE;
			return;
		}
		switch (last_keycode)
		{
			case 0x17: // Ctrl-W
				break;
			case 0x13: // Ctrl-S
				break;
			case 0x01: // Ctrl-A
				if (cursor_raw_index > 0)
					cursor_raw_index--;
				break;
			case 0x04: // Ctrl-D
				cursor_raw_index++;
				break;

			case 0x0D: // Enter
				WriteCharToFile(0x0A);
				break;

			case 0x05: // Delete
				DeleteCharFromFile();

		}
		is_keycode_pending = FALSE;
}

U0 DrawFileToViewArea()
{
	U16 i = view_cursor_raw_index;
	U8 c = file_area[i];
	U16 x = 0;
	U16 y = 0;

	memset(view_area, 0x20, 40 * 30); // fill with empty (space) character

	while (c != 0xFF && y < 30)
	{
		if (file_area + i > 0xEFFF) // error
			return;
		if (c == 0x0A) // newline
		{
			if (i == cursor_raw_index && blink & 1)
				view_area[40 * y + x] = 0x0; //draw cursor
			y++;
			x = 0;
		}
		else
		{
			view_area[40 * y + x] = c;
			if (i == cursor_raw_index && blink & 1)
				view_area[40 * y + x] = 0x0; //draw cursor
			x++;
			if (x >= 40)
			{
				y++;
				x = 0;
			}
		}

		i++;
		c = file_area[i];
	}	
}

int main()
{
	U16 i;
	
	while (TRUE)
	{
		DrawFileToViewArea();
		memcpy(video_ram, view_area, video_ram_size + 1);

		GetKey();
		HandleKey();
		blink++;
	}

	return 0;
}
