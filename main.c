#include "main.h"
#include "server.h"

int main(int argc, char *argv[])
{
	int ret;
	int proc_total = 10; /* default multiprocess max value */

	if ( argc != 1 ) {
		proc_total = atoi(argv[1]);
		if ( proc_total == 0 ) {
			exit_err("please write vaild int value\n");
		}
	}

	//if ( (ret = fork()) < 0 ) {
	//	exit_err("fork error");
	//} else if ( ret == 0 ) { /* child */
		start_server();
	//} else { /* parent */
	//	sleep(1);
	//	//start_client();
	//}
	//create_multiprocess(proc_total);
	return 0;
}

void exit_err(const char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}
