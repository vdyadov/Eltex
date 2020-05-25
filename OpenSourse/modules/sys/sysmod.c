#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/string.h>

#define NAME_NODE "mysys"
#define MAX_LEN 128

static struct kobject *x_kobject;
static char buf_msg[MAX_LEN + 1] = "Have a message\n";

static ssize_t x_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    strcpy(buf, buf_msg);
    printk("READ: %ld\n", strlen(buf));

    return strlen(buf);
}

static ssize_t x_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    printk("WRITE: %ld\n", count);
    strncpy(buf_msg, buf, count);
    buf_msg[count] = '\0';
    return count;
}

static struct kobj_attribute x_attribute =__ATTR(test, 0660, x_show, x_store);

static int __init sys_init(void){
    int error = 0;
    
    printk("MODULE INITIALIZATION: Success\n");

    x_kobject = kobject_create_and_add(NAME_NODE, kernel_kobj);
    if (!x_kobject)
        return -ENOMEM;
    
    error = sysfs_create_file(x_kobject, &x_attribute.attr);
    if (error)
        printk("failed create the file /sys/kernel/%s\n", NAME_NODE);
    
    return error;
}

static void __exit sys_exit(void){
    kobject_put(x_kobject);
    printk("MODULE UNINITIALIZATION: Success\n");
}

module_init (sys_init);
module_exit (sys_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dyadov Vladislav");
