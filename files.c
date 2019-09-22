void sendFileSize(char *filename, SOCKET socket) {
    char buffer2[256];
    sprintf(buffer2, "%i", getFileSize(filename));
    sendCommand(socket, buffer2);  // DATA SEND
}
 
int sendFile(char *filename, SOCKET socket) {
    long size = getFileSize(filename);
 
    if(size == -1 || size == 0) {
        //sendCommand(socket, "111");
        return -1;
    }
 
    FILE *file = fopen(filename, "r");
 
    char tmp[1024];
    sprintf(tmp, "%i", size);
    sendCommand(socket, tmp);
 
    char* buffer;
    buffer = new char[size];
 
    fread(buffer, sizeof(buffer[0]), size, file);
    send(socket, buffer, size, 0);
 
    printf("%s", buffer);
 
    fclose(file);
    return 0;
}
 
void recvFile(char *filename, SOCKET socket) {
    FILE *file = fopen(filename, "w");
 
    char* csize = recvResponse(socket); // DATA RECV
    int size = stringToInt(csize);
 
    char* buffer;
    buffer = new char[size];
 
    recv(socket, buffer, size, 0);
    fprintf(file, buffer);
      
    fclose(file);
}