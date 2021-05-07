#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

typedef struct user {
	char username[200];
	char password[200];
}user;

typedef struct clientdata {
	int client;
	int skiplogin;
}clientdata;

int connected;
int connectedwrite;
int accountwrite;
int filewrite;
int accountn;
user accounts[100];

typedef struct bookdata {
	int client, cenemy;
	int* health1;
	int* health2;
}bookdata;

void* serve(void* arg) {
	clientdata d = *(clientdata*)arg;
	int client = d.client;
	int skiplogin = d.skiplogin;
	if (!skiplogin) {
		screen1:;
		char data[500];
		memset(data, 0, sizeof(data));
		int mode;
		char username[200], password[200];
		read(client, data, sizeof(data));
		mode = data[0] - '0';
		int i = 2;
		for (i = 2; i < strlen(data); i++) {
			if (data[i] == '\t') break;
			username[i-2] = data[i];
		}
		username[i-2] = '\0';
		int ii = ++i;
		for (; i < strlen(data); i++) {
			if (data[i] == '\t') break;
			password[i-ii] = data[i];
		}
		if (mode == 0) { // LOGIN
			int success = 0;
			while (accountwrite);
			for (int i = 0; i < accountn; i++) {
				if (strcmp(username, accounts[i].username) == 0 && strcmp(password, accounts[i].password) == 0) {
					success = 1;
					break;
				}
			}
			send(client, &success, sizeof(success), 0);
			if (success) printf("Auth success\n");
			else {
				printf("Auth failed\n");
				goto screen1;
			}
		} else if (mode == 1) { // REGISTER
			accountwrite = 1;
			user newacc;
			strcpy(newacc.username, username);
			strcpy(newacc.password, password);
			accounts[accountn] = newacc;
			while (filewrite);
			filewrite = 1;
			FILE *f = fopen("akun.txt", "a");
			fprintf(f, "%s:%s\n", username, password);
			fclose(f);
			filewrite = 0;
			accountn++;
			accountwrite = 0;
			for (int i = 0; i < accountn; i++) {
				printf("%s\t%s\n", accounts[i].username, accounts[i].password);
			}
		}
	}
	skiplogin = 0;
	int mode;
	read(client, &mode, sizeof(mode));
	if (mode == 0) { // FIND MATCH
		while (connectedwrite);
		connectedwrite = 1;
		connected++;
		connectedwrite = 0;
		while (connected < 2);
	} else if (mode == 5) { // LOGOUT
		goto screen1;
	}
	pthread_exit(0);
}

int main() {
	int sock;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	  
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	if (bind(sock, (struct sockaddr *)&address, sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(sock, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	FILE* f = fopen("akun.txt", "a");
	char username[200], password[200];
	int index = 0;
	while (fscanf(f, "%[^\t]\t%[^\n]\n", username, password) != EOF) {
		user newacc;
		strcpy(newacc.username, username);
		strcpy(newacc.password, password);
		accounts[index] = newacc;
		index++;
	}
	fclose(f);
	mkdir("FILES", 0777);
		FILE* fs = fopen("files.tsv", "a");
	int indexfs = 0;
	fclose(fs);
	
	accountn = index;
	accountwrite = 0;
	filewrite = 0;
	
	connected = 0;
	connectedwrite = 0;

	pthread_t serveThreads[2];
	int client[2];
	for (int i = 0; i < 2; i++) {
		client[i] = accept(sock, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (client[i] < 0) {
			perror("Cannot accept client.\n");
			i--;
			continue;
		}
		printf("Accepted client.\n");
		clientdata c;
		c.client = client[i];
		c.skiplogin = 0;
		pthread_create(&serveThreads[i], NULL, serve, (void*)&c);
	}
	
		clientdata c1, c2;
		c1.client = client[0];
		c1.skiplogin = 1;
		c2.client = client[1];
		c2.skiplogin = 1;
		pthread_create(&serveThreads[0], NULL, serve, (void*)&c1);
		pthread_create(&serveThreads[1], NULL, serve, (void*)&c2);
	
}
