#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/uaccess.h>  // Needed for copy_to_user()

#define PROC_NAME "seconds"

static struct proc_dir_entry *proc_file;
static unsigned long start_jiffies;

static ssize_t proc_read_seconds(struct file *file, char __user *buffer, size_t count, loff_t *offset) {
    unsigned long elapsed_seconds = (jiffies - start_jiffies) / HZ;
    char temp[20];
    int len;

    len = snprintf(temp, sizeof(temp), "%lu\n", elapsed_seconds);
    if (*offset >= len)
        return 0;

    if (copy_to_user(buffer, temp, len))
        return -EFAULT;

    *offset += len;
    return len;
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read_seconds,
};

static int __init proc_init(void) {
    start_jiffies = jiffies;
    proc_file = proc_create(PROC_NAME, 0444, NULL, &proc_fops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    if (!proc_file) {
        return -ENOMEM;
    }
    return 0;
}

static void __exit proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);

}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
