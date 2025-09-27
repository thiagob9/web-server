#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

char *sliding_window(char *string, unsigned int start,  unsigned int end){
    int newLen = end - start;
    if(newLen < 1){
        return NULL;
    }

    char *newString = malloc(sizeof(char)*(newLen+1));
    int j = 0;
    for(int i = start; i < end; i++){
        newString[j] = string[i];
        j++;
    }
    newString[j] = '\0';
    return newString;
}

void get_content(char *content, char *uri){
    FILE *file = NULL;
    
    char *path = sliding_window(uri, 1, strlen(uri));
    if(strcmp(uri, "/") == 0){
        file = fopen("index.html", "r");
    }
    else{
        file = fopen(path, "r");
    }


    if(file == NULL){
        strcpy(content,
        "HTTP/1.0 404 NOT FOUND\r\n"
        "Server: server.c\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><h1>404 Not Found</h1></html>");
        return;
    }
    
    strcpy(content,
    "HTTP/1.0 200 OK\r\n"
    "Server: server.c\r\n"
    "Content-Type: text/html\r\n\r\n"
    );
    
    free(path);
    
    int len = strlen(content);
    fread(content + len, sizeof(char), (1000 - len), file);
}

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

void clear_buffer(char *response){
    memset(response, '\0', strlen(response));
}
// int main(){
//     write_log("eu odeio");
//     char buffer[1000];
//     get_content(buffer, "/a.html");
//     printf("%s", buffer);
// }