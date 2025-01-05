// Including linux modules and init headers and subfolders
#include <linux/module.h>
#include <linux/init.h>
// Accessing filesystems
#include <linux/fs.h>

// For licensing purposes when this module is be used in the future
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alireza");
MODULE_DESCRIPTION("Registering a device number and implementing soem callback functions");


/* @brief This function is called when device file is opened */
static int driver_open(struct inode *device_file, struct file *instance){

	printk("dev_nr open func was called!\n");
	return 0;
}

/* @brief This function is called when device file is closed */
static int driver_close(struct inode *device_file, struct file *instance){

	printk("dev_nr close func was called!\n");
	return 0;	
}


static struct file_operations fops ={
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close
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
	/* Resgistering the device_nr */
	retval = register_chrdev(MYMAJOR, "my_dev_nr_driver", &fops);
	
	if (retval == 0) /* meaning the device number is free */
	{
		printk("dev_nr registered  Device number Major : %d, Minor, %d", MYMAJOR, 0);
	}
	else if (retval > 0){
		/**
		 * meaning the current major device number is being used
		 * 12 upper bits of the return value are major number
		 * 20 lower bits are the minor number
		 **/
		printk("dev_nr registered  Device number Major : %d, Minor, %d", retval>>20, retval&0xffffff);
	}
	else{
		printk("Could not register device number!\n");
		return -1;
	}

	return 0;
}

/* @brief  Called when module is unloaded */
static void __exit ModuleExit(void)
{
	/* freeing the register number */
	unregister_chrdev(MYMAJOR, "my_dev_nr_driver");
	printk("Goodbye Kernel - Dev nr!\n");
}


// Specifying the callback functions
module_init(ModuleInit);
module_exit(ModuleExit);

