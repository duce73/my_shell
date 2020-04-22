# my_shell

This is my OS homework;

## Homework demand
1. The shell must support the following internal commands:


i. cd <directory> - Change the current default directory to <directory>. If the <directory> argument is not present, report the current directory. If the directory does not exist an appropriate error should be reported. This command should also change the PWD environment variable.
  
  
ii. clr - Clear the screen.


iii. dir <directory> - List the contents of directory <directory>.
  
 
iv. environ - List all the environment strings.


v. echo <comment> - Display <comment> on the display followed by a new line (multiple spaces/tabs may be reduced to a single space).
  

vi. help - Display the user manual using the more filter.


vii. pause - Pause operation of the shell until 'Enter' is pressed.


viii. quit - Quit the shell.


ix. The shell environment should contain shell=<pathname>/myshell where <pathname>/myshell is the full path for the shell executable (not a hardwired path back to your directory, but the one from which it was executed).
  
  
2. All other command line input is interpreted as program invocation, which should be done by the shell forking and execing the programs as its own child processes. The programs should be executed with an environment that contains the entry:
parent=<pathname>/myshell where <pathname>/myshell is as described in ix. above.
  
  
3. The shell must be able to take its command line input from a file. That is, if the shell is invoked with a command line argument:
myshell batchfile  then batchfile is assumed to contain a set of command lines for the shell to process. When the end-of-file is reached, the shell should exit. Obviously, if the shell is invoked without a command line argument, it solicits input from the user via a prompt on the display.


4. The shell must support i/o-redirection on either or both stdin and/or stdout.That is, the command line
programname arg1 arg2 < inputfile > outputfile
will execute the program programname with arguments arg1 and arg2, the stdin FILE stream replaced by inputfile and the stdout FILE stream replaced by
outputfile.
stdout redirection should also be possible for the internal commands dir, environ, echo, & help.
With output redirection, if the redirection character is > then the outputfile is created if it does not exist and truncated if it does. If the redirection token is >> then outputfile is created if it does not exist and appended to if it does.


5. The shell must support background execution of programs. An ampersand (&) at the end of the command line indicates that the shell should return to the command line prompt immediately after launching that program.


6. The command line prompt must contain the pathname of the current directory.
Note: You can assume that all command line arguments (including the redirection symbols, <, > & >> and the background execution symbol, &) will be delimited from other command line arguments by white space - one or more spaces and/or tabs (see the command line in 4. above).

## Done
基于C语言实现在Linux环境下的shell程序；支持部分内置命令与外部命令。

## 内部命令

通过C语言实现了8个内置命令，cd、echo、clr、dir、help、
environ、puase、exit

```js
//跳转到指定的目录
cd <目录>
//字符回显
echo <string>
//清屏功能
clr
//列出当前目录下的文件
dir
//获取可用的内置命令
help
//列出当前的环境变量
environ
//暂停程序直至输入Enter
pause
//退出my_shell
exit
```

## 外部命令

在my_shell中使用创建新的进程的方法实现外部命令，对于外部命令，使用fork( )函数与exec( )函数，使用fork函数复制当前进程，并且使用exec函数的变体execvp( )函数传入参数来执行外部命令。

## 基本思路

从输入中读取字符串并且进行处理、匹配、执行（内置命令或者外部命令），基于循环的方式来完成整个shell。

## 存在问题

1. 不支持转义输入
2. 不支持输入输出的重定向
3. 对于大量输入的内存分配问题

注：my_shell参考 [github链接](https://github.com/brenns10/lsh) 所完成。
