#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 1024

using namespace std;

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
    char recv_buf2[BUF_SIZE];
    int msg_len;

    pid_t pid;
    pid_t pid2;

    char message[] = "attestation result check request";

    if(argc != 3)
    {
        printf("Usage: %s <attestation port> <mig port>\n", argv[0]);
        exit(1);
    }

    //sock = socket(PF_INET, SOCK_STREAM, 0);
    //if(sock == -1)
    //    error_handling("socket() error");

    //sock2 = socket(PF_INET, SOCK_STREAM, 0);
    //if(sock == -1)
    //    error_handling("socket() error");

    printf("Target host daemon sample\n");
    printf("TTP remote attestaton port : %s\n", argv[1]);
    printf("Migration signal port : %s\n", argv[2]);

    pid = fork();

    printf("Fork Complete\n");

//----------------------------------------process attestation request--------------------------------------//

    if(pid != 0)
    {
        //while(1)
        //{
    	    sock = socket(PF_INET, SOCK_STREAM, 0);
    	    if(sock == -1)
                error_handling("socket() error");
            
	        memset(&ttp_addr_in, 0, sizeof(ttp_addr_in));
            ttp_addr_in.sin_family = AF_INET;
            ttp_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
            ttp_addr_in.sin_port = htons(atoi(argv[1]));

            if(bind(sock, (struct sockaddr*)&ttp_addr_in, sizeof(ttp_addr_in)) == -1)
                error_handling("bind() error");

            printf("Listening remote attestation request\n");
            if(listen(sock, 5) == -1)
                error_handling("listen() error");    

            ttp_addr_in_size = sizeof(ttp_addr_in);
            sock = accept(sock, (struct sockaddr*)&ttp_addr_in, &ttp_addr_in_size);

            if(sock == -1)
                error_handling("accept() error");

            recv(sock, recv_buf, sizeof(recv_buf), 0);
            printf("%s\n", recv_buf);
            
	        shutdown(sock, SHUT_RDWR);
	        close(sock);
	        //Doing remote attestation

            //Absolute Path
            //char *dcap_args[] = {"/home/mobileosdcaps/SGX/SGXDataCenterAttestationPrimitives/SampleCode/QuoteGenerationSample/app", NULL};
            char *dcap_args[] = {"/home/mobileosdcaps/SGX/mosl/SGXDataCenterAttestationPrimitives/SampleCode/QuoteGenerationSample/app 127.0.0.1 5500", NULL};
            char *epid_args[] = {"/home/mobileosdcaps/SGX/sgx-ra-sample/run-client", NULL};
            int ret;

            pid2 = fork();

            if(pid2 != 0)//parent
            {
                return 0;
            }
            else //child
            {
                //execute dcap remote attestation sample
                ret = system(dcap_args[0]);

                if(ret != 0)
                {
                    cout << "Error in DCAP Remote Attestation Process" << endl;
                    //execute epid remote attestation sample
                    ret = execv("/home/mobileosdcaps/SGX/sgx-ra-sample/run-client", epid_args);
                    //ret = execv("/home/mobileosdcaps/SGX/SGXDataCenterAttestationPrimitives/SampleCode/QuoteGenerationSample/app", dcap_args);
                    //ret = system(epid_args[0]);
                    if(ret != 0)
                    {
                        perror("execv");
                        exit(EXIT_FAILURE);
                    }
    
                }
                else
        	        cout << "DCAP Attestation END" << endl;
            }
        //}
    }
//-------------------------------------------process migration---------------------------------------------// 
    else
    {
        //while(1)
        //{
    	    sock2 = socket(PF_INET, SOCK_STREAM, 0);
    	    if(sock2 == -1)
                error_handling("socket() error");
            
	    memset(&ttp_addr, 0, sizeof(ttp_addr));
            ttp_addr.sin_family = AF_INET;
            ttp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            //ttp_addr.sin_addr.s_addr = inet_addr(argv[1]);
            ttp_addr.sin_port = htons(atoi(argv[2]));
            //ttp_addr.sin_port = htons(atoi(argv[2]));

            if(bind(sock2, (struct sockaddr*)&ttp_addr, sizeof(ttp_addr)) == -1)

            printf("Listening migration signal\n");
            if(listen(sock2, 5) == -1)
                error_handling("listen() error");    

            ttp_addr_size = sizeof(ttp_addr);
            sock2 = accept(sock2, (struct sockaddr*)&ttp_addr, &ttp_addr_size);

            if(sock2 == -1)
                error_handling("accept() error");

            recv(sock2, recv_buf2, sizeof(recv_buf), 0);
            printf("%s\n", recv_buf2);
            /*
            if(connect(sock2, (struct sockaddr*)&ttp_addr, sizeof(ttp_addr)) == -1)
                error_handling("connect() error");

            send(sock2, message, sizeof(message), 0);
            */

	    shutdown(sock2, SHUT_RDWR);
	    close(sock2);
        //}
    }
    return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
