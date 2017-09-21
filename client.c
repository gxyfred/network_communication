#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define CLIENT_TCP_PORT "23720"  //Set the TCP port number to 23720.

void error(char *msg) 
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd,portno,n,a,b;  //Define characters firstly
	int count = 0;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	FILE *fp;
	char buffer[3000];
	char filename[100];
	char s[30];
	if (argc != 2)  //The input form of the client should be: ./client.out <input_file_name>. So it is expecting argc = 2.
	{
		fprintf(stderr, "Please use the form: ./client.out <input_file_name>\n");
		exit(0);
	}

	//Configure the TCP connection with edge server in the next part
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  //Create a new socket
	if (sockfd < 0)
	{
		error("Error opening socket.");
	}
	server = gethostbyname("localhost");  //Get the address through name
	if (server == NULL)
	{
		fprintf(stderr, "Error, no such host.\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));  //Input the characters into the socket we just created
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr,server->h_length);
	serv_addr.sin_port = htons(atoi(CLIENT_TCP_PORT));
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)  //Connect the client and edge server through TCP
	{
		error("Error connecting.");
	}
	printf("The client is up and running.\n");

	//Transmit frames in the next part
	fp = fopen(argv[1], "r");  //Open the file with the name of what we just input in the command line
	if (fp == NULL)
	{
		printf("Could not open file %s", filename);
		return 0;
	}
	memset(s,0,sizeof(s));
	while(fgets(s,30,fp) != NULL)  //Read the file line by line until the end of file
	{
		n = write(sockfd, s, strlen(s));  //Send what we get from the file to the edge server
		if (n < 0)
		{
			error("Error writing to socket.");
		}

		if (strlen(s)>3)  //If the line is not an empty line, count if line number plus one (Last line could be empty if the line before last line end with a '\n')
		{
			count++;
		}
		memset(s,0,sizeof(s));
	}	
	fclose(fp);  //Close the file
	printf("The client has successfully finished sending %d lines to the edge server.\n",count);  //Print out how many lines we just send to edge server
	bzero(buffer,3000);
	n = read(sockfd, buffer, 3000);  //Get the answer from the edge server
	if (n < 0)
	{
		error("Error reading from socket.");
	}
	printf("The client has successfully finished receiving all computation results from the edge server.\n");
	printf("The final computation result are:\n%s",buffer);  //Show the results from the edge server on the screen
	return 0;
}
