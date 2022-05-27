#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02
#pragma output CLIB_MALLOC_HEAP_SIZE = 0xFFF

#include "cerberus.h"
#include <strings.h>

const U8 test_string[] = "Test print string to video ram. text starts at a given x,y and wraps due to simple positioning logic";
U16 cursor_x = 4;
U16 cursor_y = 9;

U0 ScreenPrint(U16 x, U16 y, U8 *str)
{ // Write bytes of str to video ram at position x,y.
	U16 i;
	U16 l = strlen(str);

	for (i = 0; i < l; i++)
		video_ram[40 * y + x++] = str[i];
}

int main()
{
	ScreenPrint(cursor_x, cursor_y, test_string);

	return 0;
}

