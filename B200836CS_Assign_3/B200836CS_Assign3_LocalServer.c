#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
    int server_socket;
    server_socket = socket(AF_INET,SOCK_DGRAM, 0);
    if(server_socket < 0){
        printf("Server Socket is Not Created\n");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(4044);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr*)&server_address,sizeof(server_address));
    if(bind_status < 0){
        printf("Error in binding\n");
        return -1;
    }
    struct sockaddr_in client_address;
    int len = sizeof(client_address);
    char message[1000];
    char ip_address[1000];
    int root_socket;
    struct sockaddr_in root_address;
    while(1){
        // Received from Client
        int n = recvfrom(server_socket,message,1000, 0, (struct sockaddr*) &client_address, &len);
        message[n] = '\0';

        printf("Request from Client : %s\n",message);
        //printf("Querying root DNS\n");

        root_socket = socket(AF_INET,SOCK_DGRAM,0);

        if(root_socket < 0){
            printf("Error in root socket creation\n");
            return -1;
        }
        root_address.sin_family = AF_INET;
        root_address.sin_port = htons(4041);
        root_address.sin_addr.s_addr = INADDR_ANY;


        // sending the message that is received from Client to Root Server
        sendto(root_socket, message, strlen(message),0,(struct sockaddr*) &root_address,len);
        if(strcmp(message,"exit") == 0){
            break;
        }
        // recevive from IP address from the root server
        n = recvfrom(root_socket, ip_address,1000,0,(struct sockaddr*)&root_address,&len);
        ip_address[n] = '\0';
        printf("IP address of %s : %s\n",message,ip_address);

        close(root_socket);
        // sending to the Client
        sendto(server_socket,ip_address,strlen(ip_address),0,(struct sockaddr*) &client_address,len);
    }
    close(server_socket);
    return 0;
}