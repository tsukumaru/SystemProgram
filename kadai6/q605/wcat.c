/*
  echo-client-fdopen-one.c -- 文字列を送受信するクライアント(TCP/IP版)
  ~yas/syspro/ipc/echo-client-fdopen-one.c
  Created on: 2009/06/01 21:13:38
*/
#include <stdio.h>
#include <stdlib.h>	/* exit() */
#include <string.h>	/* memset(), memcpy() */
#include <sys/types.h>	/* socket() */
#include <sys/socket.h>	/* socket() */
#include <netinet/in.h>	/* struct sockaddr_in */
#include <netdb.h>	/* getaddrinfo() */
#include <string.h>	/* strlen() */
#include <unistd.h>	/* close() */

extern	int http_client_one( char *server, int portno, char *file );
extern  int echo_send_request( FILE *out, char *message );
extern  int echo_receive_reply( FILE *in, char buf[], int size );
extern	int tcp_connect( char *server, int portno );
extern  int fdopen_sock( int sock, FILE **inp, FILE **outp );
extern  int http_request( FILE *out, char *server, int portno, char *file);

int
main( int argc, char *argv[] )
{
	char *server ;
	int portno ;
	char *message, *file ;
	int err;
	
	if( argc != 4 )
	{
		fprintf( stderr,"Usage: %s host port file\n",argv[0] );
		exit( -1 );
	}
	
	server  = argv[1] ;
	portno  = strtol( argv[2],0,10 );
	file = argv[3];
	

	err = http_client_one( server, portno, file );
	return( err );
}

#define	BUFFERSIZE	1024
 
int
http_request( FILE *out, char *server, int portno, char *file) {
	int res;
	
	res = fprintf(out,"GET %s HTTP/1.0\r\nHost: %s:%d\r\n\r\n", file, server, portno);
	return res;
}

int
http_client_one( char *server, int portno, char *file)
{
	int sock ;
	FILE *in, *out ;
	char rbuf[BUFFERSIZE];
	int res;

	sock = tcp_connect( server, portno );
	if( sock<0 )
		return( 1 );
	
	if( fdopen_sock(sock,&in,&out) < 0 )
	{
		fprintf(stderr,"fdooen()\n");
		close( sock );
		return( 1 );
	}
	
	res = http_request( out, server, portno, file);
	if( res < 0 )
	{
		fprintf(stderr,"fprintf()\n");
		fclose( in );
		fclose( out );
		return( 1 );
	}

	
//	printf("sent: %d bytes [%s\n]\n",res,message );
	while(res != -1) {
		res = echo_receive_reply( in, rbuf, BUFFERSIZE );
		fprintf(stdout, "%s\n", rbuf);
	}
/*	if( res < 0 )
	{
		fprintf(stderr,"fprintf()\n");
		fclose( in );
		fclose( out );
		return( 1 );
	}
*/
	printf("received: %d bytes [%s]\n", res, rbuf );
	fclose( in );
	fclose( out );
	return( 0 );
}

int
echo_send_request( FILE *out, char *message )
{
	int res;
	res = fprintf( out, "%s\n", message ); /* send a request with '\n' */
	return( res );
}

int
echo_receive_reply( FILE *in, char buf[], int size )
{
	char *res;
	res = fgets( buf, size, in ); /* receive a reply message */
	if( res )
		return( strlen(buf) );
	else
		return( -1 );
}

/* 新城、筑波大学／情報学類／システムプログラム講義用 */
#define PORTNO_BUFSIZE 30

int
tcp_connect( char *server, int portno )
{
	struct addrinfo hints, *ai;
	char portno_str[PORTNO_BUFSIZE];
	int s, err;
	snprintf( portno_str,sizeof(portno_str),"%d",portno );
	memset( &hints, 0, sizeof(hints) );
	hints.ai_socktype = SOCK_STREAM;
	if( (err = getaddrinfo( server, portno_str, &hints, &ai )) )
	{
		fprintf(stderr,"unknown server %s (%s)\n",server,
			gai_strerror(err) );
		goto error0;
	}
	if( (s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0 )
	{
		perror("socket");
		goto error1;
	}
	if( connect(s, ai->ai_addr, ai->ai_addrlen) < 0 )
	{
		perror( server );
		goto error2;
	}
	freeaddrinfo( ai );
	return( s );
error2:
	close( s );
error1:
	freeaddrinfo( ai );
error0:
	return( -1 );
}

int
fdopen_sock( int sock, FILE **inp, FILE **outp )
{
	int sock2 ;
	if( (sock2=dup(sock)) < 0 )
	{
		return( -1 );
	}
	if( (*inp = fdopen( sock2, "r" )) == NULL )
	{
		close( sock2 );
		return( -1 );
	}
	if( (*outp = fdopen( sock, "w" )) == NULL )
	{
		fclose( *inp );
		*inp = 0 ;
		return( -1 );
	}
	setvbuf(*outp, (char *)NULL, _IONBF, 0);
	return( 0 );
}
