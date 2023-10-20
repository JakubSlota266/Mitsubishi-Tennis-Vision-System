#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_MSG_LEN 4096

#define SERWER_IP "192.168.192.125"
#define MAX_CONNECTION 10

int main()
{
	int port = 55555;
    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
        .sin_port = htons( port )
    };
    if( inet_pton( AF_INET, SERWER_IP, & serwer.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
   
    const int socket_ = socket( AF_INET, SOCK_STREAM, 0 );
    if( socket_ < 0 )
    {
        perror( "socket() ERROR" );
        exit( 2 );
    }
   
    socklen_t len = sizeof( serwer );
    if( bind( socket_,( struct sockaddr * ) & serwer, len ) < 0 )
    {
        perror( "bind() ERROR" );
        exit( 3 );
    }
   
    if( listen( socket_, MAX_CONNECTION ) < 0 )
    {
        perror( "listen() ERROR" );
        exit( 4 );
    }
   
   
    
        printf( "Waiting for connection...\n" );
       
        struct sockaddr_in client = { };
       
        const int clientSocket = accept( socket_,( struct sockaddr * ) & client, & len );
        if( clientSocket < 0 )
        {
            perror( "accept() ERROR" );
            return -1;
        }
		while( 1 )
		{
        char buffer[ MAX_MSG_LEN ] = { };
       
        strcpy( buffer, "Message from server\n" );
        if( send( clientSocket, buffer, strlen( buffer ), 0 ) <= 0 )
        {
            perror( "send() ERROR" );
            exit( 6 );
        }
       
    }
   
    shutdown( socket_, SHUT_RDWR );
}
