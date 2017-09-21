#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define EDGE_UDP_PORT "24720" 
#define OR_UDP_PORT "21720"

void error(char *msg) 
{
	perror(msg);
	exit(0);
}

//This function is used to send the results to Edge server as a client with server port number: 24720
void Send_to_Edge(char tempy[], char tempz[], char ans[], int count) 
{
  	int sockfd;  //Define some characters firstly
  	struct addrinfo hints, *servinfo, *p;
  	int rv;
  	int numbytes;
  	memset(&hints, 0, sizeof hints);
 	hints.ai_family = AF_UNSPEC;
 	hints.ai_socktype = SOCK_DGRAM;
 	if ((rv = getaddrinfo("localhost", EDGE_UDP_PORT, &hints, &servinfo)) != 0) 
	{
    		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
  	}
  	for(p = servinfo; p != NULL; p = p->ai_next)   //Input the parameters
	{
    		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
		{
      			perror("talker: socket");
      			continue;
    		}
    		break;
  	}
  	if (p == NULL) 
	{
    		fprintf(stderr, "talker: failed to bind socket\n");
  	}

	//We don't even need to bind the socket before sending because it is UDP protocol
  	if ((numbytes = sendto(sockfd, ans, strlen(ans), 0, p->ai_addr, p->ai_addrlen)) == -1) 
	{
    		perror("talker: sendto");
    		exit(1);
  	}
  	freeaddrinfo(servinfo);
  	printf("%s or %s = %s  (This is the result of No.%d line received from edge server.)\n", tempy, tempz, ans, count);  //Print out the calculation result and line number
  	close(sockfd);
}

int main ()
{
	int sock, length, fromlen, n;  //Define characters firstly
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buf[300];
	memset(buf,0,sizeof(buf));

	//These characters are used to cut the line string into three parts("and" or "or" part, first binary number part, and second binary part)
	char x[5];  //x stands for the "and" or "or" part
	char y[11];  //y stands for the first binary number part
	char z[11];  //z stands for the second binary number part
	int a=0;
	int j=0,m=0,k=0; //j is the parameter of x, m is the parameter of y, and k is the parameter of k
	int count=1;  //count will help us know how many lines this OR server has processed
	memset(x,0,sizeof(x));
	memset(y,0,sizeof(y));
	memset(z,0,sizeof(z));

	char tempy[11];
	char tempz[11];
	char ans[11];
	memset(tempy,0,sizeof(tempy));
	memset(tempz,0,sizeof(tempz));
	memset(ans,0,sizeof(ans));
	int w,v;  //w and v stand for the difference between strlen(y) and strlen(z) to help us pad "0"
	int d=0;

	//Configure the UDP connection with edge server in the next part
	sock = socket(AF_INET, SOCK_DGRAM, 0);  //Create a new socket
	if (sock < 0)
	{
		error("Opening socket.");
	}
	length = sizeof(server);
	bzero(&server, length);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(OR_UDP_PORT));
	if (bind(sock, (struct sockaddr *)&server, length) < 0)  //Bind the socket
	{
		error("Binding");
	}
	fromlen = sizeof(struct sockaddr_in);
	printf("The Server OR is up and running using UDP on port %s.\n", OR_UDP_PORT);
	printf("The Server OR is ready to receive lines from the edge server for OR computation. The computation results are:\n");

	//Transmit frames and analysis data in the next part
	while (1)  //Server is a non-stop device, so it is always trying to get the frames with correct IP address through the port number
	{
		n = recvfrom(sock, buf, 300, 0, (struct sockaddr *)&from, &fromlen);  //Receive the frame and save the data into buf
		if (n < 0)
		{
			error("Receive\n");
		}
		for (a;a<sizeof(buf);a++)  //All the data before the first comma is "and" or "or" part, so we save them into x[]
		{
			if (buf[a]!=',')
			{
				x[j] = buf[a];
				j++;
			}	
			else if (buf[a]==',')
			break;
		}
		x[j] = '\0';
		a++;
		for (a;a<sizeof(buf);a++)  //All the data between the first comma and the second comma is the first binary number part, so we save them into y[]
		{
			if (buf[a]!=',')
			{
				y[m] = buf[a];
				m++;
			}
			else if (buf[a]==',')
			break;
		}
		y[m] = '\0';
		a++;
		for (a;a<sizeof(buf);a++)  //All the data between the second comma and the '\n' is the second binary number part, so we save them into z[]
		{
			if (buf[a]!='\n')
			{
				z[k] = buf[a];
				k++;
			}
			else if (buf[a]=='\n')
			break;
		}
		z[k] = '\0';
		if (strlen(y)<strlen(z))  //If the string length of y is less than string length of z, then we pad some "0"s before y string to help us calculate the result
		{
			w=(strlen(z)-strlen(y));
			strcpy (tempy,y);
			strcpy (tempz,z);
			for(d;d<strlen(z);d++)
			{
				if(d<w)
				{
					y[d] = '0';
					ans[d] = y[d]|z[d];
				}
				else
				{
					y[d] = tempy[(d-w)];
					ans[d] = y[d]|z[d];
				}
			}
			ans[d] = '\0';
		}
		else if (strlen(y)>strlen(z))  //If the string length of y is larger than string length of z, then we pad some "0"s before z string to help us calculate the result
		{
			v=(strlen(y)-strlen(z));
			strcpy (tempy,y);
			strcpy (tempz,z);
			for(d;d<strlen(y);d++)
			{
				if(d<v)
				{
					z[d] = '0';
					ans[d] = y[d]|z[d];
				}
				else
				{
					z[d] = tempz[(d-v)];
					ans[d] = y[d]|z[d];
				}
			}
			ans[d] = '\0';
		}	
		else if (strlen(y)==strlen(z))  //If the string length of y is equal to the string length of z, then we don't need to pad any "0"
		{
			strcpy (tempy,y);
			strcpy (tempz,z);
			for(d;d<strlen(y);d++)
			{
				ans[d] = y[d]|z[d];
			}
			ans[d] = '\0';
		}
		if (strlen(ans)<2)  //If the result is all 0's for every single bits, then we save one "0" at the beginning
		{
			ans[0] = '0';
			ans[1] = '\0';
		}
		Send_to_Edge(tempy, tempz, ans, count);
		memset(x,0,sizeof(x));  //Clean all the parameters in the while loop to get ready for the next calculation
		memset(y,0,sizeof(y));
		memset(z,0,sizeof(z));
		memset(tempy,0,sizeof(tempy));
		memset(tempz,0,sizeof(tempz));
		memset(ans,0,sizeof(ans));
		a=0;
		j=0;
		m=0;
		k=0;
		d=0;
		count++;
	}
	return 0;
}
