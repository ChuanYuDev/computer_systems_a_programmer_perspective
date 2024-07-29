# Practice Problem 7.5 (solution page 718)
Consider the call to function swap in object file `m.o` (Figure 7.5).

```
9: e8 00 00 00 00       callq e <main+0xe>      swap()
```

with the following relocation entry:

```
r.offset    = 0xa
r.symbol    = swap
r.type      = R_X86_64_PC32
r.addend    = -4
```

Now suppose that the linker relocates `.text` in `m.o` to address `0x4004d0` and `swap` to address `0x4004e8`. Then what is the value of the relocated reference to `swap` in the `callq` instruction?

## Solution:
- Now, suppose that the linker has determined that

    ```
    ADDR(s) = ADDR(.text) = 0x4004d0
    ADDR(r.symbol) = ADDR(swap) = 0x4004e8
    ```

- Using the algorithm in Figure 7.10, the linker first computes the run-time address of the reference (line7):

    ```
    refaddr = ADDR(s) + r.offset
            = 0x4004d0 + 0xa
            = 0x4004da
    ```

- It then updates the reference so that it will point to the `swap` routine at run time (line 8):

    ```
    *refptr = (unsigned)(ADDR(r.symbol) + r.addend - refaddr)
            = (unsigned)(0x4004e8       + (-4)     - 0x4004da)
            = (unsigned)(0xa)
    ```

- In the resulting executable object file, the `call` instruction has the following relocated form:

    ```
    4004d9: e8 0a 00 00 00      callq 4004e8 <swap>      swap()
    ```

- The value of the relocated reference to `swap` is `0xa`