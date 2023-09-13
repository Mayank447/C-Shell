#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "shell.h"
#include "color.h"

void handle_error(char* error){
    print_error(error);
    exit(EXIT_FAILURE);
}

// Function to remove tags in html
void remove_tags(char* input){
    int tag = 0;
    int l = strlen(input);
    int j=0;

    for(int i=0; i<l; i++){
        if(input[i]=='<'){
            tag = 1;
        }
        else if(input[i] == '>'){
            if(!tag) j=0; // the chunk read contained an incomplete part of tag as starting chracters
            tag = 0; // End of a tag, clear the flag
        }
        else if(!tag)
            input[j++] = input[i];
    }
    input[j]='\0';
}

void iman(char command_string[][MAX_ARGUMENT_LENGTH], int n_arguments){
    if(n_arguments==1){
        handle_error("iMan: missing argument\n");
    }

    else if(n_arguments>2){
        handle_error("iMan: too many arguments\n");
    }

    // DNS resolution
    struct hostent *host = gethostbyname("man.he.net");
    if (host == NULL) {
        handle_error("Error: Doing DNS resolution\n");
    }

    // Opening a network socket and loading the adress
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(network_socket < 0){
        handle_error("Error: Opening socket\n");
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);
    memcpy(&server_address.sin_addr.s_addr, host->h_addr, host->h_length);

    // Connecting to server and sending http request
    if (connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        handle_error("Error connecting to server\n");
    }

    char http_request[4096];
    sprintf(http_request, "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command_string[1]);
    if (send(network_socket, http_request, strlen(http_request), 0) < 0) {
        handle_error("Error sending request\n");
    }

    // Receiving and printing the ManPage
    char ManPage[10000];
    memset(ManPage, '\0', sizeof(ManPage));
    
    int bytes_read;
    int header_start = 0;
    char* start = 0;
    while ((bytes_read = recv(network_socket, ManPage, sizeof(ManPage), 0)) > 0) 
    {
        if(strstr(ManPage, "No matches for")!=NULL){
            handle_error("No such man command found\n");
        }

        remove_tags(ManPage);
        if(!header_start){
            start = strstr(ManPage, "NAME");
            header_start = 1;
            fwrite(start, 1, bytes_read, stdout);
        }
        else {
            fwrite(ManPage, 1, bytes_read, stdout);
        }

        //int end = strstr(ManPage, "SEE ALSO");
        // if (!header_end) {
        //     char *header_end_marker = strstr(ManPage, "\r\n\r\n");
        //     if (header_end_marker != NULL) {
        //         header_end = 1;
        //         // fwrite(response, 1, header_end_marker - response + 4, stdout);
        //         // Print an empty line to separate headers from content
        //         printf("\n");
        //         // Print the HTML content
        //         print_html(header_end_marker + 4);
        //     }
        // } 
        // else {
        //     // Print the HTML content
        //     print_html(ManPage);
        // }
    }

    if (bytes_read < 0) {
        handle_error("Error: Reading response from the webpage\n");
    }
    printf("\n");
    close(network_socket);
}