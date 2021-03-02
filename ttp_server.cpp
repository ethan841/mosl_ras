#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <curl/curl.h>
#include <string>

#define BUF_SIZE 1024
#define ATT_PORT 4500

using namespace std;

void error_handling(char* message);
size_t writeFunction(void *ptr, size_t size, size_t nmemb, string* data)
{
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

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

    string response_string;
    string header_string;

    auto curl = curl_easy_init();
    auto curl2 = curl_easy_init();

    if(curl)
    {
	printf("DCAP attestation Server connect ready\n");
	curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:8081/sgx/certification/v2/verification");
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	//curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYPEER, 0L);
	//curl_easy_setopt(curl, CURLOPT_USERPWD, "user:pass");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
    }

    if(curl2)
    {
	printf("EPID attestation Server connect ready\n");
	curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:8081/sgx/certification/v2/verification");
	//curl_easy_setopt(curl2, CURLOPT_URL, "localhost:4000/epid_att_start");
	curl_easy_setopt(curl2, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYPEER, 0L);
	//curl_easy_setopt(curl, CURLOPT_USERPWD, "user:pass");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
	curl_easy_setopt(curl2, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(curl2, CURLOPT_TCP_KEEPALIVE, 1L);

	curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, writeFunction);
	curl_easy_setopt(curl2, CURLOPT_WRITEDATA, &response_string);
	curl_easy_setopt(curl2, CURLOPT_HEADERDATA, &header_string);
    }




    //fixed target ip -> need change
    if(argc != 7)
    {
        printf("Usage:%s <User init incoming port> <target IP> <target Port> <source IP> <source Port> <dcap : 1, EPID : 2>\n", argv[0]);
        exit(1);
    }

    printf("TTP Server process Sample. - Need more Development\n\n");

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

    printf("Step 1. listening user's migration request\n\n");
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
    
    printf("Migration Request received -> "); 
    printf("%s\n", recv_buf); 
    // print source platform message + information  

//----------------------------2. Sending remote attestation request to target host----------------------//
// need to check DCAP or EPID Attestation - Based on Platform info DB 

    if(atoi(argv[6]) == 1)
    {
    	curl_easy_perform(curl);
    	curl_easy_cleanup(curl);
    	curl = NULL;
    }
    else
    {
    	curl_easy_perform(curl2);
    	curl_easy_cleanup(curl2);
    	curl2 = NULL;
    }

    printf("    Send Attestation Verification request to verification server. (TTP -> Att Server) \n\n");

    sleep(1);

    memset(&target_host_addr, 0, sizeof(target_host_addr));
    target_host_addr.sin_family = AF_INET;
    target_host_addr.sin_addr.s_addr = inet_addr(argv[2]);
    target_host_addr.sin_port = htons(atoi(argv[3]));

    if(connect(target_sock, (struct sockaddr*)&target_host_addr, sizeof(target_host_addr)) == -1)
        error_handling("target connect() error");
    
    printf("Step 2. Send Attestation request to target host\n\n");
    send(target_sock, message, sizeof(message), 0);

//----------------------------3. listening remote attestation result-------------------------------------//

    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    remote_addr.sin_port = htons(ATT_PORT); 

    if(bind(remote_sock, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) == -1)
        error_handling("bind() error");

    printf("Step 3. Waiting Attestation result from Attestation Server\n\n");
    if(listen(remote_sock, 5) == -1)
        error_handling("listen() error");
    
    remote_addr_size = sizeof(remote_addr);
    remote_sock = accept(remote_sock, (struct sockaddr*)&remote_addr, &remote_addr_size);

    if(remote_sock == -1)
        error_handling("accept() error");

    //write(source_host_sock, message, sizeof(message));
    recv(remote_sock, recv_buf2, sizeof(recv_buf2), 0);
    
    printf("Remote Attestation Result received\n\n"); 
    printf("result : %s\n\n", recv_buf2); 

//---------------------------4. Send migration start signal to source host------------------------------//

    memset(&source_host_addr, 0, sizeof(source_host_addr));
    source_host_addr.sin_family = AF_INET;
    source_host_addr.sin_addr.s_addr = inet_addr(argv[4]);
    source_host_addr.sin_port = htons(atoi(argv[5]));

    //sleep(1);
    if(connect(source_sock, (struct sockaddr*)&source_host_addr, sizeof(source_host_addr)) == -1)
        error_handling("target connect() error");
    
    printf("Step 4. Send migration signal to source host\n\n");
    send(source_sock, message2, sizeof(message2), 0);

//-------------------------------------------------Repeat----------------------------------------------//

    printf("end of TTP server Sample.\n");

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
