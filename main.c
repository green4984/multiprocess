#include "main.h"
#include "server.h"

int main(int argc, char *argv[])
{
	int proc_total = 10; /* default multiprocess max value */

	if ( argc != 1 ) {
		proc_total = atoi(argv[1]);
		if ( proc_total == 0 ) {
			exit_err("please write vaild int value\n");
		}
	}

	start_server(proc_total);
	return 0;
}

void exit_err(const char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}
