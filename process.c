#include "process.h"
int create_multiprocess(int proc_num)
{
	int i, rst;
	int *proc_arr = calloc( proc_num, sizeof(int));
	for ( i = 0; i < proc_num; i++ ) {
		if ( ( rst = fork() ) < 0 ) {
			perror("fork error");
			exit(EXIT_FAILURE);
		} else if ( rst == 0 ) { /* child */
			do_execute_wait();
		} else {
			printf("child pid %d has been created!\n", rst);
			proc_arr[i] = rst;
		}
	}
	free(proc_arr);
	proc_arr = NULL;
	wait(NULL);
}
void do_execute_wait()
{
	//sleep(3);
	pause();
}
