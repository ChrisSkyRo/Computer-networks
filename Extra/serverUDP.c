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

	// receive the numbers from the server
	uint16_t a, b, t, ca, cb;
	recvfrom(s, &a, sizeof(a), MSG_WAITALL, (struct sockaddr *) &client, &l);
	a = ntohs(a);
	ca = a;
	recvfrom(s, &b, sizeof(b), MSG_WAITALL, (struct sockaddr *) &client, &l);
	b = ntohs(b);
	cb = b;
	// calculate GCD
	while(b)
	{
		t = b;
		b = a%b;
		a = t;
	}
	// calculate LCD
	b = ca*cb/a;
	// send the GCD and LCD
	a = htons(a);
	b = htons(b);
	sendto(s, &a, sizeof(a), 0, (struct sockaddr *) &client, l);
	sendto(s, &b, sizeof(b), 0, (struct sockaddr *) &client, l);
  	
  	close(s);
}

