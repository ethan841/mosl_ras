#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 1024
#define ATT_PORT 4500

void error_handling(char* message);

int main(int argc, char *argv[])
{
    int user_sock;
    int target_sock;
    int remote_sock;
    int source_sock;

    char recv_buf[BUF_SIZE];
    char recv_buf2[BUF_SIZE];

    struct sockaddr_in user_addr;
    struct sockaddr_in remote_addr;
    struct sockaddr_in source_host_addr;
    struct sockaddr_in target_host_addr;
    socklen_t user_addr_size;
    socklen_t remote_addr_size;
    socklen_t source_host_addr_size;
    socklen_t target_host_addr_size;

    char message[] = "Request Remote Attestation - TTP";
    char message2[] = "Start Migration - TTP";

    //fixed target ip -> need change
    if(argc != 6)
    {
        printf("Usage:%s <User init incoming port> <target IP> <target Port> <source IP> <source Port>\n", argv[0]);
        exit(1);
    }

    printf("TTP Server process Sample. - Need more Development\n");

    //user init sock
    user_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(user_sock == -1)
        error_handling("socket() error");    
    
    //target sock
    target_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(target_sock == -1)
        error_handling("socket() error");    

    remote_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(target_sock == -1)
        error_handling("socket() error");    
    
    source_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(target_sock == -1)
        error_handling("socket() error");

//------------------------------------1. listening user init message------------------------------------//

    memset(&user_addr, 0, sizeof(user_addr));
    user_addr.sin_family = AF_INET;
    user_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    user_addr.sin_port = htons(atoi(argv[1])); 

    if(bind(user_sock, (struct sockaddr*)&user_addr, sizeof(user_addr)) == -1)
        error_handling("bind() error");

    if(listen(user_sock, 5) == -1)
        error_handling("listen() error");
    
    user_addr_size = sizeof(user_addr);
    user_sock = accept(user_sock, (struct sockaddr*)&user_addr, &user_addr_size);

    if(user_sock == -1)
        error_handling("accept() error");

    //write(source_host_sock, message, sizeof(message));
    recv(user_sock, recv_buf, sizeof(recv_buf), 0);
    
    printf("Migration Request received\n"); 
    printf("%s\n", recv_buf); 
    // print source platform message + information  

//----------------------------2. Sending remote attestation request to target host----------------------//

    memset(&target_host_addr, 0, sizeof(target_host_addr));
    target_host_addr.sin_family = AF_INET;
    target_host_addr.sin_addr.s_addr = inet_addr(argv[2]);
    target_host_addr.sin_port = htons(atoi(argv[3]));

    if(connect(target_sock, (struct sockaddr*)&target_host_addr, sizeof(target_host_addr)) == -1)
        error_handling("target connect() error");
    
    send(target_sock, message, sizeof(message), 0);

//----------------------------3. listening remote attestation result-------------------------------------//
/*
    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    remote_addr.sin_port = htons(ATT_PORT); 

    if(bind(remote_sock, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) == -1)
        error_handling("bind() error");

    if(listen(remote_sock, 5) == -1)
        error_handling("listen() error");
    
    remote_addr_size = sizeof(remote_addr);
    remote_sock = accept(remote_sock, (struct sockaddr*)&remote_addr, &remote_addr_size);

    if(remote_sock == -1)
        error_handling("accept() error");

    //write(source_host_sock, message, sizeof(message));
    recv(remote_sock, recv_buf2, sizeof(recv_buf2), 0);
    
    printf("Remote Attestation Result received\n"); 
    printf("%s\n", recv_buf2); 
*/
//---------------------------4. Send migration start signal to source host------------------------------//

    memset(&source_host_addr, 0, sizeof(source_host_addr));
    source_host_addr.sin_family = AF_INET;
    source_host_addr.sin_addr.s_addr = inet_addr(argv[4]);
    source_host_addr.sin_port = htons(atoi(argv[5]));

    if(connect(source_sock, (struct sockaddr*)&source_host_addr, sizeof(source_host_addr)) == -1)
        error_handling("target connect() error");
    
    send(source_sock, message, sizeof(message2), 0);

//-------------------------------------------------Repeat----------------------------------------------//

    printf("end of Sample.\n");

    close(user_sock);
    close(remote_sock);
    close(source_sock);    
    close(target_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
