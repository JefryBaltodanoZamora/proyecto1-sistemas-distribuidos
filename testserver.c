#include "wcomm.h"


void runclient(char *ip, char *fpath);
void runserver();

WComm w;

void main(int argc, char *argv[])
{
	if(argc==1)runserver();
	else runclient(argv[1],argv[2]);
}


void runserver()
{
	// Start Server Daemon
	w.startServer(27015);
	printf("Server Started........\n");
    while (TRUE) {
		// Wait until a client connects
		w.waitForClient();
		printf("Client Connected......\n");

		// Work with client
		while(TRUE)
		{
			char rec[50] = "";
			w.recvData(rec,32);w.sendData("OK");

			if(strcmp(rec,"FileSend")==0)
			{
				char fname[32] ="";
				w.fileReceive(fname);
				printf("File Received.........\n");
			}
			if(strcmp(rec,"EndConnection")==0)break;
			printf("Connection Ended......\n");
		}
		// Disconnect client
		w.closeConnection();
	}
}

void runclient(char *ip, char *fpath)
{
	char rec[32] = "";
	// Connect To Server
	w.connectServer(ip,27015);
	printf("Connected to server...\n");
	// Sending File
	w.sendData("FileSend");	w.recvData(rec,32);
	w.fileSend(fpath);
	printf("File Sent.............\n");
	// Send Close Connection Signal
	w.sendData("EndConnection");w.recvData(rec,32);
	printf("Connection ended......\n");
}