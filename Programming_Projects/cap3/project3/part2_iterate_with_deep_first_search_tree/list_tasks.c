/**
 * list_tasks.c
 *
 * This kernel module iterates through every task in the system and returns 
 * information about each task.
 * In this implementation, deep first search is used.
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


void print_deep_first_search(struct task_struct *current_task)
{
    static int i = 0;
    struct list_head *list;
    struct task_struct *task;
    printk(KERN_INFO "(%3d) - %-9d%-30s%ld\n", ++i, current_task->pid, current_task->comm, current_task->state);
    /* Loop through the children of current_task */
    list_for_each(list, &(current_task->children))
    {
        task = list_entry(list, struct task_struct, sibling);
        print_deep_first_search(task);
    }
}



/* This function is called when the module is loaded. */
static int mod_init(void)
{
    printk(KERN_INFO "%s\n", MOD_NAME);
    printk(KERN_INFO "List of tasks:\n");
    printk(KERN_INFO "        PID      Command                       State\n");
    print_deep_first_search(&init_task);
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



