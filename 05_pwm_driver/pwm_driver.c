/* Goal:
 * Writing PWM Driver
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
#include <linux/pwm.h>

// For licensing purposes when this module is be used in the future
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alireza");
MODULE_DESCRIPTION("A simple driver to access PWM hardware");


/* Varialbes for device and device class */
static dev_t my_device_nr; /*containt device number */
static struct class *my_class; /*device class that we need */
static struct cdev my_device; /*contains all information about the device we are going to create */

#define DRIVER_NAME "my_pwm_driver"
#define DRIVER_CLASS "MyModuleClass"

/* Variables */
struct pwm_device *pwm0 = NULL;
u32 pwm_on_time = 500000000; //500 ms

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

	/* Setting PWM on-time value */
	if (value < 'a' || value > 'j')
		printk("Invalid value\n");
	else
		pwm_config(pwm0, 1000000000 * (value-'a'), 1000000000);

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
	.write = driver_write
};

//--------------------------------------------------------------------------
/* Looking at cat /proc/devices, you can see a list of devices registered with their major numbers
*MYMAJOR number is selected based on a unoccupied number in the list. */
#define MYMAJOR 90

/* @brief  Called when module is loaded */
static int __init ModuleInit(void)
{

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
	/* Requesting for PWM peripheral */
	pwm0 =  pwm_request(0, "my-pwm"); // gpio 12 connected to pwm0
	if (pwm0 == NULL){
		printk("Could not get PWM0!\n");
		goto AddError;
	}
	if (pwm_config(pwm0, pwm_on_time, 1000000000)) {
		printk("Failed to configure PWM0!\n");
		goto AddError;
	}
	//pwm_config(pwm0, pwm_on_time, 1000000000); //setting pwm ontime and freq (in nano sec)
	pwm_enable(pwm0);


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
	
	pwm_disable(pwm0);
	pwm_free(pwm0);
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye Kernel - Dev nr!\n");
}


// Specifying the callback functions
module_init(ModuleInit);
module_exit(ModuleExit);

