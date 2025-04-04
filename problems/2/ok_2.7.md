# Practice Problem 2.7  (solution page 145)
What would be printed as a result of the following call to `show_bytes`?

```c
const char *s = "abcdef";
show_bytes((byte_pointer) s, strlen(s));
```

Note that letters `a` through `z` have ASCII codes `0x61` through `0x7A`.

## Solution:
- A string in C is encoded by an array of characters terminated by the null (having value 0) character.
    - `s` is the pointer having the address of `a`
    - `strlen(s)` returns the number of characters excluding the null character

    - `show_bytes` will print `a`, `b`, `c`, `d`, `e`, `f` hexadecimal representations.

- Print: `61 62 63 64 65 66`