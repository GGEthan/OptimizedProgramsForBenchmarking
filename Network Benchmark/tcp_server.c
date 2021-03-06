#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>
#define NUM_BLOCKS 1
#define NUM_CLIENTS 1
#define BLOCK_SIZE 1024   // 65536 - 64KB
void *connection_handler(void *);

int main(int argc, char *argv[])
{
    int listenfd = 0,c,client_sock,connfd = 0;
    struct sockaddr_in serv_addr,client;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
    
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;
    while( (client_sock = accept(listenfd, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
	puts("Connection accepted");
	if( pthread_create( &thread_id , NULL , connection_handler , (void*) &client_sock) < 0)
	{
		perror("could not create thread");
		return 1;
	}
	//Now join the thread , so that we dont terminate before the thread
	//pthread_join( thread_id , NULL);
	puts("Handler assigned");
	}
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
	}
	

void *connection_handler(void *socket_desc)
{
        int i,j;
        char sendBuff[BLOCK_SIZE];
	memset(sendBuff, '0', sizeof(sendBuff)); 
    	int connfd = *(int*)socket_desc;
	for(i=0;i<BLOCK_SIZE;i++)
	{
	sendBuff[i]='a';
	}
	
        //writing data
        for(j=0;j<NUM_BLOCKS/NUM_CLIENTS;j++){     
	write(connfd, sendBuff,BLOCK_SIZE); 
        }        	
        close(connfd);
        sleep(1);
     
}
