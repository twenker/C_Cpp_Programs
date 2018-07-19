// Name of Author: Todd Wenker
// Course and Semester: CSE 434, Computer Networks and Fall 2017
// Project Part: 2
// Time Spent: 15 hours 
// UDP client filelock program

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

// includes the definition for the timeval struct
#include <sys/time.h>

// includes flock() function
#include <sys/file.h>

// error report function
void error(char *msg)
{
        perror(msg);
        exit(1);
}

// argv[1] = port number
// argv[2] = hostname
// argv[3] = client number
// argv[4] = number of bytes to send
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
	
	// buffer for general strings, mode for storing a w or r for write/read
	char buffer[256];
	char mode[2];	
	
	// file to be read or written to
	FILE *fp;

	// time value set to 3 seconds
	struct timeval timeout={3,0};

	// check command line args
	if(argc != 5)
	{
		error("usage: ./<EXECUTABLE> <SERVER PORT> <HOST NAME> <CLIENT ID> <NUMBEr OF BYTES>\n");
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

	// check if server rejected request
	if(buffer[0] == '0')
	{
		printf("Client number is not valid.\n");
		// close socket
		close(sockfd);
		return 0;
	}
	
        printf("Received new port number from server: %s\n", buffer);

	// set new port number from server and adjust sockaddr serv_adder
	serv_addr.sin_port = htons(atoi(buffer));	

	// Recieve confimation message from server
        bzero(buffer, 256);
        n = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *) &serv_addr, &addrlen);
        if(n < 0) error("Error: Failed to receive message!\n");
		
	// send byte count to server
	n = sendto(sockfd,argv[4],sizeof(argv[4]),0,(struct sockaddr *) &serv_addr, addrlen);
	
	bzero(buffer, 256);
	
	// receive file name from user
	printf("Enter file name.\n");
	fgets(buffer, 256, stdin);	
	// remove trailing newline
	buffer[strlen(buffer)-1] = '\0';

	// receive mode
	printf("Enter file mode.\n");
	fgets(mode, 2, stdin);

	// check for the existence of the file and whether it can be read or written
	// access(char *filepath, int mode)
	if(access(buffer, F_OK == -1) || access(buffer, R_OK) == -1 || access(buffer, W_OK) == -1)
	{
		// kill connnection to server
		sendto(sockfd, "0", sizeof("0"),0,(struct sockaddr *) &serv_addr, addrlen);
		return 0;
	}
	
	// open file with mode. if writing to server, open for read and vice versa
	if(mode[0] == 'w')
	{
		fp = fopen(buffer,"r");
	}
	else
	{
		fp = fopen(buffer,"w");
	}
	// add comma
	strcat(buffer,",");
	// concat with buffer to form message for server
	strcat(buffer,mode);
	
	printf("<filename>,<mode> sent to server: %s\n", buffer);
	
	// send file name and mode to client
	n = sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &serv_addr, addrlen);
	if(n < 0) error("Error: Failed to send message!\n");

	bzero(buffer, 256);

	// receive confirmation message from Server that the file exists and can be accessed
	recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &serv_addr, &addrlen);
	if(!strcmp(buffer,"0"))
	{
		printf("Server could not open file.\n");
		return 0;
	}

	// lock the file. if it returns -1 then the file is already locked
	if(flock(fileno(fp), LOCK_EX | LOCK_NB) == -1)
	{
		printf("File already locked.\n");
		return 0;
	}

	// set the socket with the timeout value
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(struct timeval));
	
	// if mode is write vs read
	// write. Read first b bytes FILE fp and send them to the server
	if(mode[0] == 'w')
	{
		// read from file
		fgets(buffer, atoi(argv[4]), fp);
		printf("Data sent to server:\n%s\n",buffer);
		// send buffer to server
		n = sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &serv_addr, addrlen);

	}
	// read. Receive first b bytes from file on server side then write them to 
	// FILE fp
	else
	{
		// receive file data from server
		n = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &serv_addr, &addrlen);		
		// write to file
		fputs(buffer, fp);
		printf("Data received from server:\n%s\n",buffer);
	}
	
	if(n < 0) 		
	// unlock the file
	flock(fileno(fp),LOCK_UN);

	// close file and socket
	fclose(fp);
	close(sockfd);

	return 0;
}





