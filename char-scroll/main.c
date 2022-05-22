#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include <stdint.h>
#include "types.h"

U8 *video_ram = 0xF800;
U16 video_ram_size = 0xFCAF - 0xF800;

int main()
{
	U16 i = 0;
	U8 character = 0xFF;

	while (TRUE)
	{
		*(video_ram + i++) = character++;
		if (i > video_ram_size)
		{
			i = 0;
		}
	}

	return 0;
}
