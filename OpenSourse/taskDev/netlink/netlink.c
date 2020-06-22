#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/net_namespace.h>

#define NETLINK_UNIT 31

struct sock *nl_sock = NULL;

static void recv_msg(struct sk_buff *skb) {
    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skbuff_out;
    int msg_size;
    char *msg = "Hello User! It's message from kernel!";
    int res;

    msg_size = strlen(msg);

    nlh = (struct nlmsghdr *)skb->data;
    printk(KERN_INFO "RECIEVED MESSAGE: %s\n", (char *)nlmsg_data(nlh));
    pid = nlh->nlmsg_pid; /*pid of sending process */

    skbuff_out = nlmsg_new(msg_size, 0);

    if (!skbuff_out) {
        printk(KERN_ERR "Failed to allocate new skbuff\n");
        return;
    }

    nlh = nlmsg_put(skbuff_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skbuff_out).dst_group = 0; /* not in mcast group */
        strncpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_unicast(nl_sock, skbuff_out, pid);

    if (res < 0)
        printk(KERN_ERR "Error while sending bak to user\n");
}

struct netlink_kernel_cfg conf = {
   .groups  = 1,
   .input = recv_msg,
};

static int __init hello_init(void) {
    printk(KERN_INFO "INITIALIZING MODULE: Success\n");
    nl_sock = netlink_kernel_create(&init_net, NETLINK_UNIT, &conf);

    if (!nl_sock)
    {
        printk(KERN_ERR "Error creating socket.\n");
        return -10;
    }

    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "MODULE REMOVED: Success\n");
    netlink_kernel_release(nl_sock);
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");