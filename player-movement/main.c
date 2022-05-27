#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"

const U8 player_character = 0x16;
U16 player_x = 0;
U16 player_y = 0;

int main()
{
	U16 i;
	U8 keycode;
	
	while (TRUE)
	{
		for (i = 0; i < VIDEO_RAM_SIZE; i++)
			video_ram[i] = 0x20; // clear screen (fill with space character)

		video_ram[40 * player_y + player_x] = player_character;

		do {} while (*mail_flag == 0);
		keycode = *mail_box;
		*mail_flag = 0;

		switch (keycode)
		{
			case 0x77: // w
				player_y--;
				break;
			case 0x73: // s
				player_y++;
				break;
			case 0x61: // a
				player_x--;
				break;
			case 0x64: // d
				player_x++;
				break;
		}
	}

	return 0;
}
