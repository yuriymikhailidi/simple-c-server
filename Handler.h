//
// Created by Yuriy Mikhaildi on 10/20/21.
//

// MIME Types
//.html text/html
//.txt  text/plain
//.png  image/png
//.gif  image/gif
//.jpg  image/jpg
//.css  text/css
//.js   application/javascript


#ifndef HANDLER_H
#define HANDLER_H

#include <stddef.h>
#include <stdio.h>
#include <strings.h>

#define MAXLINE  8192  /* m ax text line length */
#define MAXBUF   8192  /* max I/O buffer size */
#define LISTENQ  1024  /* second argument to listen() */
#define REQLINE  1024  /* req param string size */

/*header macros */
#define HTML "HTTP/1.1 200 Document Follows\r\nContent-Type:text/html\r\nContent-Length: %ld\r\n\r\n"
#define PLAIN "HTTP/1.1 200 Document Follows\r\nContent-Type:text/plain\r\nContent-Length: %ld\r\n\r\n"
#define PNG "HTTP/1.1 200 Document Follows\r\nContent-Type:image/png\r\nContent-Length: %ld\r\n\r\n"
#define GIF "HTTP/1.1 200 Document Follows\r\nContent-Type:image/gif\r\nContent-Length: %ld\r\n\r\n"
#define JPG "HTTP/1.1 200 Document Follows\r\nContent-Type:image/jpg\r\nContent-Length: %ld \r\n\r\n"
#define ICO "HTTP/1.1 200 Document Follows\r\nContent-Type:image/x-icon\r\nContent-Length: %ld \r\n\r\n"
#define CSS "HTTP/1.1 200 Document Follows\r\nContent-Type:text/css\r\nContent-Length: %ld\r\n\r\n"
#define JS "HTTP/1.1 200 Document Follows\r\nContent-Type:application/javascript\r\nContent-Length: %ld\r\n\r\n"
#define ERR "%s 500 Internal Server Error\r\n\r\n"


/* request handle */
typedef struct {
    char r_method[REQLINE];
    char r_uri[REQLINE];
    char r_version[REQLINE];
} Request;

/* handle the request,
 * takes parsed input, handles write file to buf */
void handle_request(Request *, int);

/* handle the response,
 * takes file buffer, handles response packet w/HTTP header */
void handle_response(int, long, int, Request *, FILE*);

/*error wrapper */
void handle_error(int);

/* header mapper, takes file type string, returns header */
char* header_mime_type(char*, int);
/*helper to find the fyle type*/
int compare_file_type(char*, char*);


#endif //HANDLER_H
