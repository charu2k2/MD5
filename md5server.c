#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "md5.h" //"" means search in local directory,else will search in /user/include
int main(int argc, char *argv[])
{
    unsigned short srv_port; /* port server binds to                */
    char buf[40];               /* buffer for sending & receiving data */
    struct sockaddr_in client_sockaddr; /* client address information          */
    struct sockaddr_in server_sockaddr; /* server address information          */
    int server_socket;                     /* socket for accepting connections    */
    int client_socket;                    /* socket connected to client          */
    int namelen;               /* length of client name               */
    char inString[30];

    /*
     * Check arguments. Should be only one: the port number to bind to.
     */

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(1);
    }

    /*
     * First argument should be the port.
     */
    srv_port = (unsigned short) atoi(argv[1]);

    /*
     * Get a socket for accepting connections.
     */
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket()");
        exit(2);
    }

    /*
     * Bind the socket to the server address.
     */
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port   = htons(srv_port);
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) < 0)
    {
        printf("Bind()");
        exit(3);
    }

    /*
     * Listen for connections. Specify the backlog as 1.
     */
    if (listen(server_socket, 1) != 0)
    {
        printf("Listen()");
        exit(4);
    }

    /*
     * Accept a connection.
     */
    namelen = sizeof(client_sockaddr);
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_sockaddr, &namelen)) == -1)
    {
        printf("Accept()");
        exit(5);
    }

    //md5 conversion
    if (recv(client_socket, inString, sizeof(inString), 0) == -1)
    	{
        	printf("Recv()");
        	exit(6);
    	}

    MD5_CTX mdContext;
    unsigned int len = strlen (inString);
    MD5Init (&mdContext);
    MD5Update (&mdContext, inString, len);
    MD5Final (&mdContext);
    //MDPrint (&mdContext);
    send(client_socket, mdContext.digest, 16,0);

    //printf (" \"%s\"\n\n", inString);

    close(client_socket);
    close(server_socket);

    printf("Server ended successfully\n");
    exit(0);
}
