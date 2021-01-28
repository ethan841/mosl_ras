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
    int sock2;
    struct sockaddr_in ttp_addr;
    struct sockaddr_in ttp_addr_in;
    socklen_t ttp_addr_size;
    socklen_t ttp_addr_in_size;
    char recv_buf[BUF_SIZE];
    int msg_len;

    pid_t pid;

    char message[] = "attestation result check request";

    if(argc != 3)
    {
        printf("Usage: %s <ttp server port> <mig incoming port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    sock2 = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    printf("TTP remote attestaton port : %s\n", argv[3]);

    pid = fork();

//----------------------------------------process attestation request--------------------------------------//

    if(pid != 0)
    {
        while(1)
        {
            memset(&ttp_addr_in, 0, sizeof(ttp_addr_in));
            ttp_addr_in.sin_family = AF_INET;
            ttp_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
            ttp_addr_in.sin_port = htons(atoi(argv[3]));

            if(bind(sock, (struct sockaddr*)&ttp_addr_in, sizeof(ttp_addr_in)) == -1)
                error_handling("bind() error");

            if(listen(sock, 5) == -1)
                error_handling("listen() error");    

            ttp_addr_in_size = sizeof(ttp_addr_in);
            sock = accept(sock, (struct sockaddr*)&ttp_addr_in, &ttp_addr_in_size);

            if(sock == -1)
                error_handling("accept() error");

            recv(sock, recv_buf, sizeof(recv_buf), 0);
            printf("%s\n", recv_buf);
            //Doing remote attestation
        }
    }
//-------------------------------------------process migration---------------------------------------------// 
    else
    {
        while(1)
        {
            memset(&ttp_addr, 0, sizeof(ttp_addr));
            ttp_addr.sin_family = AF_INET;
            ttp_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
            //ttp_addr.sin_addr.s_addr = inet_addr(argv[1]);
            ttp_addr.sin_port = htons(atoi(argv[3]));
            //ttp_addr.sin_port = htons(atoi(argv[2]));

            if(bind(sock2, (struct sockaddr*)&ttp_addr, sizeof(ttp_addr)) == -1)

            if(listen(sock, 5) == -1)
                error_handling("listen() error");    

            ttp_addr_in_size = sizeof(ttp_addr_in);
            sock = accept(sock, (struct sockaddr*)&ttp_addr_in, &ttp_addr_in_size);

            if(sock == -1)
                error_handling("accept() error");

            recv(sock, recv_buf, sizeof(recv_buf), 0);
            printf("%s\n", recv_buf);
            /*
            if(connect(sock2, (struct sockaddr*)&ttp_addr, sizeof(ttp_addr)) == -1)
                error_handling("connect() error");

            send(sock2, message, sizeof(message), 0);
            */
        }
    }
    return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
