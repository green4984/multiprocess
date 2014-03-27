#include "server.h"

static int stdout_fileno;
static int create_socket()
{
	const int opt = 1;
	int fd;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		exit_err("socket error");
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {                                                                                                                      
		exit_err("setsockopt SQ_REUSEADDR error!");
	}

#ifdef DEBUG
	printf("create socket fd [%d] success!\n", fd);
#endif
	return fd;
}
static void set_sockaddr(struct sockaddr_in *srv)
{
	memset(srv, 0x00, sizeof(struct sockaddr_in));
	srv -> sin_family = AF_INET;
	srv -> sin_port = htons(LISTEN_PORT);
	srv -> sin_addr.s_addr = INADDR_ANY; /* use local ip address */
#ifdef DEBUG
	printf("set listen port [%d]!\n", LISTEN_PORT);
#endif
}
static void bind_sockaddr(int fd, struct sockaddr_in *srv)
{
	if ( -1 == bind(fd, (struct sockaddr *)srv, 
				sizeof(struct sockaddr_in)) ) {
		exit_err("bind error");
	}
#ifdef DEBUG
	printf("bind success!\n");
#endif
}
static void begin_listen(int fd, int backlog)
{
	if (-1 == listen(fd, backlog)) {
		exit_err("listen error");
	}
#ifdef DEBUG
	printf("start listening !\n");
#endif
}
static void set_output()
{
	int fd;
	stdout_fileno = dup(STDOUT_FILENO);
	if ( (fd = open("srv.log", O_CREAT | O_RDWR | O_APPEND, 0664)) == -1)
		exit_err("open srv.log error");
	close(STDOUT_FILENO);
	if ( dup2(fd, STDOUT_FILENO) == -1 )
		exit_err("dup2 STDOUT_FILENO error");
	setbuf(stdout, NULL);
	printf("set_output ok!");
}
static void back_output()
{
	if (dup2(stdout_fileno, STDOUT_FILENO) == -1) {
		exit_err("dup2 stdout_fileno error");
	}
}
static void do_sigint(int signo)
{
	//signal(SIGINT, SIG_DFL);
	close_multiprocess();
	printf("close server now!\n\n");
	exit(EXIT_SUCCESS);
}
static void do_childprocess(int fd)
{
	int newfd;
	while (1) {
		printf("pid %d wait for parent\n", getpid());
		if ((newfd = recv_fd(fd)) == -1)
			exit_err("read_fd");
		printf("pid %d get newfd %d from ppid %d\n", getpid(), newfd, getppid());
		sleep(1);
	}
}
void start_server(int srv_num)
{
	signal(SIGINT, do_sigint);
	//set_output();
	int newfd;
	struct proc_t *proc;
	int fd = create_socket();
	socklen_t len = sizeof(struct sockaddr);
	struct sockaddr_in srv;
	set_sockaddr(&srv);
	bind_sockaddr(fd, &srv);
	create_multiprocess(srv_num, do_childprocess);
	begin_listen(fd, 20);
	while (1) {
#ifdef DEBUG
		printf("accepting!\n");
#endif
		if ((newfd = accept(fd, (struct sockaddr *)&srv, &len)) == -1) {
			exit_err("accept error");
		}
		do {
			proc = get_sleep_process();
			if (proc == NULL) {
				printf("server is full, please waiting for\n");
				sleep(1);
			} else {
				break;
			}
		} while(1);
		proc -> flag = 1;
		printf("get msg from %s newfd is %d \nsend it to child\n", 
				inet_ntoa(srv.sin_addr), newfd);
		send_fd(proc -> fd, newfd);
		close(newfd);
	}
	back_output();
}
