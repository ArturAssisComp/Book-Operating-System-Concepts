#ifndef SHM_CONF_H
#define SHM_CONF_H
#include <sys/time.h>

typedef struct
{
	struct timeval start_timestamp;
	int time_updated;
} shm_buffer;

#endif
