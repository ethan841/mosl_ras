#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>

using namespace std;

int main(char argc, char *argv[])
{
    pid_t pid;

    //Absolute Path
    //const char *dcap_path[] = {};
    const char *epid_args[] = {"dd", "dd"};
    int ret;

    pid = fork();

    if(pid != 0)
    {
        //wait until dcap attestation end
    }
    else
    {
        ret = execv("epid path", epid_args);

        if(ret==-1)
        {
            perror("execv");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}