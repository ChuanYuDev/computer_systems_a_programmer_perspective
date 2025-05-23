# Practice Problem 2.32 (solution page 153)

You are assigned the task of writing code for a function `tsub_ok`,with arguments `x` and `y`, that will return `1` if computing `x - y` does not cause overflow. Having just written the code for Problem 2.30, you write the following:

```c
/* Determine whether arguments can be subtracted without overflow */
/* WARNING: This code is buggy. */
int tsub_ok(int x, int y) {
    return tadd_ok(x, -y) ;
}
```

For what values of `x` and `y` will this function give incorrect results? Writing a correct version of this function is left as an exercise (Problem 2.74).

## Solution:

When $y = -2^{w - 1}$, this function will give incorrect results

When $y = -2^{w - 1}$, $-y = -2^{w - 1}$

When $x \geq 0$, $x-y$ overflow, but $x + (-y)$ not overflow

When $x < 0$, $x-y$ not overflow, but $x + (-y)$ overflow

However, their result is the same

Assume $x = 1, x - y = 1 + 2^{w - 1}$, positive overflow, $x -_w^t y = 1 + 2^{w - 1} - 2^w = -2^{w - 1} + 1$

On the other hand, $x + (-y) = -2^{w - 1} + 1$



