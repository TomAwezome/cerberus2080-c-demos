#pragma output CRT_ORG_CODE = 0x0202
#pragma output CRT_ENABLE_NMI = 1
#pragma output CRT_ENABLE_EIDI = 0x02
#pragma output CLIB_MALLOC_HEAP_SIZE = 0xFFF

#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include "types.h"

U8 *video_ram = 0xF800;
U16 video_ram_size = 0xFCAF - 0xF800;

int main()
{
	U8 *t = malloc(video_ram_size + 1);
	U8 *d;
	U16 i;

	while (TRUE)
	{
		for (i = 0; i <= video_ram_size; i++)
			t[i] = rand();

		memcpy(video_ram, t, video_ram_size + 1);

		d = malloc(video_ram_size + 1);
		free(t);
		t = d;
	}

	return 0;
}
