#include "webserver.h"

int main(){
	char buffer[BUFFER_SIZE];
	char response[MAX_LENGTH_BODY];

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("Error trying to open server");
		return 1;
	}

	struct sockaddr_in sockaddr;
	int addr_len = sizeof(sockaddr);

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(PORT);
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	struct sockaddr_in client_addr;
	int addr_client_len = sizeof(client_addr);

	if(bind(sock, (struct sockaddr *)&sockaddr, addr_len) != 0){
		perror("error trying to bind the address");
		return -1;
	}
	
	if(listen(sock, SOMAXCONN) != 0){
		perror("error trying to listen connections");
		return 1;
	}
	printf("Listening to connections! \n");
	
	while(1){
		int new_sock = accept(sock, (struct sockaddr *)&sockaddr, (socklen_t *)&addr_len);

		if(new_sock < 0){
			perror("error trying to accept the connection");
			continue;
		}
		printf("connection accepted\n");
		
		if(fork() == 0){
			int sock_ip = getsockname(new_sock, (struct sockaddr *)&client_addr, (socklen_t *)&addr_client_len);

			int request = read(new_sock, buffer, BUFFER_SIZE);
			if(request < 0){
				perror("error trying to read the request");
				continue;
			}

			char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
			sscanf(buffer, "%s %s %s", method, uri, version);

			printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), method, version, uri);

			send_content(new_sock, response, uri);
			close(new_sock);
			_exit(0);
		}
		
		close(new_sock);
	}


	return 0;
}
