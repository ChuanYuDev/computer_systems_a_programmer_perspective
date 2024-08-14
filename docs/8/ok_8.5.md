# Practice Problem 8.5 (solution page 797)
Write a wrapper function for sleep, called snooze, with the following interface:

```c
unsigned int snooze(unsigned int secs);
```

The `snooze` function behaves exactly as the sleep function, except that it prints a message describing how long the process actually slept:

```
Slept for 4 of 5 secs.
```

## Solution:

Code: [snooze](../../problems/8/8.5.c)