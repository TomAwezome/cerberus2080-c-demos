#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include <stdint.h>
#include "types.h"

U8 *video_ram = 0xF800;
U8 *mail_flag = 0x200;
U8 *mail_box = 0x201;

int main()
{
	U8 i = 0;

	while (TRUE)
	{
		if (*mail_flag != 0)
		{
			*video_ram = *mail_box;
			*(video_ram + 64) = i++;
			*mail_flag = 0;
		}
	}

	return 0;
}
