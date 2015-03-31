#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define MAX_LEN 2048

struct messageA{
	int message_type;
	int message_len;
	char message_body[MAX_LEN];
};
typedef struct messageA message;

int main(int argv,char *argc[])
{
	if(argv < 3) {
		printf("Please input in the format <executable code> <ip addr> <port no>\n");
		return -1;
	}
	int tcp_sckt,udp_sckt, bytes_recv,bytes_recieved;
        struct hostent *host;
        int choice;
        struct sockaddr_in server_addr;  
	struct sockaddr_in client_addr;
	char send_data[MAX_LEN];
	char recieved[50];
	host = gethostbyname(argc[1]);
	if ((tcp_sckt = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        {
            printf("Error in socket creation\n");
            exit(1);
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(argc[2]));
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        memset(&(server_addr.sin_zero),0,8);
	printf("---------- Establishing a TCP connection with the server -------------\n");
	if (connect(tcp_sckt, (struct sockaddr *) & server_addr, sizeof(struct sockaddr)) == -1) 
        {
            printf("Error in connection\n");
            exit(1);
        }
	printf("####### Connection Successfully Established #########\n");
        printf("\t====== Enter u to send UDP port request =======\n");
        scanf("%s",send_data);
	message *m = (message *)malloc(sizeof(message));
	m->message_type =1;
	m->message_len =0;
	m->message_body[0]='\0';
        send(tcp_sckt , m , sizeof(message) , 0);

	bytes_recieved=recv(tcp_sckt,m,sizeof(message),0);
	strcpy(recieved, m->message_body);
	if(bytes_recieved!=0)
	{
		printf("######################################################\n");
		printf("\t\tRecieved TCP Packet is\n");
		printf("\t\t Message Type : %d\n",m->message_type);
		printf("\t\t Message Length : %d\n",m->message_len);
		printf("\t\t Message Body : %s\n",m->message_body);
		printf("######################################################\n");

	        int udp_port_no;
	        udp_port_no = atoi(recieved);
	        
		close(tcp_sckt);
		if ((udp_sckt = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
        	{
		    printf("Error in socket creation\n");
		    exit(1);
        	}
        	server_addr.sin_family = AF_INET;     
		server_addr.sin_port = htons(udp_port_no);   
		server_addr.sin_addr = *((struct in_addr *)host->h_addr);
		memset(&(server_addr.sin_zero),0,8);
		while(1)
		{
			printf("\t ------- Enter 1 to send message and 0 to exit -------- \n");
			scanf("%d",&choice);
			if(choice == 0)
			{
				m->message_type = 5;
				m->message_len = 0;
				m->message_body[0]='\0';
			        sendto(udp_sckt, m, sizeof(message),0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
				break;
			}
			else
			{
			      printf("##########################################\n");
			      printf("Enter the message\n");
			      int k=0;
			      while(1) {
			      		char ch;ch = getchar();
					if(ch=='\n')
						break;
					send_data[k++] = ch;
			      }
			      send_data[k] ='\0';
			      scanf ("%[^\n]%*c", send_data);
			     // gets(send_data);
			      m->message_type = 3;
			      m->message_len = strlen(send_data);
			      strcpy(m->message_body,send_data);
			      sendto(udp_sckt, m, sizeof(message),0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
			      printf("\t ---- Message %s sent ---- \n",send_data);
			      int client_struct_len;
			      int bytes_received = recvfrom(udp_sckt,m,sizeof(message),0,(struct sockaddr *)&client_addr,&client_struct_len);
			      if( bytes_received >0 && m->message_type == 4 )
			      {
					printf("######################################################\n");
					printf("ACKNOWLEDGEMENT RECEIVED IN FOLLOWING UDP \n");
					printf("\t\tRecieved UDP Packet is\n");
					printf("\t\t Message Type : %d\n",m->message_type);
					printf("\t\t Message Length : %d\n",m->message_len);
					printf("\t\t Message Body : %s\n",m->message_body);
					printf("######################################################\n");

			      }
			}
		
		
		}
		close(udp_sckt);
	
	}
	
}

