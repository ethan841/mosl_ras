#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc, char *argv[])
{
    int ttp_sock;
    int ttp_sock2;
    int source_host_sock;
    int target_host_sock;

    char recv_buf[BUF_SIZE];

    struct sockaddr_in ttp_addr;
    struct sockaddr_in source_host_addr;
    struct sockaddr_in target_host_addr;
    socklen_t source_host_addr_size;
    socklen_t target_host_addr_size;

    char message[] = "Request Attestation";

    if(argc != 4)
    {
        printf("Usage:%s <init incoming port> <target IP> <target Port>\n", argv[0]);
        exit(1);
    }

    printf("TTP Server Version 0 \n");
    printf("2021/1/14 build\n");
    printf("Need more Development\n");

    ttp_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(ttp_sock == -1)
        error_handling("socket() error");    

    ttp_sock2 = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&ttp_addr, 0, sizeof(ttp_addr));
    ttp_addr.sin_family = AF_INET;
    ttp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ttp_addr.sin_port = htons(atoi(argv[1])); 

    if(bind(ttp_sock, (struct sockaddr*)&ttp_addr, sizeof(ttp_addr)) == -1)
        error_handling("bind() error");

    if(listen(ttp_sock, 5) == -1)
        error_handling("listen() error");
    
    source_host_addr_size = sizeof(source_host_addr);
    source_host_sock = accept(ttp_sock, (struct sockaddr*)&source_host_addr, &source_host_addr_size);

    if(source_host_sock == -1)
        error_handling("accept() error");

    //write(source_host_sock, message, sizeof(message));
    recv(source_host_sock, recv_buf, sizeof(recv_buf), 0);
    
    printf("Migration Request received\n"); 
    printf("%s\n", recv_buf); 
    // print source platform message + information  

    memset(&target_host_addr, 0, sizeof(target_host_addr));
    target_host_addr.sin_family = AF_INET;
    target_host_addr.sin_addr.s_addr = inet_addr(argv[2]);
    target_host_addr.sin_port = htons(atoi(argv[3]));

    if(connect(ttp_sock2, (struct sockaddr*)&target_host_addr, sizeof(target_host_addr)) == -1)
        error_handling("target connect() error");
    
    send(ttp_sock2, message, sizeof(message), 0);


    close(source_host_sock);
    close(ttp_sock);    
    close(ttp_sock2);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
