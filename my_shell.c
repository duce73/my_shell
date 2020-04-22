#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

//内置函数的声明
int my_shell_cd(char **args);
int my_shell_help(char **args);
int my_shell_exit(char **args);
int my_shell_clr(char **args);
int my_shell_echo(char **args);
int my_shell_dir(char **args);
int my_shell_pause(char **args);
int my_shell_environ(char **args);

//使用函数体指针的写法，便于功能的扩充以及后续的维护
char *builtin_str[] = {
  	"cd",
  	"help",
  	"exit",
	"clr",
	"echo",
	"dir",
	"pause",
	"environ"
};

int (*builtin_func[]) (char **) = {
  	&my_shell_cd,
  	&my_shell_help,
  	&my_shell_exit,
	&my_shell_clr,
	&my_shell_echo,
	&my_shell_dir,
	&my_shell_pause,
	&my_shell_environ
};

int my_shell_numbuiltin() {
  	return sizeof(builtin_str) / sizeof(char *);
}

//实现内置命令cd
int my_shell_cd(char **args){
  	if (args[1] == NULL) {
    	fprintf(stderr,"my_shell wraning1: input argument to\"cd\"\n");
  	} else {
    	if (chdir(args[1]) != 0) {
      		perror("my_shell");
    	}
  	}
  	return 1;
}

//实现help命令
int my_shell_help(char **args){
  	int i;
  	printf("hello, this is my_shell power by DC:\n");
  	printf("Input those builtin commands.\n");
  	for (i = 0; i < my_shell_numbuiltin(); i++) {
    	printf("  %s\n", builtin_str[i]);
  	}
  	return 1;
}

//exit
int my_shell_exit(char **args){
  	return 0;
}

//清屏
int my_shell_clr(char **args){
	printf("%s","\033[1H\033[2J");
	return 1;
}

//字符回显
int my_shell_echo(char **args){
	int position = 1;

	while(args[position] != NULL){
		printf("%s ",args[position]);
		position++;
	}
	printf("\n");
	return 1;
}

//暂停功能
int my_shell_pause(char **args){
	char c;
	c = getchar();
	while((c != EOF) && c != '\n'){
		c = getchar();
	}
	return 1;
}

//列出环境变量
int my_shell_environ(char **args){
	int i;
	for (i = 0; i < my_shell_numbuiltin(); i++) {
    	printf("  %s\n", builtin_str[i]);
  	}
  	return 1;
}

//实现外部命令，通过fork函数与exec函数的配合
int my_shell_fun(char **args){
  	int pid;
  	int status;

  	pid = fork();
  	if (pid == 0) {
    	//子进程
    	if (execvp(args[0], args) == -1) {
      		perror("my_shell warning2:");
   	 	}
    	exit(EXIT_FAILURE);
  	} else if (pid < 0) {
    	//fork出错
    	perror("my_shell warning3:");
  	} else {
    	//父进程等待子进程
    	do {
      		waitpid(pid, &status, WUNTRACED);
    	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
  	}
  	return 1;
}

//列出当前目录下的文件与文件夹
int my_shell_dir(char **args){
	args[0] = "ls";
	return my_shell_fun(args);
}

//实现了内置命令与外部命令的结合，并且对于输入为空也做了处理
int my_shell_exec(char **args){
  	int i;
  	if (args[0] == NULL) {
    	return 1;
  	}
  	for (i = 0; i < my_shell_numbuiltin(); i++) {
    	if (strcmp(args[0], builtin_str[i]) == 0) {
      		return (*builtin_func[i])(args);
    	}
  	}
  	return my_shell_fun(args);
}

//从标准输入中读取一行
#define READ 1000

char *my_shell_readline(void){
  	int position = 0;
 	char *buffer = malloc(sizeof(char) * READ);
  	int c;

  	if (!buffer) {
    	fprintf(stderr, "my_shell: allocation error\n");
    	exit(EXIT_FAILURE);
  	}

  	while (1) {
    	c = getchar();

    	if (c == EOF) {
      		exit(EXIT_SUCCESS);
    	} else if (c == '\n') {
      		buffer[position] = '\0';
      		return buffer;
    	} else {
      		buffer[position] = c;
    	}
    	position++;
  	}
}

//将读取的进行处理，通过strtok函数
#define SPLIT 100
#define DELIM " \t\r\n\a"

char **my_shell_split(char *line){
  	int position = 0;
  	char **tokens = malloc(SPLIT * sizeof(char*));
  	char *token;

	//内存分配出错
  	if (!tokens) {
    	fprintf(stderr, "my_shell: allocation error\n");
    	exit(EXIT_FAILURE);
  	}

  	token = strtok(line, DELIM);
  	while (token != NULL) {
    	tokens[position] = token;
    	position++;
    	token = strtok(NULL, DELIM);
  	}
  	tokens[position] = NULL;
  	return tokens;
}

//获取当前的路径path
#define SIZE 100

char *getpwd(){
	char *path;
	path = (char*)malloc(SIZE);
	getcwd(path,100);
	return path;
}

//my_shell的主体循环
void my_shell_loop(void){
  	char *line;
  	char **args;
	char *path;
  	int status;

  	do {
		path = getpwd();
		printf("%s ",path);
		printf("my_shell>>");
    	line = my_shell_readline();
    	args = my_shell_split(line);
    	status = my_shell_exec(args);

    	free(line);
    	free(args);
  	} while (status);
}

int main(int argc, char **argv){
  	my_shell_loop();
  	return EXIT_SUCCESS;
}
