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

U8 status_line_num = 0;

U0 TestStatusPrint(bool is_good)
{
	U8 *str;
	if (is_good)
		str = ":)";
	else
		str = ":(";

	ScreenPrint(38, status_line_num++, str);
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
	U8 line_num = 0;
	U8 test_num = 0;

	// TEST: Verify write/read success through heap space
	ScreenPrint(0, line_num, "Test 0: Heap RAM verify");

	chunk = program_end_estimate + HEAP_SIZE;
	while (chunk > program_end_estimate)
	{
		*chunk = 217;
		if (*chunk != 217)
			is_good = FALSE;
		chunk--;
	}

	TestStatusPrint(is_good);

	line_num++;
	test_num++;
	// TEST: malloc and free regions from heap, read/write verify
	sprintf(str, "Test %d: malloc, verify R/W, free", test_num);
	ScreenPrint(0, line_num, str);
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
		ScreenPrint(0, line_num + 1, str);

		memset(chunk, 217, size);
		for (j = 0; j < size; j++)
		{
			if (chunk[j] != 217)
			{
				ScreenPrint(37, line_num, "?");
				is_good = FALSE;
				return 0;
			}
		}

		free(chunk);
		i++;
	}

	TestStatusPrint(is_good);

	line_num++;
	test_num++;
	// TEST: Determine max consecutive bytes can malloc from a 32k heap
	sprintf(str, "Test %d: consecutive of 32k", test_num);
	ScreenPrint(0, line_num, str);
	is_good = TRUE;

	size = 1;
	chunk = malloc(size);
	while (chunk != NULL)
	{
		free(chunk);
		size++;
		chunk = malloc(size);
	}
	sprintf(str, "%d", size - 1);
	ScreenPrint(32, line_num, str);
	TestStatusPrint(is_good);

	return 0;
}
