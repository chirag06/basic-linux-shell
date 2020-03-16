### Basic Shell or Command Line Interpreter

**Project Description**

A simple shell called blazersh that can handle keyboard interrupts like Control-Z and Control-x. It also  provides the following functionality:



1) **list** : Lists all the files in the current directory.
The default directory is the directory one where the shell
program was invoked or the directory specified by the PWD environment variable.

2) **cd \<PATH>** : Changes the current directory to the directory mentioned in the command. Default directory would be the directory one where the shell program was invoked.

3) **help** : Displays all the internal commands and a short description on how to use them.

4) **history** : Displays all the previous command entered into the shell program.

5) **quit** : Exits the shell.

6) **jobs** : Lists the processes along with their corresponding process id that were stopped when the user pressed Control-Z. Whenever the user enters the Control-Z character, the simple shell program must keep track of processes that are stopped and the jobs command should list such processes.

7) **continue \<pid>** : Sends the continue signal to the process with process id \<pid>. User can obtain the job \<pid> to send the continue signal using the jobs commands.

**Keyboard interrupts handled:**   

1) **Ctrl-Z** : When the user presses Ctrl key and "Z" key together the process is suspended by sending a SIGSTOP signal to the child process.

2) **Control-C** : When the user presses Ctrl key and "C" key together the process is killed by sending a SIGINT signal to the child process.  

**Getting Started**

1) Build the exceutable file by running the make file
```
make
```
2) Run the executable
```
./blazersh
```   

Run Cases

```
blazersh> ./myprog
^CProcess with pid=12432 is interrupted
blazersh>
blazersh> ./myprog
^ZProcess with pid=12433 is stopped
blazersh>
blazersh> jobs
PID     Process
12433   ./myprog
blazersh>
blazersh> continue 12433
Process 12433 started
The program should continue and complete normally
blazersh> jobs
blazersh>
blazersh> ./myprog arg1 arg2 < inputfile > outputfile
```

Author: Chirag Shetty

Contact information
Email: chirag24@uab.edu
