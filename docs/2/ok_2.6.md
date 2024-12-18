# Practice Problem 2.6  (solution page 145)
Using `show_int` and `show_float`, we determine that the integer `3510593` has hexa­decimal representation `0x00359141`, while the floating-point number `3510593.0` has hexadecimal representation `0x4A564504`.

A. Write the binary representations of these two hexadecimal values.

B. Shift these two strings relative to one another to maximize the number of matching bits. How many bits match?

C. What parts of the strings do not match?

## Solution:
A.

integer: `0000 0000 0011 0101 1001 0001 0100 0001` 

float: `0100 1010 0101 0110 0100 0101 0000 0100`

B. 21 bits
```
integer: 00000000001101011001000101000001
float:     01001010010101100100010100000100
                    *********************
```

C. 
- The first non-zero bit of integer
- The leftmost and rightmost bits of float
    - The floating-point number has some nonzero high-order bits that do not match those of the integer