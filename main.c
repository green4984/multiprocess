#include "main.h"

int main(int argc, char *argv[])
{
	int proc_total = 10; /* default multiprocess max value */

	if ( argc != 1 ) {
		proc_total = atoi(argv[1]);
		if ( proc_total == 0 ) {
			perror("please write vaild int value");
			exit(EXIT_FAILURE);
		}
	}

	create_multiprocess(proc_total);
}
