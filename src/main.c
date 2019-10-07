#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 // The maximum length command
#define MAX_BUFSIZE 128

// move oldfd to newfd
static void redirect(int oldfd, int newfd) {
	if (oldfd != newfd) {
		if (dup2(oldfd, newfd) != -1)
			close(oldfd);
		else{
			perror("dup2");
		}
	}
}

int normalize_arg(char **argv, int argc,
					int *wait_child_proc,
					char *file_name_in,
					char *file_name_out){

}

int exec_argument(char **args, int fd_in, int fd_out){
	int file_desc_in = fd_in;
	int file_desc_out = fd_out;

	redirect(file_desc_in, STDIN_FILENO);
	redirect(file_desc_out, STDOUT_FILENO);

	if (execvp(args[0], args) < 0) {     
		printf("*** ERROR: exec failed\n");
		exit(1);
	}
}

int exec_command(char *command, int *fd_in, int fd[2]){
	int argc;
	char **argv = str_split(command, " ", &argc);

	int wait_child_proc = 1;
	int file_desc_out = -1;
	int saved_stdout = -1;
	int fd_out = fd[1];
	int pid;
	char *file_name_in = NULL;
	char *file_name_out = NULL;
	char **args = (char**)malloc((argc + 1) * sizeof(char*));
	for (int i = 0, k = 0; i < argc; i++, k++){
		if (args[k]){
			free(args[k]);
		}
		args[k] = (char*)malloc(sizeof(argv[i]));
		strcpy(args[k], argv[i]);
		if (i + 1 < argc && args[k][0] == '<'){
			k--;
			continue;
		}
		if (k > 0 && args[k - 1][0] == '>'){
			free(file_name_out);
			file_name_out = (char*)malloc(sizeof(args[k]));
			strcpy(file_name_out, args[k]);
			free(args[k - 1]);
			free(args[k]);
			args[k - 1] = NULL;
			argc = k - 1;
			break;
		}
		if (args[k][0] == '&'){
			free(args[k]);
			args[k] = NULL;
			argc = k;
			wait_child_proc = 0;
			break;
		}
	}
	args[argc] = NULL;

	if (file_name_out){
		file_desc_out = open(file_name_out, O_WRONLY | O_APPEND);
		if (file_desc_out < 0){
			printf("Open file failed\n");
		}else{
			saved_stdout = dup(STDOUT_FILENO);
			fd_out = file_desc_out;
		}
	}

	if (fd_out != STDOUT_FILENO){
		if ((pid = fork()) == -1){
			perror("fork");
		}else if (pid == 0){ // child process
			close(fd[0]);
			// exec_command(commands[i], fd_in, fd[1]);
			exec_argument(args, (int)*fd_in, fd_out);
		}else{ // parent
			if (wait_child_proc){
				waitpid(pid, NULL, 0);
				// while(wait(&status) != pid) {
				// 	/* code */
				// }
			}
			close(fd[1]);
			if ((int)*fd_in != STDIN_FILENO){
				close(*fd_in);					
			}
			*fd_in = (int)fd[0];
		}
	}else{
		if ((pid = fork()) == -1){
			perror("fork");
		}else if (pid == 0){ // child process
			exec_argument(args, (int)*fd_in, STDOUT_FILENO);
		}else{ // parent
			if (wait_child_proc){
				waitpid(pid, NULL, 0);
				// while(wait(&status) != pid) {
				// 	/* code */
				// }
			}
			if ((int)*fd_in != STDIN_FILENO){
				close((int)*fd_in);					
			}
		}
	}
	if (saved_stdout >= 0){
		redirect(saved_stdout, STDOUT_FILENO);
	}
}

int exec_pipe(char **commands, int num_command){
	int i = 0, fd_in = STDIN_FILENO, status;
	int fd[2];
	pid_t pid;
	for ( ; i < num_command - 1; i++){
		if (pipe(fd) == -1){
			perror("pipe");
		}else{
			exec_command(commands[i], &fd_in, fd);
		}
	}
	fd[1] = STDOUT_FILENO;
	exec_command(commands[i], &fd_in, fd);
}

int main()
{
	char **commands = NULL;
	int num_command = 0;
	char *prev_buffer = NULL;
	int should_run = 1;

	while (should_run){
		printf("\nosh>");
		fflush(stdout);

		char *buffer;
		size_t bufsize = MAX_BUFSIZE;
		buffer = (char*)malloc(MAX_BUFSIZE * sizeof(char));
		if (buffer == NULL){
			perror("Unable to allocate buffer");
			free(buffer);
			exit(1);
		}
		fgets(buffer, MAX_BUFSIZE, stdin);
		if ((bufsize < 0) || (bufsize && buffer[0] == 'q')){
			free(buffer);
			break;
		}
		buffer = trimwhitespace(buffer);

		if (strlen(buffer) == 0){
			continue;
		}

		if (strlen(buffer) >= 2 && buffer[0] == '!' && buffer[1] == '!'){
			if (!prev_buffer){
				printf("No commands inhistory.\n");
				continue;
			}

			if (buffer){
				free(buffer);				
			}
			buffer = (char*)malloc(sizeof(prev_buffer));
			strcpy(buffer, prev_buffer);

			printf("%s\n", prev_buffer);

		}else{
			if (prev_buffer){
				free(prev_buffer);				
			}
			prev_buffer = (char*)malloc(sizeof(buffer));
			strcpy(prev_buffer, buffer);
		}
		
		commands = str_split(buffer, "|", &num_command);
		// for (int i = 0; i < num_command; i++){
		// 	printf("%s\n", commands[i]);
		// }

		exec_pipe(commands, num_command);

		free(buffer);
	}
	return 0;
}