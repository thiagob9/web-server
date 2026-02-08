#pragma once 
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_LENGTH_BODY 1024

void write_log(char *message){
   FILE *log = fopen("server.log","a+");
    if(log == NULL){
        perror("fala");
        return;
    }

    time_t tempo;
    time(&tempo);

    struct tm *tempo0 = localtime(&tempo);

    char *date = malloc(50*sizeof(char));

    strftime(date, 50, "%c", tempo0);

    fprintf(log, "[%s]: \n%s\n\n", date, message);
    free(date);
    fclose(log);
}

char *get_file_type(char *file){
    if(strstr(file, ".html")){
        return "text/html";
    }

    if(strstr(file, ".css")){
        return "text/css";
    }

    if(strstr(file, ".json")){
        return "application/json";
    }

    if(strstr(file, ".js")){
        return "application/javascript";
    }

    if(strstr(file, ".png")){
        return "image/png";
    }

    if(strstr(file, ".jpeg")){
        return "image/jpeg";
    }

    if(strstr(file, ".gif")){
        return "image/gif";
    }

    if(strstr(file, ".svg")){
        return "image/svgh+xml";
    }

    if(strstr(file, ".pdf")){
        return "application/pdf";
    }

    return "application/octet-stream";
}

void send_content(int sock, char *content, char *uri){
    if(strstr(uri, "../")){
        char *header = "HTTP/1.0 404 NOT FOUND\r\n"
        "Server: server.c\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><h1>PAGE NOT FOUND</h1></html>"; 
    
        int val_response = write(sock, header, strlen(header));
        if(val_response < 0){
            perror("error trying to send response");
        }
        return;       
    }


    FILE *file = NULL;
    char *file_type = NULL;
    int uri_len = strlen(uri);

    if(uri[strlen(uri)-1] == '/'){
        int extra_len = strlen("index.html");
        char *file_path = malloc((sizeof(char))*(uri_len+extra_len+2));
        
        if(file_path == NULL){
            perror(file_path);
            exit(1);
        }   

        memset(file_path, '\0', strlen(file_path));

        file_path[0] = '.';
        strncat(file_path, uri, uri_len);
        strncat(file_path, "index.html", extra_len);
        
        file_path[uri_len+extra_len+1] = '\0';
        file = fopen(file_path, "rb");
        free(file_path);
        file_type = "text/html";
    }else{
        char *file_path = malloc(sizeof(char)*(uri_len+2));
        if(file_path == NULL){
            perror(file_path);
            exit(1);
        }

        memset(file_path, '\0', strlen(file_path));
        file_path[0] = '.';
        strncat(file_path, uri, uri_len);
        
        file_path[uri_len+1] = '\0';
        file = fopen(file_path, "rb");
        free(file_path);
        file_type = get_file_type(uri);
    }
    
    if(file == NULL){
        char *header = "HTTP/1.0 404 NOT FOUND\r\n"
        "Server: server.c\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><h1>PAGE NOT FOUND</h1></html>"; 
    
        int val_response = write(sock, header, strlen(header));
        if(val_response < 0){
            perror("error trying to send response");
        }
        return;
    }

    printf("\n\n%s\n\n", file_type);
    char header[200];

    snprintf(header, 200, "HTTP/1.0 200 OK\r\n"
    "Server: server.c\r\n"
    "Content-Type: %s\r\n\r\n", file_type);
    
    int val_response = write(sock, header, strlen(header));
    if(val_response < 0){
        perror("error trying to send response");
        return;
    }

    size_t n = 0;
    while((n = fread(content, 1, MAX_LENGTH_BODY, file)) > 0){
        val_response = write(sock, content, n);
        
        if(val_response < 0){
            perror("error trying to send response");
            return;
        }
    }
    fclose(file);
    write_log(content);
}
