// udp server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(){
    int server_socket;
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    
    struct sockaddr_in client_address;
    int len = sizeof(client_address);
    
    char message[250];
    int n = recvfrom(server_socket, message, 250, MSG_WAITALL, (struct sockaddr*) &client_address, &len);
    message[n] = '\0';
    printf("Message received from the client : %s\n",message);
    
    int l = strlen(message);
    for(int i = 0;i<(l/2);i++){
        char temp = message[i];
        message[i] = message[l-1-i];
        message[l-1-i] = temp;
    }
    
    printf("Message sent to the client : %s\n",message);
    sendto(server_socket, message, strlen(message), MSG_CONFIRM, (struct sockaddr*) &client_address, len);
    
    return 0;
}
