#include <stdio.h>
#include "scicos_usb.h"

static char to_stop = 0;
unsigned int j = 0;

int main(int argc, char **argv)
{
	int retv = 0;

	printf("\n");
	printf("*************************************************\n");
	printf("            Test flex_pc_usb library             \n");
	printf("*************************************************\n\n");
	
	printf(" - flex_pc_usb_init() : ");
	retv = flex_pc_usb_init();
	if (retv != 0) {
		printf("ERROR, returned value is %d.\n", retv);
		return 0;
	} else {
		printf("OK.\n");
		flex_pc_usb_close();
		return 1;
	}
	
	
	printf("Run.\n");
	
	/*while(!to_stop)	{
		unsigned char buf1[128],buf2[128],buf3[128],buf4[128],buf5[128],buf6[128],buf7[128],buf8[128],buf9[128],buf10[128];
		int bytes_read,bytes_read2,i;
		bytes_read=flex_pc_usb_read(buf1,128,0,0);
		bytes_read=flex_pc_usb_read(buf2,128,1,0);
		bytes_read=flex_pc_usb_read(buf3,128,2,0);
		bytes_read=flex_pc_usb_read(buf4,128,3,0);
		bytes_read=flex_pc_usb_read(buf5,128,4,0);
		bytes_read=flex_pc_usb_read(buf6,128,5,0);
		bytes_read=flex_pc_usb_read(buf7,128,6,0);
		bytes_read=flex_pc_usb_read(buf8,128,7,0);
		bytes_read=flex_pc_usb_read(buf9,128,8,0);
		bytes_read=flex_pc_usb_read(buf10,128,9,0);
		//bytes_read2=flex_pc_usb_read(buf2,128,1,0);
		//dump of bytes read
		//for(i=0; i<bytes_read; i++)
		//printf("%2x ",buf[i]);
		printf("%d %f %f %f %f %f %f %f %f %f %f",ind++,*(float *)&buf1[2],
                      *(float *)&buf2[2],*(float *)&buf3[2],
                      *(float *)&buf4[2],*(float *)&buf5[2],
                      *(float *)&buf6[2],*(float *)&buf7[2],
                      *(float *)&buf8[2],*(float *)&buf9[2],
                      *(float *)&buf10[2]);
		putchar('\n');
    
		//usleep(1000);
		fflush(stdout);
		//j++;
		//if(j==1000) {
		//	to_stop = 1;
		//	flex_pc_usb_close();
		//	return 0;
		//}
	}*/
	
	flex_pc_usb_close();
	
	return 0;
}
