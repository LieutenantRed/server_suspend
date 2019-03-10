#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "orcish.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

#define handle_error(msg) \
    do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)

struct thread_routine_info {
	void* memptr;
	int offset;
};

void* tcp_routine(void* arg);
void* handle_mem_cell(void* arg);

#endif