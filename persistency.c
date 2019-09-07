#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

int fsize(FILE *fp)
{
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); 
    return sz;
}

FILE *openPersistencyFile(char *mode)
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

void clearPersistencyFile()
{
    FILE *fptr = openPersistencyFile("w");
    fclose(fptr);
}

void writePersistencyFile(char *name, int size, char *date)
{
    FILE *fptr = openPersistencyFile("a");
    fprintf(fptr,"name:%s,", name);
    fprintf(fptr,"size:%d,", size);
    fprintf(fptr,"datemodified:%s", date);

    fclose(fptr);

    printf("\nFile name is: %s\n", name);
    printf("File size is: %d \n", size);
    printf("File modified data is: %s", date);
}

void readPersistency()
{
    FILE *f = openPersistencyFile("r");
    char *persistencyLine; 

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);
    printf("\nReading from persistency:\n%s:", string);
    free(string);
    //int c;
    //while ((c = getc(fptr)) != '#' && c != EOF){
    //    putchar(c);
    //}

    //fscanf(fptr,"%[^\n]", persistencyLine);
    //printf("\nReading from persistency: %s\n", persistencyLine);

    fclose(f);
}

void writePersistency(char *directoryName)
{
    DIR *directory;
    struct dirent *dir;
    struct stat filestat;
    directory = opendir(directoryName);
    if (directory)
    {
        clearPersistencyFile();
        while ((dir = readdir(directory)) != NULL)
        {
            stat(dir->d_name,&filestat);
            FILE *fp;
            fp=fopen(dir->d_name, "r");
            writePersistencyFile(dir->d_name, fsize(fp), ctime(&filestat.st_atime));
        }
        closedir(directory);
    }
}