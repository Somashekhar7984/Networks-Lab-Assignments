#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
    strcpy(arr[0].domain_names,"baidu.com");
    strcpy(arr[0].ip,"74,23.66.14");
    strcpy(arr[1].domain_names,"yahoo.com");
    strcpy(arr[1].ip,"13.12.16.244");
    strcpy(arr[2].domain_names,"google.com");
    strcpy(arr[2].ip,"19.18.67.234");
    strcpy(arr[3].domain_names,"redcross.org");
    strcpy(arr[3].ip,"76.87.12.214");
    strcpy(arr[4].domain_names,"bluecross.org");
    strcpy(arr[4].ip,"23.24.26.213");

    int tld_socket;
    tld_socket = socket(AF_INET,SOCK_DGRAM,0);
    if(tld_socket < 0){
        printf("Error in tld socket creation\n");
        return -1;
    }

    struct sockaddr_in tldserver_address;
    tldserver_address.sin_family = AF_INET;
    tldserver_address.sin_port = htons(4042);
    tldserver_address.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(tld_socket,(struct sockaddr*)&tldserver_address, sizeof(tldserver_address));
    if(bind_status < 0){
        printf("Error in Binding\n");
        return -1;
    }

    char message[1000];
    char ip_address[1000];
    struct sockaddr_in client_address;
    int length = sizeof(client_address);
    int auth_socket;
    struct sockaddr_in authserver_address;
    while(1){
        int n = recvfrom(tld_socket,message,1000, 0, (struct sockaddr*)& client_address, &length);
        message[n] = '\0';
        if(strcmp(message,"exit") == 0){
            sendto(auth_socket,message,strlen(message),0,(struct sockaddr*)&authserver_address, length);
            break;
        }
        printf("Request from the client : %s\n",message);
        char domain[100];
        domain[0] = '\0';
        int i = 0;
        while(message[i] != '.'){
            i++;
        }
        int j = 0;
        i++;
        while(message[i] != '\0'){
            domain[j] = message[i];
            i++;
            j++;
        }
        domain[j] = '\0';
        int index = INT_MAX;
        for(int k = 0;k<5;k++){
            if(strcmp(domain,arr[k].domain_names) == 0){
                index = k;
                break;
            }
        }
        if(index == INT_MAX){
            printf("Domain is not availabe\n");
            auth_socket = socket(AF_INET,SOCK_DGRAM,0);
            if(auth_socket < 0){
                printf("Error in authentication socket\n");
                return -1;
            }
            authserver_address.sin_family = AF_INET;
            authserver_address.sin_port = htons(4043);
            authserver_address.sin_addr.s_addr = INADDR_ANY;
            int num = sendto(auth_socket,message,strlen(message),0,(struct sockaddr*)&authserver_address, length);
            if(strcmp(message,"exit") == 0){
                break;
            }
            n = recvfrom(auth_socket,ip_address,1000,0,(struct sockaddr*)&authserver_address,&length);
            ip_address[n] = '\0';
            printf("IP address of %s is %s\n",message,ip_address);
            close(auth_socket);

            sendto(tld_socket,ip_address,strlen(ip_address),0,(struct sockaddr*)&client_address,length);
        }
        else{
            char domain_ip[1000];
            strcpy(domain_ip,arr[index].ip);
            printf("IP address of Domain %s is %s\n",domain,domain_ip);

            auth_socket = socket(AF_INET,SOCK_DGRAM,0);
            if(auth_socket < 0){
                printf("Error in authentication socket\n");
                return -1;
            }
            authserver_address.sin_family = AF_INET;
            authserver_address.sin_port = htons(4043);
            authserver_address.sin_addr.s_addr = INADDR_ANY;

            sendto(auth_socket,message,strlen(message),0,(struct sockaddr*)&authserver_address, length);
            if(strcmp(message,"exit") == 0){
                break;
            }
            n = recvfrom(auth_socket,ip_address,1000,0,(struct sockaddr*)&authserver_address,&length);
            ip_address[n] = '\0';
            printf("IP address of %s is %s\n",message,ip_address);
            close(auth_socket);
            sendto(tld_socket,ip_address,strlen(ip_address),0,(struct sockaddr*)&client_address,length);
        }
    }

    close(tld_socket);
    return 0;
}