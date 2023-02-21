#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    
    int connection_status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if(connection_status == -1){
        printf("There was an error making a connection to the remote socket \n\n");
    }
    
    char message[250];
    printf("Enter a String : \n");
    scanf("%s",message);
    
    send(client_socket,message,strlen(message),0);
    printf("Message sent to server : %s\n",message);
    
    recv(client_socket,&message, 250,0);
    printf("Message received from the server : %s\n",message);
    
    close(client_socket);
    return 0;
}
