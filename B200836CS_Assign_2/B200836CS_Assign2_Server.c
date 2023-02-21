#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<time.h>

int accepting_requests(int socket){
    struct sockaddr_storage client_address;
    int addr_size = sizeof(struct sockaddr_in);
    int client_socket = accept(socket, (struct sockaddr*)&client_address,&addr_size);
    if(client_socket < 0){
        printf("Error in accepting the request\n");
    }
    else{
        printf("Accepted Request \n");
    }
    return client_socket;
}
void broadcast_message(int i,char message[1000],char names[12][1000],int client_sockets[12],int client_num){
    //memset(message,'\0',strlen(message));
    printf("%s\n",message);
    for(int j = 0;j<client_num;j++){
        if(j != i){
            send(client_sockets[j],(void *)message,strlen(message) + 1,0);
        }
    }
}
int recv_message(int i,char names[12][1000],int client_sockets[12],int client_num){
    char message[1000];
    char temp[1000];
    strcpy(temp,names[i]);
    strcat(temp ,": ");
    int recv_status = recv(client_sockets[i],(void *)message,sizeof message,0);
    //memset(message,'\0',strlen(message));
    if(recv_status <= 0 || strcmp(message, "/quit") == 0){
        return 0;
    } 
    strcat(temp,message);
    broadcast_message(i,temp,names,client_sockets,client_num);
}
// my_addr --> server_address
int main(){
    int server_socket;
    printf("Creating a socket\n");
    server_socket = socket(AF_INET,SOCK_STREAM, 0);

    printf("Binding the socket to port \n");
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);
    server_address.sin_addr.s_addr = INADDR_ANY;
    int bind_status = bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address));
    if(bind_status < 0){
        printf("Binding failed");
    }

    printf("Listening to incoming request\n");
    listen(server_socket,12);
    fd_set current_sockets,ready_sockets;
    FD_ZERO(&current_sockets);
    FD_SET(server_socket,&current_sockets);

    int client_sockets[12];
    int names_given[12];
    for(int i = 0;i<10;i++){
        client_sockets[i] = 0;
        names_given[i] = 0;
    }
    
    int client_num = 0;
    char names[12][1000];
    strcpy(names[0],"Melvin");
    strcpy(names[1],"uday");
    strcpy(names[2],"ram");
    strcpy(names[3],"sai");
    strcpy(names[4],"raja");
    strcpy(names[5],"raj");
    strcpy(names[6],"rahul");
    strcpy(names[7],"rajesh");
    strcpy(names[8],"rakesh");
    strcpy(names[9],"sumanth");
    strcpy(names[10],"pranay");

    while(1){
        ready_sockets = current_sockets;
        select(12,&ready_sockets,NULL,NULL,NULL);

        if(FD_ISSET(server_socket, &ready_sockets)){
            int temp = accepting_requests(server_socket);
            FD_SET(temp,&current_sockets);
            client_sockets[client_num] = temp;
            client_num++;
        }

        ready_sockets = current_sockets;
        select(12,&ready_sockets,NULL,NULL,NULL);
        for(int i = 0;i<client_num;i++){
            if(FD_ISSET(client_sockets[i],&ready_sockets)){
                if(!recv_message(i,names,client_sockets,client_num)){
                    client_sockets[i] = 0;
                    names_given[i] = 0;
                    strcat(names[i]," left the chat room");
                    broadcast_message(i,names[i],names,client_sockets,client_num);
                    close(client_sockets[i]);
                }
            }
        }
    }
    close(server_socket);
    printf("Connection Closed\n");
    return 0;
}