#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("\nYou need to provide required arguments."); 
        printf("\n[Required] Arg#1: Directory's name."); 
        printf("\n[Optional] Arg#2: IP address.\n"); 
        return 0;
    }
    if (argc > 3)
    {
        printf("\nYou provided more arguments than expected."); 
        printf("\n[Required] Arg#1: Directory's name."); 
        printf("\n[Optional] Arg#2: IP address.\n"); 
        return 0;
    }
    char *directoryName;
    char *ipAddress;
    int runningMode = 0; // 0=Undefined | 1=ServerMode | 2=ClientMode
    if (argc == 2 || argc ==3) 
    {
        directoryName = argv[1];
        ipAddress = argv[2]; 
        
        if (ipAddress != NULL)
        {
            runningMode = 1;
            printf("Running in 'Server' mode. \n");
            printf("The directory's name is: %s \n", directoryName); 
            printf("The IP address is: %s \n", ipAddress);
        }
        else
        {
            runningMode = 2;
            printf("Running in 'Client' mode. \n");
            printf("The directory's name is: %s \n", directoryName); 
        }
    }
   return 0;
}