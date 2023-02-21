#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main(){
    int client_socket;
    printf("Creating a socket\n");
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    printf("Connecting to the server\n");
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connection_status = connect(client_socket,(struct sockaddr*)&server_address, sizeof(struct sockaddr));
    if(connection_status < 0){
        printf("Connection Not Established\n");
    }
    else{
        printf("Connection Established\n");
    }
    char message[1000];
    if(fork() == 0){
        while(1){
            // Receiving message
            int rec = recv(client_socket,(void *)message,sizeof(message),0);
            //memset(message,'\0',strlen(message));
            if(rec < 0){
                printf("Message is not received\n");
            }
            printf("%s\n",message);
        }
    }
    else{
        while(1){
            scanf("%[^\n]%*c",message);
            //printf("YOU : %s\n",message);
            if(strcmp(message,"/quit") == 0){
                //memset(message,'\0',strlen(message));
                int send_status = send(client_socket,(void *)message,strlen(message)+1, 0);
                if(send_status < 0){
                    printf("Message is Not Sent\n");
                }
                close(client_socket);
                return 0;
            }
            //memset(message,'\0',strlen(message));
            int send_status = send(client_socket,(void *)message,strlen(message)+1, 0);
            if(send_status < 0){
                printf("Message is Not Sent\n");
            }
        }
    }
    return 0;
}