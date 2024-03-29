// Minimal HTTP server in 0MQ
#include "zhelpers.h"

int main (void) {
  int rc;

  // Set-up our context and sockets
  void *ctx = zmq_ctx_new ();
  assert (ctx);

  // Start our server listening
  void *server = zmq_socket (ctx, ZMQ_STREAM);
  zmq_bind (server, "tcp://*:8080");
  assert (server);
  uint8_t id[256];
  size_t id_size = 256;

  while (1) {
     // Get HTTP request;
     // first frame has ID, the next the request.
     id_size = zmq_recv (server, id, 256, 0);

     // Get HTTP request
     char *request = s_recv(server);
     free (request);
                
     request = s_recv(server);
     free (request);
                
     request = s_recv(server);
     
     char *url = strtok(request," ");
     url = strtok(NULL," ");

     char filename[256];
     
     strcpy(filename, url);
     
     free (request);
     
     char * line = NULL;
     ssize_t read;
     size_t len = 0;
     
     FILE *fp = fopen(filename+1, "r");
     
     // define the response
     char http_response [] =
         "HTTP/1.0 200 OK\n"
         "Content-Length: 120\n"
         "Content-Type: text/html\n"
         "Connection: Closed\n"
         "\n"
         "<html><body><h1>Hello, World!</h1></body></html>\n";
    
     // start sending response
     zmq_send (server, id, id_size, ZMQ_SNDMORE);
     // Send the http response
     zmq_send (server, http_response,
       sizeof (http_response),
       ZMQ_SNDMORE
     );
     
     while ((read = getline(&line, &len, fp)) != -1) {
        printf("%s", line);
        //zmq_send (server, line,
          //read,ZMQ_SNDMORE);
     }
    

       // Send a zero to close connection to client
       zmq_send(server, id, id_size, ZMQ_SNDMORE);
       zmq_send(server, NULL, 0, ZMQ_SNDMORE);
    
  }

  rc = zmq_close (server);
  assert (rc == 0);

  rc = zmq_ctx_term (ctx);
  assert (rc == 0);

  return 0;
}
