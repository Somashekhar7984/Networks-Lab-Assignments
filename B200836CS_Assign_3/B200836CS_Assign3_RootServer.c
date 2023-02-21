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
    struct domains arr[3];
    strcpy(arr[0].domain_names,".com");
    strcpy(arr[0].ip,"12.13.33.34");
    strcpy(arr[1].domain_names,".org");
    strcpy(arr[1].ip,"7.34.22.21");
    strcpy(arr[2].domain_names,".edu");
    strcpy(arr[2].ip,"57.235.126.23");


    int root_socket;
    root_socket = socket(AF_INET,SOCK_DGRAM,0);
    if(root_socket < 0){
        printf("root socket is not created\n");
        return -1;
    }

    struct sockaddr_in rootserver_address;
    rootserver_address.sin_family = AF_INET;
    rootserver_address.sin_port = htons(4041);
    rootserver_address.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(root_socket,(struct sockaddr*)&rootserver_address,sizeof(rootserver_address));
    if(bind_status < 0){
        printf("Error in Binding\n");
        return -1;
    }

    char message[1000];
    char ip_address[1000];
    struct sockaddr_in client_address;
    int length = sizeof(struct sockaddr_in);

    int tld_socket;
    struct sockaddr_in tldserver_address;
    while(1){
        int n = recvfrom(root_socket,message,1000,0,(struct sockaddr*)&client_address, &length);
        //printf("%s\n",message);
        message[n] = '\0';
        //printf("%s\n",message);

        printf("Request from the local server is %s\n",message);
        char domain_name[100];
        
        domain_name[0] = message[n-4];
        domain_name[1] = message[n-3];
        domain_name[2] = message[n-2];
        domain_name[3] = message[n-1];
        domain_name[4] = '\0';
        //printf("%c %c %c %c\n",message[n-4],message[n-3],message[n-2],message[n-1]);
        //printf("%s\n",domain_name);
        int index = INT_MAX;
        for(int k = 0;k<3;k++){
            if(strcmp(arr[k].domain_names,domain_name) == 0){
                index = k;
                break;
            }
        }
        if(index == INT_MAX){
            printf("Domain name is not available\n");
            printf("%s\n",message);
            sendto(tld_socket,message,strlen(message),0,(struct sockaddr*)&tldserver_address, length);
            if(strcmp(message,"exit") == 0){
                break;
            }
            n = recvfrom(tld_socket,ip_address,1000,0,(struct sockaddr*)&tldserver_address,&length);
            ip_address[n] = '\0';
            //printf("Hello World\n");
            printf("IP address of %s is %s\n",message,ip_address);
            close(tld_socket);
            sendto(root_socket,ip_address,strlen(ip_address),0,(struct sockaddr*) &client_address,length);
            //printf("%s\n",domain_name);
        }
        else{
            char domain_ip[1000];
            strcpy(domain_ip,arr[index].ip);

            printf("IP address of Domain name %s is %s\n",domain_name,domain_ip);


            tld_socket = socket(AF_INET,SOCK_DGRAM, 0);
            if(tld_socket < 0){
                printf("Error in tld socket Creation\n");
                return -1;
            }

            tldserver_address.sin_family = AF_INET;
            tldserver_address.sin_port = htons(4042);
            tldserver_address.sin_addr.s_addr = INADDR_ANY;

            sendto(tld_socket,message,strlen(message),0,(struct sockaddr*)&tldserver_address, length);
            // if(strcmp(message,"exit") == 0){
            //     break;
            // }
            n = recvfrom(tld_socket,ip_address,1000,0,(struct sockaddr*)&tldserver_address,&length);
            ip_address[n] = '\0';
            //printf("Hello World\n");
            printf("IP address of %s is %s\n",message,ip_address);
            close(tld_socket);
            sendto(root_socket,ip_address,strlen(ip_address),0,(struct sockaddr*) &client_address,length);
        }
    }
    close(root_socket);
    return 0;
}