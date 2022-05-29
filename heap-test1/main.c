#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02
#pragma output CLIB_MALLOC_HEAP_SIZE = 0xFFF

#include "cerberus.h"
#include <stdlib.h>
#include <string.h>

int main()
{
	U8 *t = malloc(VIDEO_RAM_SIZE);
	U16 i;

	while (TRUE)
	{
		for (i = 0; i < VIDEO_RAM_SIZE; i++)
			t[i] = rand();

		memcpy(VIDEO_RAM, t, VIDEO_RAM_SIZE);

		t = malloc(VIDEO_RAM_SIZE);
		// since not freeing t, eventually malloc will hit
		// Out Of Memory error and return 0; program halts once heap is full
		if (t == NULL)
			break;
	}

	return 0;
}