#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"

int main()
{
	U8 i = 0;
	U8 keycode;

	while (TRUE)
	{
		if (*mail_flag != 0)
		{
			keycode = *mail_box;
			*mail_flag = 0;

			*video_ram = keycode;
			*(video_ram + 64) = i++;

			if ((keycode >> 4) <= 9)
				*(video_ram + 128) = 0x30 + (keycode >> 4);
			else
				*(video_ram + 128) = 0x37 + (keycode >> 4);

			if ((keycode & 0x0F) <= 9)
				*(video_ram + 129) = 0x30 + (keycode & 0x0F);
			else
				*(video_ram + 129) = 0x37 + (keycode & 0x0F);

		}
	}

	return 0;
}
