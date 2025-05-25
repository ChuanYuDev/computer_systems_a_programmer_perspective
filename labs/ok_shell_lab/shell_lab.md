# Shell Lab

## Table of contents
- [Introduction](#introduction)
- [Duration](#duration)
- [Chapter](#chapter)
- [Solution](#solution)
    - [Data structure](#data-structure)
    - [Function](#function)
    - [Result](#result)

- [Remaining problems](#remaining-problems)

## Introduction
- Students implement their own simple Unix shell program with job control, including the `ctrl-c` and `ctrl-z` keystrokes, `fg`, `bg`, and `jobs` commands

- This is the students' first introduction to application level concurrency, and gives them a clear idea of Unix process control, signals, and signal handling

## Duration
- 2025/3/22 - 2025/4/14

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

### Result
- All the results are printed in [`tsh.out`](./shlab-handout/tsh.out) which we can compare with [`tshref.out`](./shlab-handout/tshref.out) via other tools such as https://text-compare.com

## Remaining problems
- How to handle add too many jobs? Refer to `tshref`?