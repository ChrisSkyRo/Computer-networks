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
	
	// send request to server
	uint16_t request;
	request = 1;
	request = htons(request);
	sendto(c, &request, sizeof(request), 0, (struct sockaddr *) &server, l);

	// receive seconds from server
	uint32_t seconds;
	recvfrom(c, &seconds, sizeof(seconds), MSG_WAITALL, (struct sockaddr *) &server, &l);
	seconds = ntohl(seconds);
	printf("Seconds since January 1, 1970: %d\n", seconds);
	
  	close(c);
}

