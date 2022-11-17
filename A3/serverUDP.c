#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  	int s;
  	struct sockaddr_in server, client;
  	uint32_t l;
  	
  	s = socket(AF_INET, SOCK_DGRAM, 0);
  	if (s < 0) {
    	printf("Error creating server socket\n");
    	return 1;
  	}

	int port = 1234;
	if(argc == 2)
		port = atoi(argv[1]);
  	
  	memset(&server, 0, sizeof(server));
  	server.sin_port = htons(port);
  	server.sin_family = AF_INET;
  	server.sin_addr.s_addr = INADDR_ANY;
  	
  	if (bind(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
    	printf("Error on bind\n");
    	return 1;
  	}
	 
  	l = sizeof(client);
  	memset(&client, 0, sizeof(client));

	// receive request from client
	uint16_t request;
	recvfrom(s, &request, sizeof(request), MSG_WAITALL, (struct sockaddr *) &client, &l);
	request = ntohs(request);

	// send the time in seconds since January 1, 1970 to client
	time_t secs;
	secs = time(NULL);
	uint32_t seconds = secs;
	secs = htonl(seconds);
	sendto(s, &secs, sizeof(secs), 0, (struct sockaddr *) &client, l);
  	
  	close(s);
}

