#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
#define DELTM "\t\n\r\a"

int my_cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr,"please input argument to \"cd\"\n");
    } else{
        if(chdir(args[1]) != 0){
            fprintf(stderr,"my_shell1");
        }
    }
    return 1;
} 

int my_exec(char **args){
	//null
	 if(args[0] == NULL){
        return 1;
    }
    //cd
    if(strcmp(args[0],"cd") == 0){
        return my_cd(args);
    }
    //exit
    if(strcmp(args[0],"exit") == 0){
        return 0;
    }

	pid_t pid;
    int flag;

	pid = fork();
  	if (pid == 0) {
    // Child process
    	if (execvp(args[0], args) == -1) {
      		perror("my_shell2");
    	}
    	exit(EXIT_FAILURE);
  	} else if (pid < 0) {
    	// Error forking 
    	perror("my_shell3");
  	} else {
    	// Parent process
    	do {
      		waitpid(pid, &flag, WUNTRACED);
    	} while (!WIFEXITED(flag) && !WIFSIGNALED(flag));
  }
  return 1;
}


char **split_line(char *line){
	int num = 0;
	char **args = malloc(MAX_SIZE*sizeof(char*));
	char *arg;
	arg = strtok(line,DELTM);
	while(arg != NULL){
		args[num] = arg;
		num++;
		arg = strtok(NULL,DELTM);
	}
	args[num] = NULL;
	return args;
}



char *read_line(void){
	int num = 0;
	char *line=malloc(sizeof(char)*MAX_SIZE);
	int value;

	while(1){
		value = getchar();

		if(value == EOF){
			exit(EXIT_FAILURE);
		}
		else if(value == '\n'){
			line[num] = '\0';
			return line;
		}
		else{
			line[num] = value;
		}
		num++;
	}
}


void my_shell_loop(void){
	char *line;
	char **args;
	int flag;

	do{
		printf("my_shell$:");
		line = read_line();
		args = split_line(line);
		flag = my_exec(args);

		free(line);
		free(args);
	}while(flag);
}

int main(int argc, char *argv[]){

	my_shell_loop();
	return EXIT_SUCCESS;
}
