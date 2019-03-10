#include "tcp_server.h"

void* tcp_routine(void* arg) {
	int sock = *((int*)arg);
	char buf[BUF_SIZE];
	
	while (1) {
		memset(&buf, 0, BUF_SIZE * sizeof(char));
		recv(sock, buf, BUF_SIZE, 0);
		if (strcmp(buf, "exit\n") == 0) {
			break;
		}
		into_orcish(buf);
		send(sock, buf, BUF_SIZE, 0);
	}
	close(sock);
	return 0;
}

void* handle_mem_cell(void* arg) {
	struct thread_routine_info info;
	memcpy(&info, arg, sizeof(struct thread_routine_info));
	fprintf(stderr,"Cell %d ready.\n", info.offset);
	int* connectionfd = (int*)info.memptr + info.offset;
	*connectionfd = 0;
	while (1) {
		if (*connectionfd != 0) {
			fprintf(stderr,"New connection on cell %d.\n", info.offset);
			tcp_routine(connectionfd);
			*connectionfd = 0;
			fprintf(stderr,"Cell %d now free.\n", info.offset);
			kill(getpid(), SIGUSR1);
		}		
	}
	return 0;
}