#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/kern_levels.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mydev"
#define DEVICE_FIRST 0
#define DEVICE_COUNT 3
#define BUF_LEN 128

static int major = 0;
module_param(major, int, S_IRUGO);

static int device_open = 0;
static int temp = 0;
char msg[BUF_LEN];
static struct cdev hcdev;

static ssize_t dev_read(struct file *file, char *buf, size_t count, loff_t *ppos){
    int len = strlen(msg);

    if( count < len ) 
        return -EINVAL; 

    if( *ppos != 0 )
        return 0;

    if(copy_to_user(buf, msg, len)) 
        return -EINVAL; 

    *ppos = len; 

    printk( KERN_INFO "READ: %d\n", len ); 
    return len; 
}

static ssize_t dev_write(struct file *file, const char *buf, size_t count, loff_t *ppos){
    copy_from_user(msg, buf, count);
    
    count = count;
    temp = count;

    printk(KERN_INFO "WRITE: %ld\n", count);

    return count;
}

static int dev_open(struct inode *n, struct file *f){
    if (device_open)
        return -EBUSY;
    
    device_open++; 
    return 0;  
}

static int dev_release(struct inode *n, struct file *f){
    device_open--;
    return 0;
}

static struct file_operations dev_fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .write = dev_write,
    .open = dev_open,
    .release = dev_release
};

static int __init dev_init(void){
    int ret;
    dev_t dev;
    if (major != 0){
        dev = MKDEV(major, DEVICE_FIRST);
        ret = register_chrdev_region(dev, DEVICE_FIRST, DEVICE_NAME);
    }else{
        ret = alloc_chrdev_region(&dev, DEVICE_FIRST, DEVICE_COUNT, DEVICE_NAME);
        major = MAJOR(dev);
    }
    if (ret < 0){
        printk(KERN_ERR "!!CAN NOT REGISTER CHAR DEVICE REGION!!\n");
        return ret;
    }
    cdev_init(&hcdev, &dev_fops);
    hcdev.owner = THIS_MODULE;
    ret = cdev_add(&hcdev, dev, DEVICE_COUNT);
    if (ret < 0){
        unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
        printk(KERN_ERR "!!CAN NOT ADD CHAR DEVICE!!\n");
        return ret;
    }
    printk(KERN_INFO "============= MODULE INSTALLED MAJOR:%d MINOR:%d =============\n", MAJOR(dev), MINOR(dev));

    return ret;
}

static void __exit dev_exit(void){
    cdev_del(&hcdev); 
    unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT); 
    printk(KERN_INFO "=============== MODULE REMOVED ===============\n"); 
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dyadov Vladislav");
