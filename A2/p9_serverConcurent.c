#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
 
void serveClient(int c) {
	// serving client
	uint16_t a, b;
    int i;
    uint16_t va[100], vb[100];
    // receive the two arrays from client
    recv(c, &a, sizeof(a), MSG_WAITALL);
    a = ntohs(a);
    for(i = 0; i < a; i++)
    {
		recv(c, &va[i], sizeof(va[i]), MSG_WAITALL);
		va[i] = ntohs(va[i]);
	}

    recv(c, &b, sizeof(b), MSG_WAITALL);
	b = ntohs(b);
    for(i = 0; i < b; i++)
	{
		recv(c, &vb[i], sizeof(vb[i]), MSG_WAITALL);
		vb[i] = ntohs(vb[i]);
	}

    // find the numbers unique to the first array
    int j, ok;
    uint16_t vx[100], x = 0;
    for(i = 0; i < a; i++)
    {
		ok = 1;
		for(j = 0; j < b && ok; j++)
			if(va[i] == vb[j])
				ok = 0;
		if(ok)
          	vx[x++] = va[i];
    }

	// send the final array to the client
	int cx = x;
	x = htons(x);
	send(c, &x, sizeof(x), 0);
	for(i = 0; i < cx; i++)
	{
		vx[i] = htons(vx[i]);
		send(c, &vx[i], sizeof(vx[i]), 0);
	}
	close(c);
	// end of serving client
}
 
int main() {
	int s;
  	struct sockaddr_in server, client;
  	int c;
   	uint32_t l;
	int option = 1;
  	s = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
  	if (s < 0) {
    	printf("Error on creating server socket\n");
    	return 1;
  	}
  
  	memset(&server, 0, sizeof(server));
  	server.sin_port = htons(1235);
  	server.sin_family = AF_INET;
  	server.sin_addr.s_addr = INADDR_ANY;
  
  	if (bind(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
    	printf("Error on bind\n");
    	return 1;
  	}
 	
  	listen(s, 5);
  	
  	l = sizeof(client);
  	memset(&client, 0, sizeof(client));
	printf("Server started\n");
  	
  	while (1) {
    	c = accept(s, (struct sockaddr *) &client, &l);
    	printf("A client connected.\n");
    	// child
		if (fork() == 0) 
		{
      		serveClient(c);
      		return 0;
    	}
  }
}

