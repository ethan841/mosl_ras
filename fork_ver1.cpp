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
    char *dcap_args[] = {"/home/mobileosdcaps/SGX/sgx-ra-sample/run-server", NULL};
    char *epid_args[] = {"/home/mobileosdcaps/SGX/sgx-ra-sample/run-server", NULL};
    int ret;

    pid = fork();

    if(pid != 0)//parent
    {
        return 0;
    }
    else //child
    {
        //execute dcap remote attestation sample
        ret = system(dcap_args[0]);

        if(ret != -1)
        {
            cout << "Error in DCAP Remote Attestation Process" << endl;
            //execute epid remote attestation sample
            ret = execv("/home/mobileosdcaps/SGX/sgx-ra-sample/run-server", epid_args);

            if(ret==-1)
            {
                perror("execv");
                exit(EXIT_FAILURE);
            }
        }
        else
            return 0;
    }
    
    return 0;
}
