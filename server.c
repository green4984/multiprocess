#include "server.h"

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
void start_server()
{
	int newfd;
	int fd = create_socket();
	socklen_t len = sizeof(struct sockaddr);
	struct sockaddr_in srv;
	set_sockaddr(&srv);
	bind_sockaddr(fd, &srv);
	begin_listen(fd, 20);
	while (1) {
#ifdef DEBUG
		printf("accepting!\n");
#endif
		if ((newfd = accept(fd, (struct sockaddr *)&srv, &len)) == -1) {
			exit_err("accept error");
		}
		printf("get msg from %s\n", inet_ntoa(srv.sin_addr));
		close(newfd);
	}
}
