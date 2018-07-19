// Name of Author: Todd Wenker
// Course and Semester: CSE 434, Computer Networks and Fall 2017
// Project Part: 1
// Time Spent: 15-20 hours
// Simple client using UDP protocol

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

// includes the hostent struct 
#include <netdb.h>

// error report function
void error(char *msg)
{
        perror(msg);
        exit(1);
}

// argv[1] = port number
// argv[2] = hostname
// argv[3] = client number
int main(int argc, char *argv[])
{
	// var declarations

	// sockfd is the socket file descriptor
	// n saves the return values of functions
	int sockfd, n;

	// serv_addr for holding the address of the server
	struct sockaddr_in serv_addr;
	
	// addrlen holds the lenght of serv_addr
	socklen_t addrlen = sizeof(serv_addr);	

	//variable server is a pointer to a structure of type hostent
	/*
	struct  hostent
	{
  		char    *h_name;        // official name of host
  		char    **h_aliases;    // alias list
  		int     h_addrtype;     // host address type
  		int     h_length;       // length of address
  		char    **h_addr_list;  // list of addresses from name server
  		#define h_addr  h_addr_list[0]  // address, for backward compatiblity
	};
	*/
	struct hostent * server;

	char buffer[256];
	
	// check command line args
	if(argc != 4)
	{
		error("Error: Need Port Number, hostname, and client id");
	}

	// socket(int domain, int type, int protocol)
	// AF_INET designates IPv4 
	// SOCK_DGRAM designates connectionless datagrams
	// 0 designates a default protocol (UDP in this case)
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0) error("Error: Failed to create socket!\n");
	
	// gethostbyname() takes a string as and argument and returns a pointer to a
	// hostent struct with information about the host
	server = gethostbyname(argv[2]);
	if(!server) error("Error: No such host!\n");

	// Set the address buffer to 0
	bzero((char *) &serv_addr, sizeof(serv_addr));

	// sets the address family of serv_addr to AF_INET
	serv_addr.sin_family = AF_INET;
	// copies bytes from server's h_addr field to serv_addr.sin_addr.s_addr
	// essentialy extracts the server's address from server to serv_addr
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);

	// sets serv_addr with the designated port from the command line
	// htons sets converts it into unsigned short int with network byte order 
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	// Send client id to server
	bzero(buffer, 256);
	strcpy(buffer, argv[3]);
	
	// sendto(int sockfd, const void* buf, size_t len, int flags, const struct 
        //      sockaddr *dest_addr, socklen_t addrlen)
        n = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serv_addr, addrlen);
        if(n < 0) error("Error: Failed to send message!\n");

	// recieve new port number from server
	// recvfrom(int sockfd, const void *buf, size_t len, int flags, const struct
        //      sockaddr *restrict address, socklen_t *restrict address_len)
        bzero(buffer, 256);
        n = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &serv_addr, &addrlen);
        if(n < 0) error("Error: Failed to receive message!\n");

	// set new port number from server and adjust sockaddr serv_adder
	serv_addr.sin_port = htons(atoi(buffer));	

	// Recieve confimation message from server
        bzero(buffer, 256);
        n = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &serv_addr, &addrlen);
        if(n < 0) error("Error: Failed to receive message!\n");
	// loop. Sends messages to new port 
	for( ; ;)
	{
		// receiving the message from user and sending to new port on server
		bzero(buffer, 256);
		printf("Please enter message.");
		fgets(buffer, 256, stdin);
		
		n = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serv_addr, addrlen);
		if(n < 0) error("Error: Failed to send message!\n");
		
		// receive the server response
		bzero(buffer, 256);
		n = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &serv_addr, &addrlen);
		if(n < 0) error("Error: Failed to receive message!\n");
		
		// check if server rejected message
		if(!strcmp(buffer,"0"))
		{
			printf("client number is not valid.");
			// close socket
			close(sockfd);
			return 0;
		}
		
		printf("Echo from server: %s\n", buffer);
	
	}
	
	return 0;
}





