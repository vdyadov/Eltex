#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define SIZE_BUFF 128
#define NAME_NODE "mymod"

int len, temp, res;
 
char *msg = NULL;
 
static ssize_t read_proc(struct file * filp, char * buf, size_t count, loff_t * ppos) {
	if(count > temp)
		count = temp;

	temp = temp - count;

    printk(KERN_INFO "READ: %ld\n", count);

	if ((res = copy_to_user(buf, msg, count)) < 0)
        printk(KERN_INFO "MESSAGE NOT READ!\n");

    printk(KERN_INFO "RETURN BYTES: %d\n", res);
    
	if(count == 0)
		temp = len;
 
	return count;
}
 
static ssize_t write_proc(struct file * filp, const char * buf, size_t count, loff_t * ppos) {
    printk(KERN_INFO "WRITE: %ld\n", count);
	copy_from_user(msg, buf, count);

	len = count;
	temp = len;
	return count;
}

struct file_operations proc_fops = {
	read:
        read_proc,
	write: 
        write_proc
};

static void create_new_proc_entry(void) {
	proc_create(NAME_NODE, 0, NULL, &proc_fops);
	msg = kmalloc(SIZE_BUFF * sizeof(char), GFP_KERNEL);
}



static int __init proc_module_init(void) {
	create_new_proc_entry();
	printk(KERN_INFO "CREATE MODULE: Succsess!\n");
	return 0;
}

static void __exit proc_module_exit(void) {
	remove_proc_entry(NAME_NODE, NULL);
    	kfree(msg);
	printk(KERN_INFO "/proc/%s REMOVED: Success!\n", NAME_NODE);
}

module_init(proc_module_init);
module_exit(proc_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dyadov Vladislav");