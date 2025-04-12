# Shell Lab

## Table of contents
- [Introduction](#introduction)
- [Duration](#duration)
- [Chapter](#chapter)
- [Solution](#solution)
    - [Data structure](#data-structure)
    - [Function](#function)
    - [Trace01](#trace01)
    - [Trace02](#trace02)
    - [Trace03](#trace03)
    - [Trace04](#trace04)
    - [Trace05](#trace05)
    - [Trace06](#trace06)
    - [Trace07](#trace07)
    - [Trace08](#trace08)
    - [Trace09](#trace09)
    - [Trace10](#trace10)
    - [Trace11](#trace11)
    - [Trace12](#trace12)
    - [Trace13](#trace13)
    - [Trace14](#trace14)
    - [Trace15](#trace15)
    - [Trace16](#trace16)

## Introduction
- Students implement their own simple Unix shell program with job control, including the `ctrl-c` and `ctrl-z` keystrokes, `fg`, `bg`, and `jobs` commands

- This is the students' first introduction to application level concurrency, and gives them a clear idea of Unix process control, signals, and signal handling

## Duration
- 2025/3/22 - Due: 2025/4/14

## Chapter
- Chapter 8: Exceptional Control Flow

## Solution
### TODO
- Why make test08 kill error?

### Data structure
- `job_t`
    - Each job can be identified by PID or JID 
        - `%5` denotes JID 5
        - `5` denotes PID 5

    - `pid` is the initial child process ID forked by `tsh` which is also the process group ID

    - `jid` starts from 1

### Function
- `main`
    - Initialize jobs

    - `while(1)`
        - Read the command line from `stdin`
        - If `EOF` is received, terminate the program (`trace01`)

        - `eval(char *cmdline)`

- `eval(char *cmdline)`
    - `parseline`

    - Skip the blank line
        - If the first element of `argv` is `NULL`, return immediately
    
    - If the command is builtin
        - `quit` (`trace02`)
            - Exit the program
        
        - `jobs` (`trace05`)
            - `listjobs`

        - `bg` or `fg` 
            - `do_bgfg`

    - If the command is not builtin
        - Block `SIGCHLD` signal
        - Fork a child process
            - Create unique process group (`trace07`)
            - Unblock `SIGCHLD` signal
            - Execute the executable in the context of the child
        
        - Block all the signals
        - Add jobs
        - Get jid

        - **If add too many jobs, how to handle?**
        - Unblock `SIGCHLD` signal

        - If foreground job (`trace03`)
            - `waitfg`
        
        - else (`trace04`)
            - print background job information

- `parseline`
    - Build `argv`
        - Check the arguments number

    - The last element of `argv` is `NULL`
    - If background job, return 1 
    - Otherwise, return 0

- `do_bgfg`
    - Get job
        ```c
        char *buf = argv[1];
        struct job_t *job_ptr;
        int jid;
        pid_t pid;

        if (*buf == '%')
        {
            buf++;
            jid = atoi(buf);
            job_ptr = getjobjid(jobs, jid);
        }
        else
        {
            pid = atoi(buf);
            job_ptr = getjobpid(jobs, pid);
        }
        ```

    - If (job_ptr)
        - Send `SIGCONT` to the process group
        - If `fg`
            - job state changed to `FG`
            - `waitfg`
        
        - If `bg`
            - job state changed to `BG`

- `waitfg`

    - Block all the signals because we need to access the shared global data structure `jobs`

    - Use `fgpid` to check whether the current foreground job exists instead of using PID

        ```c
        while(fgpid(jobs))
            sigsuspend to unblock SIGCHLD and wait for the signal 
        ```

    - Unblock all the signals

- `sigchld_handler`
    - Save `errno`
    - Reap all the child processes (`options = WNOHANG|WUNTRACED`)
        - Block all the signals

        - If STOPPED (`WIFSTOPPED(status)`)
            - Print `job [jid] (pid) stopped by signal <signal num>` (`WSTOPSIG(status)`)

            - Change job state to `ST`

        - If TERMINATED (`WIFSIGNALED(status)`)
            - Print `job [jid] (pid) terminated by signal <signal num>` (`WTERMSIG(status)`)

            - Delete jobs

        - Unblock all the signals


    - Restore `errno`

- `sigint_handler` (`trace06`)
    - Function:
        - Catch `SIGINT` (ctrl-c)
        - Send it along to the foreground job

    - Save `errno`
    - Find the PID of current foreground job (`fgpid`)

    - If PID > 0
        - Send `SIGINT` to the foreground process job

    - Restore `errno`

- `sigtstp_handler` (`trace08`)
    - Function:
        - Catch `SIGTSTP` (ctrl-z)
        - Suspend the foreground job by sending it `SIGTSTP`
    
    - Save `errno`
    - Find the PID of current foreground job (`fgpid`)

    - If PID > 0
        - Send `SIGTSTP` to the foreground process job

    - Restore `errno`

- `sigquit_handler`
    - For the driver program
    - For the builin command `quit`, exit the program directly

- `Waitpid`
    - **Because `Waitpid` will be revoked in the signal handler, is it safe to use `fprintf` in `unix_error`?**

### Trace01
- Result
    ```
    make rtest01
    ./sdriver.pl -t trace01.txt -s ./tshref -a "-p"
    #
    # trace01.txt - Properly terminate on EOF.
    #
    ```

    ```
    make test01
    ./sdriver.pl -t trace01.txt -s ./tsh -a "-p"
    #
    # trace01.txt - Properly terminate on EOF.
    #
    ```

### Trace02
- Result:
    ```
    make rtest02
    ./sdriver.pl -t trace02.txt -s ./tshref -a "-p"
    #
    # trace02.txt - Process builtin quit command.
    #
    ```

    ```
    make test02
    ./sdriver.pl -t trace02.txt -s ./tsh -a "-p"
    #
    # trace02.txt - Process builtin quit command.
    #
    ```

### Trace03
- Result
    ```
    make rtest03
    ./sdriver.pl -t trace03.txt -s ./tshref -a "-p"
    #
    # trace03.txt - Run a foreground job.
    #
    tsh> quit
    ```

    ```
    make test03
    ./sdriver.pl -t trace03.txt -s ./tsh -a "-p"
    #
    # trace03.txt - Run a foreground job.
    #
    tsh> quit
    ```

### Trace04  
- Result:
    ```
    make rtest04
    ./sdriver.pl -t trace04.txt -s ./tshref -a "-p"
    #
    # trace04.txt - Run a background job.
    #
    tsh> ./myspin 1 &
    [1] (881037) ./myspin 1 &
    ```

    ```
    make test04
    ./sdriver.pl -t trace04.txt -s ./tsh -a "-p"
    #
    # trace04.txt - Run a background job.
    #
    tsh> ./myspin 1 &
    [1] (881077) ./myspin 1 &
    ```

### Trace05
- Result:
    ```
    make rtest05
    ./sdriver.pl -t trace05.txt -s ./tshref -a "-p"
    #
    # trace05.txt - Process jobs builtin command.
    #
    tsh> ./myspin 2 &
    [1] (881420) ./myspin 2 &
    tsh> ./myspin 3 &
    [2] (881422) ./myspin 3 &
    tsh> jobs
    [1] (881420) Running ./myspin 2 &
    [2] (881422) Running ./myspin 3 &
    ```

    ```
    make test05
    ./sdriver.pl -t trace05.txt -s ./tsh -a "-p"
    #
    # trace05.txt - Process jobs builtin command.
    #
    tsh> ./myspin 2 &
    [1] (881476) ./myspin 2 &
    tsh> ./myspin 3 &
    [2] (881478) ./myspin 3 &
    tsh> jobs
    [1] (881476) Running ./myspin 2 &
    [2] (881478) Running ./myspin 3 &
    ```

### Trace06
- Result:
    ```
    make rtest06
    ./sdriver.pl -t trace06.txt -s ./tshref -a "-p"
    #
    # trace06.txt - Forward SIGINT to foreground job.
    #
    tsh> ./myspin 4
    Job [1] (881796) terminated by signal 2
    ```

    ```
    make test06
    ./sdriver.pl -t trace06.txt -s ./tsh -a "-p"
    #
    # trace06.txt - Forward SIGINT to foreground job.
    #
    tsh> ./myspin 4
    job [1] (882461) terminated by signal 2
    ```

### Trace07
- Result:
    ```
    make rtest07
    ./sdriver.pl -t trace07.txt -s ./tshref -a "-p"
    #
    # trace07.txt - Forward SIGINT only to foreground job.
    #
    tsh> ./myspin 4 &
    [1] (882720) ./myspin 4 &
    tsh> ./myspin 5
    Job [2] (882722) terminated by signal 2
    tsh> jobs
    [1] (882720) Running ./myspin 4 &
    ```

    ```
    make test07
    ./sdriver.pl -t trace07.txt -s ./tsh -a "-p"
    #
    # trace07.txt - Forward SIGINT only to foreground job.
    #
    tsh> ./myspin 4 &
    [1] (882796) ./myspin 4 &
    tsh> ./myspin 5
    job [2] (882798) terminated by signal 2
    tsh> jobs
    [1] (882796) Running ./myspin 4 &
    ```

### Trace08
- Result:
    ```
    make rtest08
    ./sdriver.pl -t trace08.txt -s ./tshref -a "-p"
    #
    # trace08.txt - Forward SIGTSTP only to foreground job.
    #
    tsh> ./myspin 4 &
    [1] (883948) ./myspin 4 &
    tsh> ./myspin 5
    Job [2] (883950) stopped by signal 20
    tsh> jobs
    [1] (883948) Running ./myspin 4 &
    [2] (883950) Stopped ./myspin 5
    ```

    ```
    make test08
    ./sdriver.pl -t trace08.txt -s ./tsh -a "-p"
    #
    # trace08.txt - Forward SIGTSTP only to foreground job.
    #
    tsh> ./myspin 4 &
    [1] (889865) ./myspin 4 &
    tsh> ./myspin 5
    job [2] (889867) stopped by signal 20
    tsh> jobs
    [1] (889865) Running ./myspin 4 &
    [2] (889867) Stopped ./myspin 5 
    ```
### Trace09
### Trace10
### Trace11
### Trace12
### Trace13
### Trace14
### Trace15
### Trace16