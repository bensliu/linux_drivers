/*
 * sleepy.c -- the writers awake the readers
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 * $Id: sleepy.c,v 1.7 2004/09/26 07:02:43 gregkh Exp $
 */

#include <linux/module.h>
#include <linux/init.h>

#include <linux/sched.h>  /* current and everything */
#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/types.h>  /* size_t */
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioport.h>

#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/irq.h>

MODULE_LICENSE("Dual BSD/GPL");

#define NUM_CMOS_BANKS		2
#define CMOS_BANK_SIZE		(0xFF*8)
#define CMOS_DEVICE_NAME	"cmos"

#define CMOS_BANK0_INDEX_PORT	0x70
#define CMOS_BANK0_DATA_PORT	0x71
#define CMOS_BANK1_INDEX_PORT	0x72
#define CMOS_BANK1_DATA_PORT	0x73

unsigned char addrports[NUM_CMOS_BANKS] = {
		CMOS_BANK0_INDEX_PORT,
		CMOS_BANK1_INDEX_PORT
};

unsigned char dataports[NUM_CMOS_BANKS] = {
		CMOS_BANK0_DATA_PORT,
		CMOS_BANK1_DATA_PORT
};

static struct cmos_dev {
	struct cdev		my_cdev;
	char 			name[10];
	int				bank_number;
	unsigned int	size;

	unsigned short	current_pointer;

} *cmos_devp[NUM_CMOS_BANKS];

static dev_t cmos_dev_num;
static struct class* cmos_class;

static ssize_t cmos_read (struct file *filp, char __user *user_buf, size_t count, loff_t *pos)
{
	char c = 'a';

	printk(KERN_INFO "read data end\n");

	if (copy_to_user(user_buf, &c, 1))
		return -EFAULT;

	*pos += count;
	return 0; /* EOF */
}

static ssize_t cmos_write (struct file *filp, const char __user *user_buf, size_t count,
		loff_t *pos)
{
	char c = 'a';

	printk(KERN_INFO "write data\n");

	if (copy_from_user(&c, user_buf, 1))
		return -EFAULT;

	*pos += count;
	return count; /* succeed, to avoid retrial */
}

static loff_t cmos_llseek (struct file *filp, loff_t pos, int count)
{

	return 0;
}

static long cmos_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	return 0;
}

static int cmos_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Driver: open()\n");
	return 0;
}

static int cmos_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Driver: close()\n");
	return 0;
}

static struct file_operations cmos_fops = {
	.owner 		= THIS_MODULE,
	.open  		= cmos_open,
	.release 	= cmos_close,
	.read  		= cmos_read,
	.write 		= cmos_write,
	.llseek		= cmos_llseek,
	.unlocked_ioctl		= cmos_ioctl,
};

int __init cmos_init(void)
{
    int ret, i;

    printk(KERN_INFO "cmos_init\n");

	/*
	 * Register your major, and accept a dynamic number
	 */
	if (alloc_chrdev_region(&cmos_dev_num, 0, NUM_CMOS_BANKS, CMOS_DEVICE_NAME) < 0)
	{
	    printk(KERN_INFO "cannot create cmos driver\n");
		return -1;
	}

	cmos_class = class_create(THIS_MODULE, CMOS_DEVICE_NAME);
	if (!cmos_class)
	{
	    printk(KERN_INFO "cannot create cmos class\n");
		unregister_chrdev_region(cmos_dev_num, NUM_CMOS_BANKS);
		return -1;
	}

	for (i=0; i<NUM_CMOS_BANKS; i++)
	{
		cmos_devp[i] = kmalloc(sizeof(struct cmos_dev), GFP_KERNEL);
		if (!cmos_devp[i])
		{
		    printk(KERN_INFO "cannot alloc memory for cmos driver\n");
			return -1;
		}

		// request IO region
		sprintf(cmos_devp[i]->name, "cmos%d", i);
		if (!request_region(addrports[i], 2, cmos_devp[i]->name))
		{
		    printk(KERN_INFO "cannot request_region\n");
			goto fail1;
		}

		cmos_devp[i]->bank_number = i;

		cdev_init(&cmos_devp[i]->my_cdev, &cmos_fops);
		cmos_devp[i]->my_cdev.owner = THIS_MODULE;

		ret = cdev_add(&cmos_devp[i]->my_cdev, (cmos_dev_num+i), 1);
		if (ret)
		{
		    printk(KERN_INFO "cannot cdev_add\n");
			return -1;
		}

		device_create(cmos_class, NULL, MKDEV(MAJOR(cmos_dev_num), i), NULL, "cmos%d", i);
	}

    printk(KERN_INFO "cmos driver initialized: %d\n", MAJOR(cmos_dev_num));

    return(ret);

fail1:
	unregister_chrdev_region(cmos_dev_num, NUM_CMOS_BANKS);
	class_destroy(cmos_class);
	return -1;
}

void __exit cmos_cleanup(void)
{
	int i;

    printk(KERN_INFO "cmos_cleanup\n");

	unregister_chrdev_region(cmos_dev_num, NUM_CMOS_BANKS);

	for (i=0; i<NUM_CMOS_BANKS; i++)
	{
		device_destroy(cmos_class, MKDEV(MAJOR(cmos_dev_num), i));
		release_region(addrports[i], 2);
		cdev_del(&cmos_devp[i]->my_cdev);
		kfree(cmos_devp[i]);
	}

	class_destroy(cmos_class);
}

module_init(cmos_init);
module_exit(cmos_cleanup);

