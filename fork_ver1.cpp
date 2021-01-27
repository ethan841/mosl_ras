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
    //char *dcap_args[] = {"/home/mobileosdcaps/SGX/SGXDataCenterAttestationPrimitives/SampleCode/QuoteGenerationSample/app", NULL};
    char *dcap_args[] = {"/home/mobileosdcaps/SGX/mosl/SGXDataCenterAttestationPrimitives/SampleCode/QuoteGenerationSample/app 127.0.0.1 5500", NULL};
    char *epid_args[] = {"/home/mobileosdcaps/SGX/sgx-ra-sample/run-client", NULL};
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
    
    return 0;
}

