// udp client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(){
    int client_socket;
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    char message[250];
    printf("Enter a string : \n");
    scanf("%s",message);
    
    
    printf("Message sent to server : %s\n",message);
    sendto(client_socket, message, strlen(message), MSG_CONFIRM, (struct sockaddr*) &server_address, sizeof(server_address));
    
    int len;
    int n = recvfrom(client_socket, message, 250, MSG_WAITALL, (struct sockaddr*) &server_address, &len);
    message[n] = '\0';
    printf("Message received from server : %s\n",message);
    close(client_socket);
    return 0;
}
