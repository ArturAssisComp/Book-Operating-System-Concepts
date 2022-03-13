/**
 * task_info.c
 *
 * Kernel module that returns information about the task with specified pid.
 * The informations about the task are: 
 * (1) - The command the task is running;
 * (2) - The pid of the task; 
 * (3) - The current state of the task;
 * 
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
//#include <linux/proc_fs.h>
//#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 128

#define PROC_NAME "pid"
static long current_pid;

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
        .write = proc_write
};


/* This function is called when the module is loaded. */
static int proc_init(void)
{

        // creates the /proc/pid entry
        // the following function call is a wrapper for
        // proc_create_data() passing NULL as the last argument
        proc_create(PROC_NAME, 0, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void) {

        // removes the /proc/jiffies entry
        remove_proc_entry(PROC_NAME, NULL);

        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/pid is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;
        task_struct target_task_struct;

        if (completed) {
                completed = 0;
                return 0;
        }

        completed = 1;


        target_task_struct = pid_task(find_vpid(current_pid), PIDTYPE_PID);
        if (!target_task_struct) 
        {
            rv = 0;
            goto end;
        }

        //Store target task struct info into the kernel buffer:
        if(sprintf(buffer, "PID: %d\nSTATE: %u\nCommand: %s\n", target_task_struct->pid, target_task_struct->__state, target_task_struct->comm) < 0)
        {
            rv = -1;
            goto end;
        }
        printk(KERN_INFO "%s\n", buffer);

        // copies the contents of buffer to userspace usr_buf
        if (copy_to_user(usr_buf, buffer, rv)) rv = -1;

end:
        return rv;
}

static ssize_t proc_write(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char *k_mem;
    const int base = 10;

    /*Allocate kernel memory*/
    k_mem = kmalloc(count, GFP_KERNEL);

    /*Copies user space usr_buf to kernel memory */
    copy_from_user(k_mem, usr_buf, count);

    printk(KERN_INFO "%s\n", k_mem);
    //kstrtol(k_mem, base, &current_pid);
    sscanf(k_mem, "%lu", &current_pid)



    /*Return kernel memory*/
    kfree(k_mem);

    return count;
}

/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TASK INFO");
MODULE_AUTHOR("SGG");
