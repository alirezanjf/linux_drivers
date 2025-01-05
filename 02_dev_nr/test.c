#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


/* Just a c code to open and close the device */
/* used:
* sudo mknod /dev/mydevice c 90 0
* to make a device file, which further, are used in the user-space to access hardware or kernel modules.
* the "c" here specifies that the device file to be created at theat address, which is a character device and major number is 90 and minor number is 0 (which specifies how many devices are being controlled by this driver)
*/

/*
* compile by gcc test.c -o test
* run by ./test
*/
int main(){
	int dev = open("/dev/mydevice", O_RDONLY);
	if (dev == -1){
		printf("Openning was no possible\n");
		return -1;
	}
	printf("Openning succesfull\n");
	close(dev);
	// No need to return? return 0; for instance?
}
	
