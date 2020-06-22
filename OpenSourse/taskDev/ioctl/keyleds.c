/*
 * kbleds.c − Blink keyboard leds until the module is unloaded.(modified for > 4.15)
 */
#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h>       /* For vc_cons */
#include <linux/vt_kern.h>
#include <linux/timer.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/string.h>
 
#define BLINK_DELAY   HZ/5
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF
#define NAME_NODE "keyledctl"

struct timer_list my_timer;
struct tty_driver *my_driver;
static struct kobject *x_kobject;
static int _kbledstatus = 0;
static int keycod = 0;

static ssize_t get_led_status(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
        sprintf(buf, "%d\n", keycod);
        printk("READ: %ld\n", strlen(buf));

        return strlen(buf);
}

static ssize_t set_led_status(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
        printk("WRITE: %ld\n", count);
        sscanf(buf, "%du", &keycod);
    
        return count;
}

static struct kobj_attribute x_attribute =__ATTR(test, 0660, get_led_status, set_led_status);

static int sys_init(void){
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

static void my_timer_func(struct timer_list *ptr)
{
        int *pstatus = &_kbledstatus;

        if (*pstatus == keycod)
                *pstatus = RESTORE_LEDS;
        else
                *pstatus = keycod;
                
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, *pstatus);

        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
}

static int __init kbleds_init(void)
{
        int error = sys_init();
        int i;

        printk(KERN_INFO "KEYLEDS: fgconsole is %x\n", fg_console);
        for (i = 0; i < MAX_NR_CONSOLES; i++) {
                if (!vc_cons[i].d)
                        break;

                printk(KERN_INFO "POET_ATKM: console[%i/%i] #%i, tty %lx\n", i,
                       MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
                       (unsigned long)vc_cons[i].d->port.tty);
        }

        printk(KERN_INFO "KEYLEDS: finished scanning consoles\n");
        my_driver = vc_cons[fg_console].d->port.tty->driver;
        printk(KERN_INFO "KEYLEDS: tty driver magic %x\n", my_driver->magic);

        timer_setup(&my_timer, my_timer_func, 0);
        
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);

        return error;
}
static void __exit kbleds_cleanup(void)
{
        printk(KERN_INFO "KEYLEDS: removed\n");
        del_timer(&my_timer);
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);

        kobject_put(x_kobject);
}
module_init(kbleds_init);
module_exit(kbleds_cleanup);

MODULE_LICENSE("GPL");
