#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  	int c;
  	struct sockaddr_in server;
	uint32_t l;
  	
  	c = socket(AF_INET, SOCK_DGRAM, 0);
  	if (c < 0) {
    	printf("Error creating client socket\n");
    	return 1;
  	}

	char address[] = "127.0.0.1";
	int port = 1234;
	if(argc == 2)
		port = atoi(argv[1]);
  	
	l = sizeof(server);
  	memset(&server, 0, sizeof(server));
  	server.sin_port = htons(port);
  	server.sin_family = AF_INET;
	if(argc == 3)
		server.sin_addr.s_addr = inet_addr(argv[2]);
	else
		server.sin_addr.s_addr = inet_addr(address);
	
	// read two ints
	uint16_t a, b;
	printf("a = ");
	scanf("%hu", &a);
	printf("b = ");
	scanf("%hu", &b);
	// send the ints to the server
	a = htons(a);
	sendto(c, &a, sizeof(a), 0, (struct sockaddr *) &server, l);
	b = htons(b);
	sendto(c, &b, sizeof(b), 0, (struct sockaddr *) &server, l);

	// receive the GCD and LCM
	recvfrom(c, &a, sizeof(a), MSG_WAITALL, (struct sockaddr *) &server, &l);
	recvfrom(c, &b, sizeof(b), MSG_WAITALL, (struct sockaddr *) &server, &l);
	a = ntohs(a);
	b = ntohs(b);
	printf("GCD: %hu LCM: %hu\n", a, b);
	
  	close(c);
}

