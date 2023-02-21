#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
struct domains{
    char domain_names[100];
    char ip[100];
};
int main(){
    struct domains arr[6];
    strcpy(arr[0].domain_names,"www.baidu.com");
    strcpy(arr[0].ip,"75.123.66.24");
    strcpy(arr[1].domain_names,"www.yahoo.com");
    strcpy(arr[1].ip,"132.142.165.224");
    strcpy(arr[2].domain_names,"www.google.com");
    strcpy(arr[2].ip,"195.182.167.204");
    strcpy(arr[3].domain_names,"www.redcross.org");
    strcpy(arr[3].ip,"176.187.12.184");
    strcpy(arr[4].domain_names,"www.bluecross.org");
    strcpy(arr[4].ip,"123.240.126.203");
    int auth_socket;
    auth_socket = socket(AF_INET,SOCK_DGRAM, 0);
    if(auth_socket < 0){
        printf("Error in auth_socket creation\n");
        return -1;
    }
    struct sockaddr_in authserver_address;
    authserver_address.sin_family = AF_INET;
    authserver_address.sin_port = htons(4043);
    authserver_address.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(auth_socket, (struct sockaddr*)&authserver_address, sizeof(authserver_address));
    if(bind_status < 0){
        printf("Error in Binding\n");
        return -1;
    }

    char message[1000];
    struct sockaddr_in client_address;
    int length = sizeof(client_address);
    while(1){
        int n = recvfrom(auth_socket,message,1000, 0, (struct sockaddr*) &client_address,&length);
        message[n] = '\0';

        if(strcmp(message,"exit") == 0){
            break;
        }
        printf("Request from the client : %s\n",message);
        int index = -1;
        int flag = 0;
        for(int i = 0;i<5;i++){
            if(strcmp(arr[i].domain_names,message) == 0){
                index = i;
                flag = 1;
            }
        }
        char ip_address[1000];
        if(index == -1){
            strcpy(ip_address,"Not Found");
            sendto(auth_socket,ip_address,strlen(ip_address),0,(struct sockaddr*)&client_address,length);
            printf("IP address of %s is %s\n",message,ip_address);
        }
        else{
            strcpy(ip_address,arr[index].ip);
            printf("IP address of %s is %s\n",message,ip_address);
            sendto(auth_socket,ip_address,strlen(ip_address),0,(struct sockaddr*)&client_address,length);
        }
    }
    close(auth_socket);
    return 0;
}