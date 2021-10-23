//
// Created by Yuriy Mikhaildi on 10/20/21.
//
#include "Handler.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>



void handle_error(char* msg){
    perror(msg);
}
int compare_file_type(char* name, char* type){
    int file_name_len, type_str_len;
    file_name_len = strlen(name);
    type_str_len = strlen(type);

    if(type_str_len > file_name_len)
        return 0;
    /* loop from end to check the file type against name */
    for(int i = file_name_len - 1, j = type_str_len - 1; j >= 0; --i, --j)
        if(name[i] != type[j])
            return 0;

    return 1;
}

char* header_mime_type(char* file_name){
    char bufffer[MAXLINE];
    if(compare_file_type(file_name, "html"))
        strcpy(bufffer, HTML);

    else if(compare_file_type(file_name, "txt"))
        strcpy(bufffer, PLAIN);

    else if(compare_file_type(file_name, "png"))
        strcpy(bufffer, PNG);

    else if(compare_file_type(file_name, "gif"))
        strcpy(bufffer, GIF);

    else if(compare_file_type(file_name, "jpg"))
        strcpy(bufffer, JPG);
    else if(compare_file_type(file_name, "ico"))
        strcpy(bufffer, ICO);
    else if(compare_file_type(file_name, "css"))
        strcpy(bufffer, CSS);
    else if(compare_file_type(file_name, "js"))
        strcpy(bufffer, JS);
    else
        handle_error( "ERROR wrong file format");

    return bufffer;
}
/* file size method is base of https://stackoverflow.com/questions/6537436/how-do-you-get-file-size-by-fd */
void handle_request(Request* request_info, int connfd){
    /*local*/
    long file_size;
    int  r_file_descriptor;
    struct stat file_stats;

    /* file path */
    const char* file_path = request_info->r_uri;

    /*get file stats*/
    if(stat(file_path, &file_stats) < 0)
        handle_error("ERROR: File stats failed");

    /*record file size */
    file_size = file_stats.st_size;

    /* open file */
    if((r_file_descriptor = open(file_path, O_RDONLY)) < 0)
        handle_error("ERROR: File was not open");

    handle_response(r_file_descriptor, file_size, connfd, request_info);
}

void handle_response(int req_file_descriptor, long file_size, int connfd, Request* request_info){
        /* allocate packet memory */
        unsigned char packet[MAXLINE + file_size];
        unsigned char header [MAXLINE];
        unsigned char payload[file_size];

        /*get file payload */
        long read_bytes = read(req_file_descriptor, payload, file_size);
        /* get header string */
        sprintf(header, header_mime_type(request_info->r_uri), file_size);
        /* copy header in */
        strcpy(packet, header);

        /*copy payload */
        memcpy(packet + strlen((char*)header), payload, sizeof (payload));
        /*print ACK */
        printf("server returning a http message with the following content.\n%s\n", packet);

        /* send response */
        long write_bytes = write(connfd, packet, sizeof (packet) );
        printf("Read %ld, Write %ld\n", read_bytes, write_bytes);


        close(req_file_descriptor);
}
/*
 * parser function, based of https://codereview.stackexchange.com/questions/245454/parse-string-into-a-struct,
 *      parser function breaks the buffer to array of strings, the strings are put in the struct, the request info
 *      command, url (root file path), http version.
 */
Request* parse_request(char* request_buffer){
    /* basic vars */
    Request* req;
    char* req_type = "GET";
    char* delim =  " ";
    char* uri = strtok(request_buffer, delim);
    char** request_arr = calloc(sizeof(request_buffer), sizeof (char *));

    /*parsing loop to get info */
    for (int i = 0; uri != NULL; i++) {
        uri[strcspn(uri, "\n")] = '\0'; // drop trailing new lines
        request_arr[i] = uri;
        uri = strtok(NULL, delim); //parse char
    }

//    /*check the request type */
//    if(strcmp(request_arr[0], "GET") != 0 && strcmp(request_arr[0], "HEAD") != 0)
//        handle_error("ERROR: Request is not recognize");

    // Parse GET command
    if (strcmp(request_arr[0], req_type) == 0) {
        if (request_arr[1] == NULL) {
            handle_error("ERROR: Uri is empty");
        }
        // Validate key length
        if (strlen(request_arr[1]) > MAXLINE) {
            handle_error("ERROR: Uri exceeds the max buffer");
        }
        /* create file path, allocate the buf */
        char* full_path;
        int length_combined = strlen(request_arr[1]) + 2;
        full_path = malloc((length_combined) * sizeof(char));

        /* strcat the full path */
        bzero(full_path, sizeof (full_path));
        char* file_root_path = "..";
        strcat(full_path, file_root_path);

        /*construct the request */
        strcpy(req->r_method, request_arr[0]);
        strcpy(req->r_uri, strcat(full_path, request_arr[1]));
        strcpy(req->r_version, request_arr[2]);

        free(full_path);
    }
    return req;
}