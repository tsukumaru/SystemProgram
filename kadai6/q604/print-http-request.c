
/*
  print-http-request.c -- HTTPの要求を画面に表示する
*/

#include <stdio.h>  /* fprintf() */
#include <stdlib.h> /* exit() */

extern void http_request( FILE *out, char *url, char *path );

int
main( int argc, char *argv[] )
{
	if (argc != 3) {
		printf("Usage: %s URL Path\n", argv[0]);
		exit(-1);
	}

	http_request( stdout , argv[1], argv[2]);
}

void
http_request( FILE *out, char *url, char *path) {
    	fprintf(out,"GET %s HTTP/1.0\r\nHost:%s\r\n\r\n", path, url);
}
