/* Goal:
 * Writing GPIO Driver
 */


// Including linux modules and init headers and subfolders
#include <linux/module.h>
#include <linux/init.h>
// Accessing filesystems
#include <linux/fs.h>
// Why adding this?
#include <linux/cdev.h>
#include <linux/uaccess.h>
// To acces gpio
#include <linux/gpio.h>

// For licensing purposes when this module is be used in the future
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alireza");
MODULE_DESCRIPTION("A simple GPIO Driver to toggle a pin");


/* Varialbes for device and device class */
static dev_t my_device_nr; /*containt device number */
static struct class *my_class; /*device class that we need */
static struct cdev my_device; /*contains all information about the device we are going to create */

#define DRIVER_NAME "my_gpio_driver"
#define DRIVER_CLASS "MyModuleClass"



/*
 * @brief: Callback func to read data from buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
	int to_copy, not_copied, delta;
	
	char tmp[3] = "\n";
	/* Get amount of data to copy
	 * e.g. if we have written 5 bytes to the buffer (buffer pointer at 4) and we 
	 * want to read 3 bytes (count = 3), this way we specify the amounts of bytes
	 * to be copie
*/
	
	to_copy = min(count, sizeof(tmp));

	/* Reading the value of the input */
	tmp[0] = gpio_get_value(17) + '0';
	printk("Value of the button is %d\n", gpio_get_value(17));
	

	not_copied = copy_to_user (user_buffer, &tmp, to_copy);

	/* Calculate data */
	delta = to_copy - not_copied;

	return delta;
}

/*
 * @brief: Callback func for writing functionality
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs){
	int to_copy, not_copied, delta;
		
	char value;

	printk("Writing with the driver\n");
	/* Get amount of data to copy */
	to_copy = min(count, sizeof(value));

	
	not_copied = copy_from_user(&value, user_buffer, to_copy);

	switch(value)
	{
		case '0':
			gpio_set_value(4,0);
			break;
		case '1':
			gpio_set_value(4,1);
			break;
		default:
			printk("Input invalid\n");
			break;
	}

	/* Calculate data */
	delta = to_copy - not_copied;

	return delta;

}

	
/* @brief This function is called when device file is opened */
static int driver_open(struct inode *device_file, struct file *instance){

	printk("Driver is opened!\n");
	return 0;
}

/* @brief This function is called when device file is closed */
static int driver_close(struct inode *device_file, struct file *instance){

	printk("Driver is closed!\n");
	return 0;	
}


static struct file_operations fops ={
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};

//--------------------------------------------------------------------------
/* Looking at cat /proc/devices, you can see a list of devices registered with their major numbers
*MYMAJOR number is selected based on a unoccupied number in the list. */
#define MYMAJOR 90

/* @brief  Called when module is loaded */
static int __init ModuleInit(void)
{
	int retval;
	printk("Hello Kernel - Dev nr!\n");
	
	
	/*Auto-Allocating device numbe - step 1 */
	if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME)  < 0){
		printk("Device number could not be allocated!\n");
		return -1;
	}
	else{
		printk("Read wrtie - device number major %d, minor %d was registersted\n", my_device_nr >> 20 , my_device_nr && 0xffffff);
	}

	/* Step 2: Creating device class */
	if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL ){
		printk("Device class cannot be created\n");
		goto ClassError;
	}

	/* create device file */
	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL){
		printk("Cannot create device file\n");
		goto FileError;
	}
	
	/* Step 3: Initialize device file */
	cdev_init(&my_device, &fops);
	/* Step 4: Registering device to kernel*/
	if (cdev_add(&my_device, my_device_nr, 1) == -1){
		printk("Registering of device to kernel failed\n");
		goto AddError;
	}
	
	/* GPIO 4 init */
	if (gpio_request(4, "rpi-gpio-4")){
		printk("Can not allocate hpio 4\n");
		goto AddError;
	}
	/* Set GPIO 4 directions*/
	if (gpio_direction_output(4, 0)){
		printk("Can not set GPIO to output mode\n");
		goto Gpio4Error;
	}

	/* GPIO 17 init */
	if (gpio_request(17, "rpi-gpio-17")){
		printk("Can no allocate gpio 17\n");
		goto Gpio4Error;
	}
	/* Set GPIO 17 mode to input*/
	if (gpio_direction_input(17)){
		printk("Can not set GPIO to output mode\n");
		goto Gpio17Error;
	}
	return 0;


Gpio4Error:
	gpio_free(4);
Gpio17Error:
	gpio_free(17);
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;

}

/* @brief  Called when module is unloaded */
static void __exit ModuleExit(void)
{
	gpio_set_value(4, 0);
	gpio_free(17);
	gpio_free(4);
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye Kernel - Dev nr!\n");
}


// Specifying the callback functions
module_init(ModuleInit);
module_exit(ModuleExit);

