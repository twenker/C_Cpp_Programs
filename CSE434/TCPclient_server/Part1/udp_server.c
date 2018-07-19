// Name of Author: Todd Wenker
// Course and Semester: CSE 434, Computer Networks and Fall 2017
// Project Part: 1
// Time Spent: 15-20 hours 
// Simple server using UDP protocol
/*
Question: What is the difference in the creation in TCP sockets vs UDP sockets?
A TCP connection requires more steps to set up. While both TCP and UDP require the 
socket file descriptor to be created using the socket() function and the socket
to be bound to the server's address using the bind() function, a TCP requires more 
steps. The server then needs to use listen() function to begin listening to the socket
and then needs to use the accept() function to wait for and accept an incoming 
connection. To read data sent through the connection, the read() is used and to send 
data, the write() function is use. 
After binding the socket to the server address, the sendto() and recvfrom() functions
are used. While read() and write() on a TCP connection only need the socket file
descriptor to send or receive data, sendto() needs the address of the destination. 
This is because TCP is connection oriented while UDP is connectionless.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// includes definitions for data types used in sys calls
#include <sys/types.h>

// includes definition for sockets
#include <sys/socket.h>

// includes constants and structures for domain addresses
#include <netinet/in.h>

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

// child process. Use new port number to receive and send echo to client
void recv_message(int portno, int cliid, struct sockaddr_in cli_addr)
{
	int sockfd, n;
	char buffer[256] = "Hello";
	struct sockaddr_in serv_addr;
	socklen_t clilen = sizeof(cli_addr);
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero((char *) &serv_addr, sizeof(serv_addr));

	// set serv_addr values
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);

	// bind to socket
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Error: Child failed to bind socket!\n");
	
	// send confirmation message to client
	n = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, clilen);
	if(n < 0) error("Error: Child failed to send message!\n");

	// receive message from client, send it back
	bzero(buffer, 256);

	n = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, &clilen);
	if(n < 0) error("Error: Child failed to receive message!\n");
	
	// remove trailing newline char
	buffer[strcspn(buffer, "\n")] = 0;
	printf("Message \" %s \" received from client%d\n", buffer, cliid);
	
	n = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, clilen);
	if(n < 0) error("Error: Child failed to send message!\n");
	
	// wait for new message from client and return a 0
	bzero(buffer, 256);
        
        n = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, &clilen);
        if(n < 0) error("Error: Child failed to receive message!\n");
	
	printf("UDP Server: Duplicate client number %d\n", cliid);
	
	n = sendto(sockfd, "0", sizeof("0"), 0, (struct sockaddr *) &cli_addr, clilen);
	if(n < 0) error("Error: Child failed to send message!\n");
	
	// close socket
	close(sockfd);

	return;
}

// argv[1] = port number
int main(int argc, char *argv[])
{
	// var declaration
	// sockfd/newsockfd both store the return values of the socket()  
	int sockfd, n;
	// port numbers to be sent to clients. increments for each client
	int newportno = 8000;

	// clilen holds the address length of the client. used in accept sys call
	socklen_t clilen;	
	
	// reads the characters sent from the client throught the socket connection
	char buffer[256];

	// holds client ids
	int cliid_list[100];
	//sockaddr_in is a structure containing an internet address
	/*
	struct sockaddr_in
	{
  		short   sin_family; //must be AF_INET
  		u_short sin_port;
  		struct  in_addr sin_addr;
  		char    sin_zero[8]; // Not used, must be zero
	};
	*/
	//in_addr structure, contains only one field, a unsigned long called s_addr.

	// serv_add contains this server's address and cli_addr contains the address
	// of the client 
	struct sockaddr_in serv_addr, cli_addr;

	// check and set port number
	if(!argv[1])
	{
		error("Error: Port Number missing!\n");
	}

	// creating the socket. AF_INET uses IPv4 protocol, SOCK_DGRAM uses 
	// conectionless datagrams (UDP)
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0) error("Error: Failed to open socket!\n");

	bzero((char *) &serv_addr, sizeof(serv_addr));

	// set serv_addr values
	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	serv_addr.sin_port = htons(atoi(argv[1]));

	// bind(int socket, struct sockaddr * src_addr, socklen_t *src_len)
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Error: Failed to bind socket!\n");

	clilen = sizeof(cli_addr);
	
	// receive client id from client, if new client, use fork to run recv_message
	// function
	for(;;)
	{
		bzero(buffer,256);
		
		// receive client number through stated port
		// recvfrom(int socket, void *restrict buffer, size_t length, 
		// int flags, struct sockaddr * restric src_addr, 
		// socklen_t *restrict *src_len)
		n = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr,	&clilen);
		if(n < 0) error("Error: Failed to receive message!\n");

		// if request is valid, print and respond to the client
		// use fork to execute recv_message(int portno, int cliid)
		if(check_cliid(buffer, cliid_list))
		{
			// pid stores the return from fork
			// portbuffer stores string conversion of newportno
			pid_t pid;
			char portbuffer[5];
	
			// send new port number to client
	        	// sendto((int sockfd, const void *buf, size_t len, int flags, 
	        	//      const struct sockaddr *dest_addr, socklen_t addrlen);
	        	
			// if the cmd line arg port number is equal to newportno, skip 
			// it
			if(newportno == atoi(argv[1]))
			{
				newportno++;
			}
			// use sprintf to transfer newportno into string
			sprintf(portbuffer, "%d", newportno);
			// send new port number to client
			n = sendto(sockfd, portbuffer, sizeof(portbuffer), 0, (struct sockaddr *) &cli_addr, clilen); 
                        if(n < 0) error("Error: Failed to send message!\n");
			
			newportno++;			

			// the child process will have pid == 0
			pid = fork();
			if(!pid)
			{
				recv_message(newportno, atoi(buffer), cli_addr);
				return 0;
			}
			
		}
		// if client id is not valid, send 0 to client
		else
		{
			n = sendto(sockfd, "0", sizeof("0"), 0, (struct sockaddr *) &cli_addr, clilen);
			if(n < 0) error("Error: Failed to send message!\n");

			printf("UDP server: Duplicate client number %s\n", buffer);
		
		}
	}
	
	// close sockets
	close(sockfd);
	
	return 0;
}





