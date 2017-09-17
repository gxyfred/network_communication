#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define EDGE_UDP_PORT "24720" 
#define EDGE_TCP_PORT "23720" 
#define AND_UDP_PORT "22720"
#define OR_UDP_PORT "21720"

void error(char *msg)  //This function is copied from the allowed youtube video to help me print out error faster.
{
	perror(msg);
	exit(1);
}

//This function is used to receive all the UDP frames from Backend servers as a server with port number: 24720
char *Recv_UDP()  //Most part of this function is copied from Beej's book from section 6.3
{
	int sockfd;  //Define some characters firstly
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	static char buf[300];  //It is important here to make buf static, so that we can get the data of buf in main function
	socklen_t addr_len;

	memset(&hints, 0, sizeof hints);
	memset(buf, 0, sizeof(buf));

	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; 

	if ((rv = getaddrinfo(NULL, EDGE_UDP_PORT, &hints, &servinfo)) != 0)   //Port number here is 24720
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}

	for(p = servinfo; p != NULL; p = p->ai_next)  //Input the parameters and bind the socket
	{

		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
		{
      			perror("listener: socket");
      			continue;
    		}

    		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
		{
      			close(sockfd);
     			perror("listener: bind");
      			continue;
    		}
    	break;
  	}

  	if (p == NULL) 
	{
    		fprintf(stderr, "listener: failed to bind socket\n");
  	}

  	freeaddrinfo(servinfo);
  	addr_len = sizeof their_addr;

  	if ((numbytes = recvfrom(sockfd, buf, 299 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)   //Receive the frames from Backend server
  	{
  		perror("recvfrom");
  		exit(1);
  	}
  	buf[numbytes] = '\0';
  	close(sockfd);
	return buf;  //The return value is the array buf[]
}

int main()
{
	char buf_recv[300];  //Define characters firstly
	bzero(buf_recv,300);
	char ans[3000];
	bzero(ans,3000);
	int line_num_to_and=0;
	int line_num_to_or=0;

	//These parameters are used for the TCP connection with client
	int sockfd, newsockfd, portno, clilen;  
	char buffer[3000];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	//These parameters are used for cutting the data into lines
	char line[101][100];
	memset(line,0x0,sizeof(line));
	int i=0,a=0,b=0;
	int m=0;

	//These parameters are used to send the frames to Backend servers through UDP
	int sock, length, port;
	struct sockaddr_in server, from;
	struct hostent *hp;
	
	//Configure the TCP connection with edge server in the next part, and some of this function is copied from the allowed youtube video
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  //Create a new socket

	if (sockfd < 0)
	{
		error("Error opening socket.");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;  //Input the characters into the socket we just created
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(EDGE_TCP_PORT));   //Port number here is 23720

	printf("The edge server is up and running.\n");

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  //Bind the socket
	{
		error("Error on binding.");
	}

	//Create a child socket to help us save the data into buffer
	listen(sockfd, 10);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0)
	{
		error("Error on accept.");
	}

	bzero(buffer, 3000);

	n = read(newsockfd, buffer, 2999);

	if (n < 0)
	{
		error("Error reading from socket.");
	}
	
	//Cutting all the data in the buffer into line[][], which means I can get the every single line by using line[]
	for (i;i<strlen(buffer);i++)
	{
		if((buffer[i]!='\n')&&(buffer[i]!='\0'))  //Copy the data from buffer[] to line[][]
		{
			line[a][b]=buffer[i];
			b++;
		}
		else if (buffer[i]=='\n')  //When meeting '\n', start at the beginning of next line[]
		{
			a++;
			b=0;
		}
		else if (buffer[i]=='\0')  //When meeting '\0', break the for loop
		break;
	}

	if (strlen(line[(a-1)])<3)  //If the last line is an empty line, then ignore this line
		a--;

	printf("The edge server has received %d lines from the client using TCP over port 23720.\n",a);

	//The next "for" loop is the most important part of this project, that will send the data to the related backend server "line by line". That is send one line first, then get the result from the related backend server, then save the result into the answer string: ans[]. After doing all that, it start dealing with the second line.
	for (m;m<a;m++)  
	{
		if(line[m][0]=='a')  //Decide which backend server should receive this line. If this line[] starts with an 'a', then it should be delivered to the server AND
		{	
			//Configure the UDP connection with server AND in the next part, and some of this function is copied from the allowed youtube video
			sock = socket(AF_INET, SOCK_DGRAM, 0);  //Create a new socket
	
			if (sock < 0)
			{
				error("socket");
			}
		
			server.sin_family = AF_INET;
			hp = gethostbyname("localhost");  //Get the address through name
		
			if (hp == 0)
			{
				error("Unknown host");
			}
		
			bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);  //Input the characters into the socket we just created
			server.sin_port = htons(atoi(AND_UDP_PORT));  //Port number here is 22720
			length = sizeof(struct sockaddr_in);

			n = sendto(sock, line[m], strlen(line[m]), 0, (struct sockaddr *)&server, length);  //Send the frame to server AND
	
			if (n < 0)
			{
				error("Sendto");
			}

			line_num_to_and++;   //This is to count how many lines have been sent to Backend server AND		
			strcat(ans,Recv_UDP());   //Put the result received from Backend server AND into the array ans[]
			strcat(ans,"\n");			
			close(sock);

		}

		else if (line[m][0]=='o')  //Decide which backend server should receive this line. If this line[] starts with an 'o', then it should be delivered to the server OR
		{
			//Configure the UDP connection with server OR in the next part, and some of this function is copied from the allowed youtube video
			sock = socket(AF_INET, SOCK_DGRAM, 0);  //Create a new socket
			if (sock < 0)
			{
				error("socket");
			}
		
			server.sin_family = AF_INET;
			hp = gethostbyname("localhost");  //Get the address through name
			if (hp == 0)
			{
				error("Unknown host");
			}
		
			bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);  //Input the characters into the socket we just created
			server.sin_port = htons(atoi(OR_UDP_PORT));  //Port number here is 21720
			length = sizeof(struct sockaddr_in);

			n = sendto(sock, line[m], strlen(line[m]), 0, (struct sockaddr *)&server, length);  //Send the frame to server OR
			if (n < 0)
			{
				error("Sendto");
			}
			
			line_num_to_or++;   //This is to count how many lines have been sent to Backend server OR
			strcat(ans,Recv_UDP());  //Put the result received from Backend server OR into the array ans[]
			strcat(ans,"\n");
			close(sock);
		}
		else
		{
			error("File content error");
		}
	}

	printf("The edge has successfully sent %d lines to Backend-Server OR\n",line_num_to_or);
	printf("The edge has successfully sent %d lines to Backend-Server AND\n",line_num_to_and);
	printf("The edge server has received all the computation results from Backend-Server OR and Backend-Server AND using UDP over port %s.\n", EDGE_UDP_PORT);
	printf("The computation results are:\n%s", ans);
	printf("The edge server has successfully finished receiving all computation results from the Backend-Server OR and Backend-Server AND.\n");

	bzero(buffer, 3000);

	n = write(newsockfd, ans, sizeof(ans));  //Send the result back to client through TCP
	if (n < 0)
	{
		error("Error writing to socket.");
	}
	printf("The edge server has successfully finished sending all computation results to the client.\n");	
	
	return 0;
}
