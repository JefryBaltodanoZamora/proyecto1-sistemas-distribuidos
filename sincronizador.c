#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int fsize(FILE *fp)
{
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); 
    return sz;
}

FILE *openPersistency(char *mode)
{
    FILE *fptr;
    fptr = fopen("persistency.txt", mode);
    if(fptr == NULL)
    {
        printf("Error while writing persistency - cannot open the file.");
        return NULL;
    }
    return fptr;
}

void cleanPersistency()
{
    FILE *fptr = openPersistency("w");
}

void writePersistency(char *name, int size, char *date)
{
    FILE *fptr = openPersistency("a");
    fprintf(fptr,"name:%s,", name);
    fprintf(fptr,"size:%d,", size);
    fprintf(fptr,"datemodified:%s", date);

    fclose(fptr);

    printf("\nFile name is: %s\n", name);
    printf("File size is: %d \n", size);
    printf("File modified data is: %s", date);
}

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
        
        if (ipAddress == NULL)
        {
            runningMode = 1;
            printf("Running in 'Server' mode. \n\n");
            printf("The directory's name is: %s \n", directoryName); 
        }
        else
        {
            runningMode = 2;
            printf("Running in 'Client' mode. \n\n");
            printf("The directory's name is: %s \n", directoryName); 
            printf("The IP address is: %s \n", ipAddress);
        }
    }

    DIR *d;
    struct dirent *dir;
    struct stat filestat;
    d = opendir(directoryName);
    if (d)
    {
        cleanPersistency();
        while ((dir = readdir(d)) != NULL)
        {
            stat(dir->d_name,&filestat);
            FILE *fp;
            fp=fopen(dir->d_name, "r");
            writePersistency(dir->d_name, fsize(fp), ctime(&filestat.st_atime));
        }
        closedir(d);
    }

   return 0;
}