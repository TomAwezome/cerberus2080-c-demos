#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02

#include "cerberus.h"

int main()
{
	U8 character = 0xFF;
	U8 *current = VIDEO_RAM;

	while (TRUE)
	{
		*current++ = character++;
		if (current >= VIDEO_RAM + VIDEO_RAM_SIZE)
			current = VIDEO_RAM;
	}

	return 0;
}
