#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int main(char argc, char *argv[])
{
    pid_t pid;

    //Absolute Path
    //const char *dcap_path[] = {};
    char *epid_args[] = {"/home/mobileos7/SGX/sgx-ra-sample/run-server", NULL};
    int ret;

    pid = fork();

    if(pid != 0)
    {
        //wait until dcap attestation end
    }
    else
    {
        ret = execv("/home/mobileos7/SGX/sgx-ra-sample/run-server", epid_args);

        if(ret==-1)
        {
            perror("execv");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}
