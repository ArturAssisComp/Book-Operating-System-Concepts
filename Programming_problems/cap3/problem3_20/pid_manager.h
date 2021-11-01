#ifndef PID_MANAGER_H
#define PID_MANAGER_H

#include "pid_table.h"

//Macros:
#define MIN_PID 300
#define MAX_PID 5000

extern pid_table *global_pid_table;


//Function declarations:
int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);
void free_map(void);


#endif
