#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in ttp_addr;
    struct sockaddr_in ttp_addr_in;
    socklen_t ttp_addr_size;
    socklen_t ttp_addr_in_size;
    char recv_buf[BUF_SIZE];
    int msg_len;

    char message[] = "migration result";

    if(argc != 3)
    {
        printf("Usage: %s <IP> <port> <incoming_port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&ttp_addr, 0, sizeof(ttp_addr));
    ttp_addr_in.sin_family = AF_INET;
    ttp_addr_in.sin_addr.s_addr = inet_addr(INADDR_ANY);
    ttp_addr_in.sin_port = htons(atoi(argv[3]));

    if(bind(sock, struct sockaddr*)&ttp_addr_in, sizeof(ttp_addr_in)) == -1)
        error_handling("bind() error");

    if(listen(sock, 5) == -1)
        error_handling("listen() error");    

    ttp_addr_in_size = sizeof(ttp_addr_in);
    sock = accept(sock, (struct sockaddr*)&ttp_addr_in, &ttp_addr_in_size);
    
    if(sock == -1)
        error_handling("accept() error");
    
    recv(sock, recv_buf, sizeof(recv_buf), 0);
    printf("%s\n", recv_buf);
    //make Attestation result
    printf("Send Attestation result\n", recv_buf);

    memset(&ttp_addr, 0, sizeof(ttp_addr));
    ttp_addr.sin_family = AF_INET;
    ttp_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ttp_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&ttp_addr, sizeof(ttp_addr)) == -1)
        error_handling("connect() error");

    send(sock, message, sizeof(message), 0);

    return 0;
}