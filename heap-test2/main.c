#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02
#pragma output CLIB_MALLOC_HEAP_SIZE = 0x2FFF
#define HEAP_SIZE 0x2FFF // ensure matches CLIB_MALLOC_HEAP_SIZE

#include "cerberus.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

U16 bytes_malloced = 0;

U8 *WrappedMalloc(U16 bytes)
{
	if (bytes_malloced + bytes >= HEAP_SIZE)
		return 0xFFFF; // test value to use in program
	bytes_malloced += bytes;
	return malloc(bytes);
}

U0 ScreenPrint(U16 x, U16 y, U8 *str)
{ // Write bytes of str to video ram at position x,y.
	U16 i;
	U16 l = strlen(str);

	for (i = 0; i < l; i++)
		video_ram[40 * y + x++] = str[i];
}

int main()
{
	U8 *t;
	U16 i;
	U8 str[128];
	U8 size;

	bytes_malloced = 0; // commenting this line will cause WrappedMalloc and actual malloc heap to lose sync on program re-runs

	while (TRUE)
	{
		sprintf(str, "Malloced bytes: %d", bytes_malloced);
		ScreenPrint(0, 0, str);
		sprintf(str, "Heap Size: %d", HEAP_SIZE);
		ScreenPrint(0, 1, str);
		size = rand() / 5;
		t = WrappedMalloc(size);

		if (t == NULL)
		{
			ScreenPrint(0, 10, "malloc() error! (Out Of Memory)");
			break;
		}
		if (t == 0xFFFF)
		{
			ScreenPrint(0, 11, "WrappedMalloc() error! (Out Of Memory)");
			break;
		}
	}

	return 0;
}
