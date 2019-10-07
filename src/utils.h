#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *trimwhitespace(char *str)
{
	char *end;

	// Trim leading space
	while(isspace((unsigned char)*str)) str++;

	if(*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) end--;

	// Write new null terminator character
	end[1] = '\0';

	return str;
}

char **str_split(char *str, const char *delim, int *numSplits){
	char *tmp = malloc(sizeof(str));
	strcpy(tmp, str);
	size_t count = 0, i = 0;

	char *token = strtok(tmp, delim);
	while(token) {
		count++;
	    token = strtok(NULL, delim);
	}

	char **result = (char**)malloc(sizeof(char*) * count);

	char *ptr = strtok(str, delim);

	while(ptr != NULL)
	{
		result[i] = malloc(sizeof(ptr));
		strcpy(result[i], ptr);
		i++;
		ptr = strtok(NULL, delim);
	}
	*numSplits = (int)count;

	return result;
}

void print_command(char **argv, int argc){
	printf("%d\n", argc);
	printf("\nosh>");
	for (int i = 0; i < argc; i++){
		printf(" %s", argv[i]);
	}
	printf("\n");
}

void print_null_terminated_command(char **args, int wait_child_proc, char *file_name_out){
	// printf("%d\n", argc);
	printf("\nosh>");
	for (int i = 0; args[i]; i++){
		printf(" %s", args[i]);
	}
	if (file_name_out){
		printf(" > %s", file_name_out);
	}
	if (!wait_child_proc){
		printf(" &");
	}
	printf("\n");
}

// Copy command from (argv2, argc2) to (argv1, argc1)
void copy_command(char **argv1, int *argc1, char **argv2, const int *argc2){
	for (int i = 0; i < *argc1; i++){
		if (argv1[i]){
			free(argv1[i]);			
		}
	}
	free(argv1);
	*argc1 = *argc2;
	argv1 = (char**)malloc((*argc1) * sizeof(char*));
	for (int i = 0; i < *argc1; i++){
		argv1[i] = (char*)malloc(sizeof(argv2[i]));
		strcpy(argv1[i], argv2[i]);
		printf("%s\n%s\n", argv1[i], argv2[i]);
	}
}