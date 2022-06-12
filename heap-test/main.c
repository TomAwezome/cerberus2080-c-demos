#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02
#pragma output CLIB_MALLOC_HEAP_SIZE = 0x7FFF
#define HEAP_SIZE 0x7FFF // ensure matches CLIB_MALLOC_HEAP_SIZE

#include "cerberus.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

U0 ScreenPrint(U16 x, U16 y, U8 *str)
{ // Write bytes of str to video ram at position x,y.
	U16 i;
	U16 l = strlen(str);

	for (i = 0; i < l; i++)
		video_ram[40 * y + x++] = str[i];
}

int main()
{
	U16 i;
	U16 j;
	U8 *chunk;
	U16 size;
	U8 *str;
	bool is_good = TRUE;
	U8 *program_end_estimate = 0x2000;

	// TEST 0
	// Program should be able to verify write/read success through heap space
	ScreenPrint(0, 0, "Test 0: Heap RAM verify");

	chunk = program_end_estimate + HEAP_SIZE;
	while (chunk > program_end_estimate)
	{
		*chunk = 217;
		if (*chunk != 217)
			is_good = FALSE;
		chunk--;
	}

	if (is_good)
		ScreenPrint(38, 0, ":)");
	else
		ScreenPrint(38, 0, ":(");

	// TEST 1
	// Program should be able to malloc and free regions from heap without issue as well as read/write verify
	ScreenPrint(0, 1, "Test 1: malloc, verify R/W, free");
	is_good = TRUE;

	i = 0;
	while (i < 30)
	{
		do
			size = rand();
		while (size == 0);

		chunk = malloc(size);
		if (chunk == NULL)
		{
			ScreenPrint(37, 1, ">");
			is_good = FALSE;
			break;
		}

		sprintf(str, "%-5d", size);
		ScreenPrint(0, 2, str);

		memset(chunk, 217, size);
		for (j = 0; j < size; j++)
		{
			if (chunk[j] != 217)
			{
				ScreenPrint(37, 1, "?");
				is_good = FALSE;
				return 0;
			}
		}

		free(chunk);
		i++;
	}

	if (is_good)
		ScreenPrint(38, 1, ":)");
	else
		ScreenPrint(38, 1, ":(");

	// TEST 2
	// Program will see how many consecutive bytes it can malloc from a 32k heap
	ScreenPrint(0, 2, "Test 2: consecutive of 32k");
	size = 1;
	chunk = malloc(size);
	while (chunk != NULL)
	{
		free(chunk);
		size++;
		chunk = malloc(size);
	}
	ScreenPrint(38, 2, ":)");
	sprintf(str, "%d", size - 1);
	ScreenPrint(32, 2, str);

	return 0;
}
