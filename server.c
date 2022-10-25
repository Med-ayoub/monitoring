#include <stdio.h> 
#include <string.h>   
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>
#include <poll.h>
#include <semaphore.h>
#include <time.h>
#include "parse.h"

#define PORT 8888
#define AP_PATH "/tmp/access_points"




static int nfds = 0;

struct array_data {
	void *array_first_item;
	int *length;
	ap_info_list_t *ap_list;
};

static volatile int sockfd = 0;


void *send_message(void * arg) {

	struct array_data *client_array = (struct array_data  *)arg;
	int *fds = client_array->array_first_item;
	while(1){
		char message[1024]= "";
		format_response(AP_PATH,client_array->ap_list,message);
		for(int i = 0 ; i < *(client_array->length) ; i++){
			write(fds[i],message, sizeof(message));
		}
		sleep(5);
	}
}


int main() {
	
	//define & init variables 
	int server_fd , new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	int fds[256];
	struct array_data data_client;
	pthread_t check_json_thread;
	ap_info_list_t access_points;

	format_ap_list(AP_PATH,&access_points);

	data_client.array_first_item = fds;
	data_client.length = &nfds;
	data_client.ap_list = &access_points;
	
	
	//Create thread
	pthread_create(&check_json_thread, NULL, &send_message, &data_client);
	

	//create socket file descriptor
	if((server_fd=socket(AF_INET,SOCK_STREAM,0))<0) 
	{
		perror("socket failed");
		return(EXIT_FAILURE);
	}
	
	//set sock option 
	if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,
		(char*)&opt,sizeof(opt))<0)
	{
		perror("setsockopt");
		return(EXIT_FAILURE);
	}
	//typde of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port =htons(PORT);
	
	//bind socket to port
	if(bind(server_fd, (struct sockaddr *)&address,
		sizeof(address))<0)
	{
		perror("bind failed");
		return(EXIT_FAILURE);
	}
	//Server is ready to listen
	if(listen(server_fd,3)<0) 
	{
		perror("listen failed");
		return(EXIT_FAILURE);
	}
	while(1)
	{
		if((new_socket = accept(server_fd,(struct sockaddr *)&address, 
									(socklen_t*)&addrlen))<0)  
		{  
			perror("accept");  
            break;  
        }
        else
        {
			fds[nfds] = new_socket;
			nfds++;  
		}
        
	}
	pthread_join(check_json_thread, NULL);
    close(server_fd);

	return EXIT_SUCCESS;
}





