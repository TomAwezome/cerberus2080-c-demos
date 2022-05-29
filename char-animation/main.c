#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"

const U64 wave_char1 = 0x00330C00CC300000;
const U64 wave_char2 = 0x0099060066180000;
const U64 wave_char3 = 0x00CC0300330C0000;

const U64 bird_char1 = 0x00205000044AA000;
const U64 bird_char2 = 0x005020000AA44000;

// char indices
#define WAVE 0xFF
#define BIRD 0xFE

int main()
{
	U16 i;
	U8 c;

	for (i = 20 * 30; i < VIDEO_RAM_SIZE; i++)
		video_ram[i] = WAVE;

	video_ram[40 * 8 + 5] = BIRD;
	video_ram[40 * 14 + 29] = BIRD;

	character_ram[WAVE] = wave_char1;
	character_ram[BIRD] = bird_char1;
	character_ram[WAVE] = wave_char1;

	i = 0;
	c = 0;
	while (TRUE)
	{
		if (i % 1024 == 0)
		{
			switch (c)
			{
				case 0:
					c++;
					character_ram[WAVE] = wave_char1;
					break;
				case 1:
					c++;
					character_ram[WAVE] = wave_char2;
					break;
				case 2:
					c++;
					character_ram[WAVE] = wave_char3;
					break;
				case 3:
					c = 0;
					character_ram[WAVE] = wave_char2;
					break;
			}
		}
		else if (i % 512 == 0)
			character_ram[BIRD] = bird_char2;
		else if (i % 256 == 0)
			character_ram[BIRD] = bird_char1;
			
		i++;
	}

	return 0;
}
