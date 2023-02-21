#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(){
    int client_socket;
    client_socket = socket(AF_INET,SOCK_DGRAM, 0);
    if(client_socket < 0){
        printf("Socket is Not Created\n");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(4044);
    server_address.sin_addr.s_addr = INADDR_ANY;
    int length = sizeof(server_address);
    char message[1000];
    char ip_address[1000];
    while(1){
        printf("Enter the hostname : \n");
        scanf("%s",message);
        sendto(client_socket,message,strlen(message),0,(struct sockaddr*) &server_address,sizeof(server_address));

        if(strcmp(message, "exit") == 0){
            break;
        }
        int n = recvfrom(client_socket,ip_address,1000,0,(struct sockaddr*)&server_address,&length);
        ip_address[n] = '\0';
        printf("The IP address of %s : %s\n",message,ip_address);
    }
    close(client_socket);
    return 0;
}