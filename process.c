#include "process.h"
int create_multiprocess(int proc_num, void (*func)(void))
{
	int i, rst;
	//int *proc_arr = calloc( proc_num, sizeof(int));
	//signal(SIGCHLD, SIG_IGN);
	for ( i = 0; i < proc_num; i++ ) {
		if ( ( rst = fork() ) < 0 ) {
			exit_err("fork error");
		} else if ( rst == 0 ) { /* child */
			func();
			return 0;
		} else {
			printf("child pid %d has been created!\n", rst);
			//proc_arr[i] = rst;
		}
	}
	//free(proc_arr);
	//proc_arr = NULL;
	for ( i = 0; i < proc_num; i++ ) {
		rst = wait(NULL);
		if ( rst == -1 ) { /* error occur */
			exit_err("wait return -1");
		} else { /* return the child process ID */
			printf("child pid %d has stop!\n", rst);
		}
	}
	return 0;
}
