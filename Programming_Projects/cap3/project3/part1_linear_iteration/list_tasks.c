/**
 * list_tasks.c
 *
 * This kernel module iterates through every task in the system and returns 
 * information about each task.
 * The informations about the task are: 
 * (1) - The command the task is running;
 * (2) - The pid of the task; 
 * (3) - The current state of the task;
 * 
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/sched.h>
#include <linux/module.h>

#define MOD_NAME "List of Tasks - Modules"





/* This function is called when the module is loaded. */
static int mod_init(void)
{
    int i = 0;
    struct task_struct *task;

    printk(KERN_INFO "%s\n", MOD_NAME);
    printk(KERN_INFO "List of tasks:\n");
    printk(KERN_INFO "        PID      Command                       State\n");
    for_each_process(task)
    {
        printk(KERN_INFO "(%3d) - %-9d%-30s%ld\n", ++i, task->pid, task->comm, task->state);

    }
	return 0;
}

/* This function is called when the module is removed. */
static void mod_exit(void) 
{
    printk( KERN_INFO "%s removed\n", MOD_NAME);
}


/* Macros for registering module entry and exit points. */
module_init( mod_init );
module_exit( mod_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LIST TASKS");
MODULE_AUTHOR("SGG");
