#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
// int main(){
//     char buffer[1000];
//     get_content(buffer, "/a.html");
//     printf("%s", buffer);
// }