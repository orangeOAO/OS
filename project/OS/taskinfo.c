#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>

#define PROC_NAME "taskinfo"
#define BUFFER_SIZE 128

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

static int current_pid = 0;

static int __init taskinfo_init(void) {
    // 创建/proc/taskinfo
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit taskinfo_exit(void) {
    // 移除/proc/taskinfo
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(taskinfo_init);
module_exit(taskinfo_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module for displaying a task's information using /proc file system.");
MODULE_AUTHOR("Your Name");

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    char buffer[BUFFER_SIZE];
    int len;
    static int completed = 0;
    struct task_struct *task;
    char task_state ;

    
    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    task = pid_task(find_get_pid(current_pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_INFO "Task with PID %d not found.\n", current_pid);
        return -ESRCH;
    }
    task_state = task_state_to_char(task);
    len = snprintf(buffer, BUFFER_SIZE, "Command: %s\nPID: %d\nState: %c\n",
                   task->comm, current_pid, task_state);
    
    if (copy_to_user(usr_buf, buffer, len)) {
        return -EFAULT;
    }

    return len;
}


static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos) {
    char k_buf[BUFFER_SIZE];

    if (count > BUFFER_SIZE) count = BUFFER_SIZE;
    if (copy_from_user(k_buf, usr_buf, count)) return -EFAULT;
    
    sscanf(k_buf, "%d", &current_pid);
    
    return count;
}
