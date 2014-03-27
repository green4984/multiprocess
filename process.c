#include "process.h"

static int srv_num = 0;
static struct proc_t *proc  = NULL;
static void init_process(int i, int fd)
{
	(proc + i) -> fd = fd;
}
int create_multiprocess(int proc_num, void (*func)(int fd))
{
	int i, rst;
	int fd[2];
	srv_num = proc_num;
	proc = calloc(proc_num, sizeof(struct proc_t));
	for ( i = 0; i < proc_num; i++ ) {
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1) {
			exit_err("socketpair");
		}
		if ( ( rst = fork() ) < 0 ) {
			exit_err("fork error");
		} else if ( rst == 0 ) { /* child */
			close(fd[1]);
			func(fd[0]);
			return 0;
		} else {
			close(fd[0]);
			init_process(i, fd[1]);
			printf("child pid %d has been created!\n", rst);
		}
	}
	return 0;
}
void close_multiprocess()
{
	int i, rst;
	for ( i = 0; i < srv_num; i++ ) {
		rst = wait(NULL);
		if ( rst == -1 ) { /* error occur */
			exit_err("wait return -1");
		} else { /* return the child process ID */
			printf("child pid %d has stop!\n", rst);
		}
	}
}
int recv_fd(int sock_fd)
{
	int ret;
	struct msghdr msg;
	char recvchar;
	struct iovec vec;
	int recv_fd;
	char cmsgbuf[CMSG_SPACE(sizeof(recv_fd))];
	struct cmsghdr *p_cmsg;
	int *p_fd;
	vec.iov_base = &recvchar;
	vec.iov_len = sizeof(recvchar);
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &vec;
	msg.msg_iovlen = 1;
	msg.msg_control = cmsgbuf;
	msg.msg_controllen = sizeof(cmsgbuf);
	msg.msg_flags = 0;

	p_fd = (int *)CMSG_DATA(CMSG_FIRSTHDR(&msg));
	*p_fd = -1;
	ret = recvmsg(sock_fd, &msg, 0);
	if (ret != 1) {
		exit_err("recvmsg");
	}

	p_cmsg = CMSG_FIRSTHDR(&msg);
	if (p_cmsg == NULL) {
		exit_err("no passed fd");
	}


	p_fd = (int *)CMSG_DATA(p_cmsg);
	recv_fd = *p_fd;
	if (recv_fd == -1) {
		exit_err("no passed fd");
	}

	return recv_fd;
}
void send_fd(int sock_fd, int send_fd)
{
	int ret;
	struct msghdr msg;
	struct cmsghdr *p_cmsg;
	struct iovec vec;
	char cmsgbuf[CMSG_SPACE(sizeof(send_fd))];
	int *p_fds;
	char sendchar = 0;
	msg.msg_control = cmsgbuf;
	msg.msg_controllen = sizeof(cmsgbuf);
	p_cmsg = CMSG_FIRSTHDR(&msg);
	p_cmsg->cmsg_level = SOL_SOCKET;
	p_cmsg->cmsg_type = SCM_RIGHTS;
	p_cmsg->cmsg_len = CMSG_LEN(sizeof(send_fd));
	p_fds = (int *)CMSG_DATA(p_cmsg);
	*p_fds = send_fd; // 通过传递辅助数据的方式传递文件描述符

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &vec;
	msg.msg_iovlen = 1; //主要目的不是传递数据，故只传1个字符
	msg.msg_flags = 0;

	vec.iov_base = &sendchar;
	vec.iov_len = sizeof(sendchar);
	ret = sendmsg(sock_fd, &msg, 0);
	if (ret != 1)
		exit_err("sendmsg");
}
struct proc_t *get_sleep_process()
{
	int i;
	for (i = 0; i < srv_num; i++) {
		if ((proc + i) -> flag == 0) {
			return proc + i;
		}
	}
	return NULL;
}
