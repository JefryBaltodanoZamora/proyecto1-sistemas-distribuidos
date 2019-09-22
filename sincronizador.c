#include <stdio.h>
#include <zhelpers.h>
#include <zmq.h>
#include <czmq.h>
#include <string.h>
#include <unistd.h>
#include "persistency.c"
#define CHUNK_SIZE  250000

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

            //  Socket to talk to clients
            void *context = zmq_ctx_new ();
            void *serverSocket = zmq_socket (context, ZMQ_ROUTER);
            int rc = zmq_bind (serverSocket, "tcp://*:8889");
            assert (rc == 0);

            FILE *file = fopen ("test2.json", "r");
            assert (file);

            while (1) {
                zframe_t *identity = zframe_recv (serverSocket);
                if (!identity)
                    break; 

                //  Second frame is "fetch" command
                char *command = zstr_recv (serverSocket);
                assert (streq (command, "fetch"));
                free (command);
                printf("Waiting...");
                while (1) {
                    printf("sending file...");
                    byte *data = malloc (CHUNK_SIZE);
                    assert (data);
                    size_t size = fread (data, 1, CHUNK_SIZE, file);
                    zframe_t *chunk = zframe_new (data, size);
                    zframe_send (&identity, serverSocket, ZFRAME_REUSE + ZFRAME_MORE);
                    zframe_send (&chunk, serverSocket, 0);
                    if (size == 0)
                        break;          //  Always end with a zero-size frame
                }
            }
            zmq_close (serverSocket);
            zmq_ctx_destroy (context);
        }
        else
        {
            runningMode = 2;
            printf("Running in 'Client' mode. \n\n");
            printf("The directory's name is: %s \n", directoryName); 
            printf("The IP address is: %s \n", ipAddress);
            printf ("Connecting to remote server… \n");

            void *context = zmq_ctx_new ();
            void *clientSocket = zmq_socket (context, ZMQ_DEALER);
            zmq_connect (clientSocket, "tcp://localhost:8889");

            s_send(clientSocket, "fetch");
            size_t total = 0;       //  Total bytes received
            size_t chunks = 0;      //  Total chunks received

            while (1) {
                //  Ask for next chunk
                zstr_sendm (clientSocket, "fetch");
                zstr_sendfm (clientSocket, "%ld", total);
                zstr_sendf (clientSocket, "%d", CHUNK_SIZE);

                zframe_t *frame = zframe_recv (clientSocket);
                if (!frame)
                    break;              //  Shutting down, quit
                chunks++;
                size_t size = zframe_size (frame);
                zframe_destroy (&frame);
                total += size;
                if (size == 0)
                    break;              //  Whole file received
            }
            printf ("%zd chunks received, %zd bytes\n", chunks, total);
            zstr_send (NULL, "OK");
            zmq_close (clientSocket);
            zmq_ctx_destroy (context);
        }
    }

    readPersistency(); //it goes to persistency.c

    writePersistency(directoryName); //it goes to persistency.c

   return 0;
}