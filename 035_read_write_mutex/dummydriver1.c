/* Goal:
 * Writing Read and Wrtie callback functioncs to enable this functionalityfrom user space
 */


// Including linux modules and init headers and subfolders
#include <linux/module.h>
#include <linux/init.h>
// Accessing filesystems
#include <linux/fs.h>
// Why adding this?
#include <linux/cdev.h>
#include <linux/uaccess.h>
// Using mutex to synchronize access to this buffer
#include <linux/mutex.h>
#include <linux/delay.h>

// For licensing purposes when this module is be used in the future
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alireza");
MODULE_DESCRIPTION("Registering a device number and implementing soem callback functions - dummydriver 1");

/*Read and Write buffer data */
char buffer[255]; /* Letting other modules in the kernel to access this*/
EXPORT_SYMBOL(buffer);
static size_t buffer_pointer = 0;

/* Varialbes for device and device class */
static dev_t my_device_nr; /*containt device number */
static struct class *my_class; /*device class that we need */
static struct cdev my_device; /*contains all information about the device we are going to create */

#define DRIVER_NAME "dummydriver1"
#define DRIVER_CLASS "DummyDrvierClass1"


static DEFINE_MUTEX(buffer_mutex);

/*
 * @brief: Callback func to read data from buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
	int to_copy, not_copied, delta;

	/* Get amount of data to copy
	 * e.g. if we have written 5 bytes to the buffer (buffer pointer at 4) and we 
	 * want to read 3 bytes (count = 3), this way we specify the amounts of bytes
	 * to be copied

*/
	mutex_lock(&buffer_mutex);
	printk("Mutex Lock -------------------------- \n");
	to_copy = min(count, buffer_pointer);

	printk("Reading %d bytes with the drvier\n",to_copy);
	/* Copy data to user
	 * memcpy does not exist here
	 * copy_to_user returns the bytes that has not be copied
	 * it needs:
	 * destination, source and bytes to copy
	 */
	not_copied = copy_to_user (user_buffer, buffer, to_copy);
	msleep(500);
	printk("Mutex Unlocked ------------------ \n");
	mutex_unlock(&buffer_mutex);
	/* Calculate data */
	delta = to_copy - not_copied;
	
	return delta;
}

/*
 * @brief: Callback func for writing functionality
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs){
	int to_copy, not_copied, delta;

	printk("Writing with the driver\n");
	/* Get amount of data to copy */
	to_copy = min(count, sizeof(buffer));

	/* Copy data from user
	 * memcpy does not exist here
	 * copy_to_user returns the bytes that has not be copied
	 * it needs:
	 * destination, source and amount of bytes to copy
	 */
	not_copied = copy_from_user(buffer, user_buffer, to_copy);
	buffer_pointer = to_copy;

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
	printk("Hello Kernel - DummyDrvier1 repoting!\n");
	
	
	/*Auto-Allocating device numbe - step 1 */
	if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME)  < 0){
		printk("Device number could not be allocated!\n");
		return -1;
	}
	else{
		printk("Read wrtie - device number major %d, minor %d was registersted\n", my_device_nr >> 20 , my_device_nr && 0xffffff);
	}

	/* Step 2: Creating device class */
	if ((my_class = class_create(DRIVER_CLASS)) == NULL ){
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

	return 0;

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
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye Kernel - DummyDriver1 over!\n");
}


// Specifying the callback functions
module_init(ModuleInit);
module_exit(ModuleExit);

