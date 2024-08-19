# Practice Problem 8.6 (solution page 797)
Write a program called `myecho` that prints its command-line arguments and environment variables. For example:

```
linux› ./myecho arg1 arg2
Command-line arguments:
    argv[ 0]: myecho
    argv[ 1]: arg1
    argv[ 2]: arg2

Environment variables:
    envp[ 0]: PWD=/usr0/droh/ics/code/ecf
    envp[ 1]: TERM=emacs
    .
    .
    .
    envp[25]: USER=droh
    envp[26]: SHELL=/usr/local/bin/tcsh
    envp[27]: HOME=/usr0/droh
```

## Solution:

Code: [myecho](../../problems/8/8.6.c)