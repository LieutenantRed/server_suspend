#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define handle_error(msg) \
    do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

typedef enum {TCP, UDP} prot_t;

int main(int argc, char** argv) {
	prot_t t;
	struct sockaddr_in host_addr;
	memset(&host_addr, 0, sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	unsigned int thread_c = 0;

	int opt;
	int flag = 0;
	
	while ((opt = getopt(argc, argv, "t:h:p:n:")) != -1) {
		switch(opt) {
		case 't':
			if (strcmp(optarg, "tcp") == 0)
				t = TCP;
			if (strcmp(optarg, "udp") == 0)
				t = UDP;
			++flag;
			break;
		case 'h':
			if (inet_aton(optarg, &host_addr.sin_addr) != 1) {
				fprintf(stderr, "invalid ip address\n");
				exit(EXIT_FAILURE);
			}
			++flag;
			break;
		case 'p':
			host_addr.sin_port = htons(atoi(optarg));
			++flag;
			break;
		case 'n':
			thread_c = atoi(optarg);
			if (thread_c == 1)
				thread_c = 0; // pthread off
			break;
		default:
			printf("Usage: %s -t [tcp/udp] -h [host_ip] -p [host_port] (-n [num])\n", argv[0]);
			exit(EXIT_FAILURE);
		}	
	}
	
	if (flag < 3) {
		printf("Usage: %s -t [tcp/udp] -h [host_ip] -p [host_port] (-n [num])\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (thread_c == 0) {
		int sockd;
		if (t == UDP)
			sockd = socket(AF_INET, SOCK_DGRAM, 0);
		if (t == TCP)
			sockd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockd == -1) {
			fprintf(stderr, "invalid socket params\n");
			exit(EXIT_FAILURE);
		}
		
		char buf[BUF_SIZE];
		if (t == TCP) {
			if (connect(sockd, (struct sockaddr*)&host_addr, sizeof(host_addr)) == -1)
				handle_error("connection to host failed\n");
			while (1) {
				memset(&buf, 0, BUF_SIZE * sizeof(char));
				fgets((void*)buf, BUF_SIZE, stdin);
				send(sockd, buf, BUF_SIZE, 0);
				if (strcmp(buf, "exit\n") == 0) {
					break;
				}
				recv(sockd, buf, BUF_SIZE, 0);
				printf("%s", buf);
			}
		}
		if (t == UDP) {
			while(1) {
				memset(&buf, 0, BUF_SIZE * sizeof(char));
				fgets((void*)buf, BUF_SIZE, stdin);
				sendto(sockd, buf, BUF_SIZE, 0, (struct sockaddr*)&host_addr, sizeof(host_addr));
				if (strcmp(buf, "exit\n") == 0) {
					break;
				}
				recv(sockd, buf, BUF_SIZE, 0);
				printf("%s", buf);
			}
		}
		close(sockd);
	} else {
		//TODO
	}

	return 0;	
}
