#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc,char* argv[])
{
	unsigned short port;       /* port client will connect to i.e the server port number        */
	unsigned char buf[30];              /* data buffer for sending & receiving */
    	struct hostent *hostnm;    /* server host name information        */
    	struct sockaddr_in server; /* server address                      */
    	int s;                     /* client socket                       */
	
    /*
     * Check Arguments Passed. Should be hostname and port.
     */
    	if (argc != 3)
    	{
        	fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
    	 	 exit(1);
   	}
	hostnm = gethostbyname(argv[1]);
    	if (hostnm == (struct hostent *) 0)
    	{
        	fprintf(stderr, "Gethostbyname failed\n");
       		exit(2);
    	}

	port = (unsigned short) atoi(argv[2]); //2nd arugument is port

	server.sin_family      = AF_INET;
    	server.sin_port        = htons(port);
    	server.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);

    	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	printf("Socket()");
        	exit(3);
    	}

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) //connect to server
    	{
        	printf("Connect()");
        	exit(4);
    	}
	printf("Enter the string for which you want to find MD5:");
	scanf("%[^\n]%*c", buf);
	if (send(s, buf, sizeof(buf), 0) < 0)
   	 	{
        		printf("Send()");
        		exit(5);
    		}
	memset(buf, 0, sizeof(buf));
	if (recv(s, buf, sizeof(buf), 0) < 0)
    		{
        		printf("Recv()");
        		exit(6);
    		}
	int i;
	for (i = 0; i < 16; i++)
   	 	printf("%02x",buf[i]); //if buf is just char then many ffff will come to extract only last byte alone do buf[i]&0xff 
       printf("\n");	

	close(s);
    	printf("Client Ended Successfully\n");
    	exit(0);
	return 0;
}
