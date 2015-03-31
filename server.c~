#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BACKLOG 1024
#define MAX_LEN 2048

struct messageA{
	int message_type;
	int message_len;
	char message_body[MAX_LEN];
};
typedef struct messageA message;
int main(int argc,char *argv[])
{
	if( argc < 2 ) {
		printf("Please Input in following format <executable code> <port no>\n");
		return -1;
	}
	int i;
	int tcp_sckt,udp_sckt,connect,bytes_recv,addr_length;
	struct sockaddr_in server_addr,client_addr;
	int size;
	char send_data[50];
	pid_t child_id;
	if ((tcp_sckt = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        {
            printf("Error in socket creation\n");
            exit(1);
        }
        server_addr.sin_family = AF_INET;        
        server_addr.sin_port = htons( atoi( argv[1] ) );  
        printf("------------ Establishing TCP stream on Port no %s | Network Byte Order %d -----------------\n" ,argv[1],server_addr.sin_port);
        server_addr.sin_addr.s_addr = INADDR_ANY; 
        memset(&(server_addr.sin_zero),0,8);
        
        if (bind(tcp_sckt, (struct sockaddr *) & server_addr, sizeof(struct sockaddr)) == -1) 
	{
                                                                      
            printf("Unable to bind\n");
            exit(1);
        }
       
	if (listen(tcp_sckt, BACKLOG) == -1)
	{
            printf("Error in listening\n");
            exit(1);
	}
      	while(1)
       	{ 
		printf("############ Tcp Server is waiting for client on port %d ##########\n",atoi(argv[1]));
		fflush(stdout);
		size = sizeof(struct sockaddr_in);

		connect = accept(tcp_sckt, (struct sockaddr *)&client_addr,&size);
	
		if((child_id=fork())==0)
		{
			child_id = getpid();
			printf("\t===== Connected to client (PID = %d) ========\n",child_id);
			
			close(tcp_sckt);
			message *m = (message *)malloc(sizeof(message));
			bytes_recv = recv(connect,m,sizeof(message),0);

			printf("######################################################\n");
			printf("\t\tRecieved TCP Packet is\n");
			printf("\t\t Message Type : %d\n",m->message_type);
			printf("\t\t Message Length : %d\n",m->message_len);
			printf("\t\t Message Body : %s\n",m->message_body);
			printf("######################################################\n");
			
			fflush(stdout);
			if ( m->message_type == 1 )
			{

			      printf("---- Request Received for Granting UDP Port. Now closing TCP Stream for Client ---- \n");
			      printf("Enter the UDP Port\n");
			      scanf("%s",send_data);
			      int udp_port_no;
			      udp_port_no = atoi(send_data);
			      m->message_type = 2;
			      m->message_len = strlen(send_data);
			      strcpy(m->message_body,send_data);
			      send(connect,m,sizeof(message),0);
			      close(connect);
			      if ((udp_sckt = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
			      {
				    printf("Error in socket creation\n");
				    exit(1);
			      }
			      server_addr.sin_family = AF_INET;
			      server_addr.sin_port = htons(udp_port_no);
			      server_addr.sin_addr.s_addr = INADDR_ANY;
			      memset(&(server_addr.sin_zero),0,8);
			      if (bind(udp_sckt, (struct sockaddr *) & server_addr, sizeof(struct sockaddr)) == -1)
			      {
				    printf("Unable to bind\n");
				    exit(1);
			      }
			      addr_length = sizeof(struct sockaddr);
        		       printf("------------ Establishing UDP Socket on Port no %d | Network Byte Order %d -----------------\n" ,udp_port_no,server_addr.sin_port);
			      
			      printf("########## UDP Server Waiting for client ( PID = %d ) on port %d ######### \n",child_id,udp_port_no);
			      fflush(stdout);
			      
			      
			      while(1)
			      {

				 	bytes_recv = recvfrom(udp_sckt,m,sizeof(message),0,(struct sockaddr *)&client_addr, &addr_length);
					if(bytes_recv <= 0)
						break; 
					printf("######################################################\n");
					printf("\t\tRecieved UDP Packet from client ( PID = %d ) is\n",child_id);
					printf("\t\t Message Type : %d\n",m->message_type);
					printf("\t\t Message Length : %d\n",m->message_len);
					printf("\t\t Message Body : %s\n",m->message_body);
					printf("######################################################\n\n");
					
					if(m->message_type == 5)	
					{
						printf("Closing Connection for client ( PID = %d )\n",child_id);
						break;
					}
					printf("\t --------- Sending ACK ---------- \n");
					m->message_type = 4;
					m->message_len = 0;
					m->message_body[0]='\0';
					int bytes_sent = sendto(udp_sckt,m,sizeof(message),0,(struct sockaddr *)&client_addr,addr_length);
					if(bytes_sent != sizeof(message))
						printf("error in sending Type 4 message\n");
					fflush(stdout);
					printf("\t---------------------------------\n\n");
			       }
			      
			 }
			close(udp_sckt);
			exit(0);
		}
		
	}
   return 0;
}
