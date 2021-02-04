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
    //char message[BUF_SIZE];
    int msg_len;

    char message[] = "User's migration request";

    if(argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&ttp_addr, 0, sizeof(ttp_addr));
    ttp_addr.sin_family = AF_INET;
    ttp_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ttp_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&ttp_addr, sizeof(ttp_addr)) == -1)
        error_handling("connect() error");
    
    send(sock, message, sizeof(message), 0);

    return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n', stderr);
	exit(1);
}
