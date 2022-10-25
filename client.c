#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 8888
#define MAX_SIZE 1024

void display_message(int sockfd) {
    char buff[MAX_SIZE];
    while(1) 
    {
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("%s\n", buff);
    }
}

int main() {

	int sockfd =0,client_fd;
	struct sockaddr_in serv_addr;
	
	//create socket file descriptor
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0) 
	{
		perror("socket failed");
		return(EXIT_FAILURE);
	}

	//typde of socket created
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port =htons(PORT);
	
	if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr) <=0) 
	{
		perror("invalid addr");
		return(EXIT_FAILURE);
	}

	if((client_fd = connect(sockfd, (struct sockaddr *)&serv_addr,
		sizeof(serv_addr)))<0)
	{
		perror("connection failed");
		return(EXIT_FAILURE);
	}

	printf("connected to the server..\n");
	
	display_message(sockfd);
	close(sockfd);
		
	return EXIT_SUCCESS;
}
