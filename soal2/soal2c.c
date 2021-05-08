#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int main(){
	int status;
	int fd1[2];
    int fd2[2];
	
	if(pipe(fd1) == -1) {
		die("pipe");
	}
	
	if(pipe(fd2) == -1) {
		die("pipe");
	}
	
	if(fork() == 0) {
		close(fd1[0]);
		dup2(fd1[1], STDOUT_FILENO);
		char *argv[] = {"ps", "aux", NULL};
		execv("/bin/ps", argv);
	} else {
		while((wait(&status)) > 0);

        if(fork() == 0) {
            close(fd1[1]);
		    dup2(fd1[0], STDIN_FILENO);
            close(fd2[0]);
		    dup2(fd2[1], STDOUT_FILENO);
            char *argv[] = {"sort", "-nrk", "3,3", NULL};
		    execv("/bin/sort", argv);
        }else {
            close(fd1[1]);
		    close(fd2[1]);
            while((wait(&status)) > 0);
            dup2(fd2[0],STDIN_FILENO);
            char *argv[] = {"head", "-5", NULL};
		    execv("/bin/head", argv);
        }
	}
}