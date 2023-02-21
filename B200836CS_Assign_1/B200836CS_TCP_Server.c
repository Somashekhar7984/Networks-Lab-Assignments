#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_address;
    int addrlen = sizeof(server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    listen(server_socket,5);
    
    int client_socket;
    client_socket = accept(server_socket,(struct sockaddr*) &server_address, (socklen_t*) &addrlen);
    
    char message[250];
    recv(client_socket, &message, 250, 0);

    printf("Message received from client: %s\n",message);
    int len = strlen(message);
    for(int i = 0;i<(len/2);i++){
        char temp = message[i];
        message[i] = message[len-1-i];
        message[len-1-i] = temp;
    }
    
    send(client_socket,message,strlen(message),0);
    printf("Message sent to the client : %s\n",message);
    close(client_socket);
}
