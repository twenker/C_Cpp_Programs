// Name of Author: Todd Wenker
// Course and Semester: CSE 434, Computer Networks and Fall 2017
// Project Part: 2
// Time Spent: 15 hours
// UDP Server that allows clients to read and write to files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// includes definitions faor data types used in sys calls
#include <sys/types.h>

// includes definition for sockets
#include <sys/socket.h>

// includes constants and structures for domain addresses
#include <netinet/in.h>

// includes the definition for the timeval stuct
#include <sys/time.h>

// includes flock() function
#include <sys/file.h>

// error report function
void error(char *msg)
{
	perror(msg);
	exit(1);
}

// reads client id and checks if server as received requests from that id before
int check_cliid(char *msg, int *cliid_list)
{
        int i = 0;
        int cliid = atoi(msg);

        printf("Received request from id %d\n",cliid);

        if(!cliid)
        {
                printf("Client ID is invalid");
                return 0;
        }

        while(cliid_list[i] != 0)
        {
                if(cliid_list[i] == cliid)
                {
                        printf("Client already made request.\n");
                        return 0;
                }
                ++i;
        }
        cliid_list[i] = cliid;
        printf("Client request accepted.\n");
        return 1;
}

// new server process that reads and writes to files
void recv_message(int portno, int cliid, struct sockaddr_in cli_addr)
{
	int sockfd, n, byte_count;
	char buffer[256] = "hello";
	char mode;
	struct sockaddr_in serv_addr;
	socklen_t clilen = sizeof(cli_addr);
	FILE * fp;
	// time value set to 3 seconds
	struct timeval timeout={3,0};

	// set socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	// zero out serv_addr
	bzero((char *) &serv_addr, sizeof(serv_addr));

	// set serv_addr values
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);
	
	// bind to socket
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Error: Child failed to bind socket!\n");

	// send confirmation to client
	n = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, clilen);
	if(n < 0) error("Error: Child failed to send message!\n");

	// receive byte count from client
	n = recvfrom(sockfd, &buffer, sizeof(buffer),0,(struct sockaddr *) &cli_addr, &clilen);
	byte_count = atoi(buffer);
	
	bzero(buffer,256);
	
	// receive file name and mode
	n = recvfrom(sockfd, &buffer, sizeof(buffer),0,(struct sockaddr *) &cli_addr, &clilen);
	
	// if client could not open the file, buffer string is set to "0"
	if(!strcmp(buffer,"0"))
	{
		error("Child: Client ended session.\n");	
	}

	// parse the mode and file name from the buffer
	mode = buffer[strcspn(buffer, ",") + 1];
	// place a null character where the comma is to isolate the file name
	buffer[strcspn(buffer, ",")] = '\0';

	// check for the existence of the file. access(char *filepath, int mode)
	if(access(buffer, F_OK) == -1 || access(buffer, R_OK) == -1 || access(buffer, W_OK) == -1)
	{
		// send rejection message
		sendto(sockfd, "0", sizeof("0"),0,(struct sockaddr *) &cli_addr,clilen);
		return;
	}

	// open the file
	fp = fopen(buffer, &mode);

	// lock the file using flock()
	flock(fileno(fp), LOCK_SH);
	
	// set the socket with the timeout value
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(struct timeval));

	bzero(buffer,256);
	// if mode is write, receive data from client and write to file 
	if(mode == 'w')
	{
		// receive data to write to afile
		n = recvfrom(sockfd,&buffer,sizeof(buffer),0,(struct sockaddr *) &cli_addr, &clilen);
		fputs(buffer,fp);
		printf("Data received from client %d to write to file:\n%s\n", cliid, buffer);
		
	}
	// if mode is read, read data from file and send to client
	else
	{
		// read from file
		fgets(buffer, byte_count, fp);
		printf("Data sent to client %d:\n%s\n", cliid, buffer);
		// send to client
		n = sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &cli_addr, clilen); 
		if(n < 0) error("Error: Child failed to send message!\n");

	}
	// unlock the file
	flock(fileno(fp),LOCK_UN);	

	printf("Connection to client %d has ended.\n",cliid);
	
	// close file	
	fclose(fp);
	close(sockfd);
	
	return;

}


// argv[1] = port number
int main(int argc, char *argv[])
{
	// var declaration
	// sockfd: file descriptor for socket
	int sockfd, n;
	// cliid_list stores client ids that have contacted the server
	int cliid_list[100];
	// port numbers sent to client for sessions, increments for each client
	// needed for concurrent servers
	int newportno = 8000;
	
	// holds address length of the client
	socklen_t clilen;
	// serv_addr contains this server's address, cli_addr holds client address
	struct sockaddr_in serv_addr, cli_addr;
	
	// for input and output through the socket
	char buffer[256];
	
	// file to be read or written to
	FILE * fp
	;
	// check args count for port number
	if(argc != 2)
	{
		error("usage: ./<EXECUTABLE <PORT NUMBER>\n");
	}

	// creating the socket
	// AF_INET: IPv4 protocol, SOCK_DRAM uses connectionless datagrams(UDP)
	sockfd = socket(AF_INET,SOCK_DGRAM, 0);
	if(sockfd < 0) error("Error: Failed to open the socket!\n");

	bzero((char *) &serv_addr, sizeof(serv_addr));

	// setting serv_addr values
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	// bind socket to serv_addr
	// if(bind(int socket, struct sockaddr * src_addr, socklen_t * src_len)
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Error: Failed to bind socket!\n");
	
	clilen = sizeof(cli_addr);		
	// receive client id, return new portno. use fork to create a new process
	for(;;)
	{
		bzero(buffer, 256);
		
		// receive client id		
		n = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, &clilen);
		if(n < 0) error("Error: Failed to receive message!\n");
		
		// check client id 
		if(check_cliid(buffer,cliid_list))
		{
			// pid for child process
			pid_t pid;
			char portbuffer[5];

			if(newportno == atoi(argv[1]))
			{
				newportno++;
			}
			//turn int newportno into a string
			sprintf(portbuffer,"%d", newportno);
			// send new port number to client
			n = sendto(sockfd, portbuffer, sizeof(portbuffer), 0, (struct sockaddr *) &cli_addr, clilen);
			if(n <0) error("Error: Failed to send message!\n");

			newportno++;
			
			// call fork
			pid = fork();
			// child function runs recv_message() 
			if(!pid)
			{
				// call function that receives and send msg on new port
				recv_message(newportno, atoi(buffer), cli_addr);
				return 0;
			}
		}
		// send 0 to client if not valid
		else
		{
			n = sendto(sockfd, "0", sizeof("0"), 0, (struct sockaddr *) &cli_addr, clilen);
			if(n < 0) error("Error: Failed to send message!\n");

			printf("UDP server: Dupllicate client number %s\n", buffer);
		}		

	}	
	
	// close socket
	close(sockfd);

	return 0;
}








 
