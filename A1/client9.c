#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
 
int main(int argc, char** argv) {
	if(argc < 3)
	{
		printf("Enter the port and the ip of the server!\n");
		return 1;
	}
	
	int c;
  	struct sockaddr_in server;
  	uint16_t a, b; 

	c = socket(AF_INET, SOCK_STREAM, 0);
  	if (c < 0) {
    	printf("Error on creating socket\n");
    	return 1;
  	}
  	
	int port = atoi(argv[1]);

  	memset(&server, 0, sizeof(server));
  	server.sin_port = htons(port);
  	server.sin_family = AF_INET;
  	server.sin_addr.s_addr = inet_addr(argv[2]);
 	
	if(connect(c, (struct sockaddr *) &server, sizeof(server)) < 0) {
		printf("Error connecting to server\n");
	  	return 1;
  	}
  	int i;
  	uint16_t va[100], vb[100];
  	// read the first array
  	printf("a = ");
  	scanf("%hu", &a);
  	for(i = 0; i < a; i++)
 		scanf("%hu", &va[i]);
  	// read the second array
  	printf("b = ");
  	scanf("%hu", &b);
  	for(i = 0; i < b; i++)
 		scanf("%hu", &vb[i]);
  	// send the arrays to the server
  	uint16_t ca = a, cb = b;
  	a = htons(a);
  	send(c, &a, sizeof(a), 0);
  	for(i = 0; i < ca; i++)
  	{
		va[i] = htons(va[i]);
 		send(c, &va[i], sizeof(va[i]), 0);
  	}
  	b = htons(b);
  	send(c, &b, sizeof(b), 0);
  	for(i = 0;i < cb; i++)
  	{
  		vb[i] = htons(vb[i]);
		send(c, &vb[i], sizeof(vb[i]), 0);
  	}
  	// receive the final array
  	uint16_t x, vx[100];
 	recv(c, &x, sizeof(x), 0);
  	x = ntohs(x);
  	for(i = 0; i < x; i++)
  	{
		recv(c, &vx[i], sizeof(vx[i]), 0);
		vx[i] = ntohs(vx[i]);
  	}
  	// print numbers
  	if(x == 0)
		printf("There are no unique numbers to the first array.\n");
  	else
  	{
		printf("Numbers unique to the first array are: ");
		for(i = 0; i < x; i++)
			printf("%hu ", vx[i]);
		printf("\n");
	}
	close(c);
}

