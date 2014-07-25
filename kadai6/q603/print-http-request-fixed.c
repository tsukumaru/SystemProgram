
/*
  print-http-request-fixed.c -- HTTPの要求を画面に表示する(枠組みだけ)
  ~yas/syspro/ipc/print-http-request-fixed.c
  Created on: 2014/07/24 17:33:10
*/

#include <stdio.h>  /* fprintf() */
#include <stdlib.h> /* exit() */

extern void http_request_fixed( FILE *out );

int
main( int argc, char *argv[] )
{
	http_request_fixed( stdout );
}

void
http_request_fixed( FILE *out ) {
    	/* この関数の内容を変更しなさい。*/
    	fprintf(out,
	"GET /about/index.html HTTP/1.0\r\nHost:www.tsukuba.ac.jp\r\n\n");
}
