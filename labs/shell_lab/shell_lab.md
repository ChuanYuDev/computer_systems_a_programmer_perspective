# Shell Lab

## Table of contents
- [Introduction](#introduction)
- [Duration](#duration)
- [Chapter](#chapter)
- [Solution](#solution)
    - [Trace01](#trace01)
    - [Trace02](#trace02)
    - [Trace03](#trace03)
    - [Trace04]
    - [Trace05]
    - [Trace06]
    - [Trace07]
    ### Trace08
    ### Trace09
    ### Trace10
    ### Trace11
    ### Trace12
    ### Trace13
    ### Trace14
    ### Trace15
    ### Trace16

## Introduction
- Students implement their own simple Unix shell program with job control, including the `ctrl-c` and `ctrl-z` keystrokes, `fg`, `bg`, and `jobs` commands

- This is the students' first introduction to application level concurrency, and gives them a clear idea of Unix process control, signals, and signal handling

## Duration
- 2025/3/22 - Due: 2025/4/14

## Chapter
- Chapter 8: Exceptional Control Flow

## Solution
### Data structure
- `job_t`
    - Each job can be identified by PID or JID 
        - `%5` denotes JID 5
        - `5` denotes PID 5

    - `pid` is the initial child process ID forked by `tsh` which is also the process group ID

    - `jid` starts from 1


### Function
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
            - `waitfg`

- `waitfg`

    - Block all the signals because we need to access the shared global data structure `jobs`

    - Use `fgpid` to check whether the current foreground job exists instead of using PID

        ```c
        while(fgpid(jobs))
            sigsuspend to unblock SIGCHLD and wait for the signal 
        ```

    - Unblock all the signals

- `eval(char *cmdline)`
    - **How to deal with the mask?**
    - Parse the command line 
        - Build `argv`
            - Check the arguments number

        - The last element of `argv` is `NULL`
        - If background job, return 1 
        - Otherwise, return 0
    
    - Skip the blank line
        - If the first element of `argv` is `NULL`, return immediately
    
    - If the command is builtin
        - `quit` 
            - Exit the program (`trace02.txt`)
        
        - `jobs`
            - `listjobs`

        - `bg` or `fg` 
            - `do_bgfg`

    - If the command is not builtin
        - Block `SIGCHLD` signal
        - Fork a child process
            - Create unique process group
            - Unblock `SIGCHLD` signal
            - Execute the executable in the context of the child
        
        - Block all the signals
        - Add jobs
        - Unblock `SIGCHLD` signal

        - If foreground job (`trace03.txt`)
            - `waitfg`

- `Waitpid`
    - **Because `Waitpid` will be revoked in the signal handler, is it safe to use `fprintf` in `unix_error`?**

- `sigchld_handler`
    - Save `errno`
    - Reap all the child processes
        - **No hang?** (`options = WNOHANG`)

        - Block all the signals
        - Delete jobs
        - Unblock all the signals

        - If `WIFSIGNALED(status)`
            - **Print a message with the job's PID and a description of the offending signal (`WTERMSIG(status)`)?**

    - Restore `errno`

- `sigint_handler`
    - Function:
        - Catch `SIGINT` (ctrl-c)
        - Send it along to the foreground job

    - Save `errno`
    - Find the PID of current foreground job (`fgpid`)

    - If PID > 0
        - Send `SIGINT` to the foreground process job

    - Restore `errno`

- `sigtstp_handler`
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

- `main`
    - Initialize jobs

    - `while(1)`
        - Read the command line from `stdin`
        - If `EOF` is received, terminate the program (`trace01.txt`)

        - `eval(char *cmdline)`

### Trace01
- Result

    ```
    make test01
    ./sdriver.pl -t trace01.txt -s ./tsh -a "-p"
    #
    # trace01.txt - Properly terminate on EOF.
    #
    ```

    ```
    make rtest01
    ./sdriver.pl -t trace01.txt -s ./tshref -a "-p"
    #
    # trace01.txt - Properly terminate on EOF.
    #
    ```

### Trace02
- Result:

    ```
    make test02
    ./sdriver.pl -t trace02.txt -s ./tsh -a "-p"
    #
    # trace02.txt - Process builtin quit command.
    #
    ```

    ```
    make rtest02
    ./sdriver.pl -t trace02.txt -s ./tshref -a "-p"
    #
    # trace02.txt - Process builtin quit command.
    #
    ```
### Trace03
- Result

    ```
    make test03
    ./sdriver.pl -t trace03.txt -s ./tsh -a "-p"
    #
    # trace03.txt - Run a foreground job.
    #
    tsh> quit
    ```

    ```
    make rtest03
    ./sdriver.pl -t trace03.txt -s ./tshref -a "-p"
    #
    # trace03.txt - Run a foreground job.
    #
    tsh> quit
    ```

### Trace04
### Trace05
### Trace06
### Trace07
### Trace08
### Trace09
### Trace10
### Trace11
### Trace12
### Trace13
### Trace14
### Trace15
### Trace16