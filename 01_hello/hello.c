// Including linux modules and init headers and subfolders
#include <linux/module.h>
#include <linux/init.h>


// There are two events for modules (load and unload) -> we need to implement callback functions

int my_init(void)
{
	printk("Hello Kernel!\n");
	// If load was succesful -> return 0
	return 0;
}

void my_exit(void)
{
	printk("Goodbye Kernel!\n");
}


// Specifying the callback functions
module_init(my_init);
module_exit(my_exit);

// For licensing purposes when this module will be used in the future
MODULE_LICENSE("GPL");

