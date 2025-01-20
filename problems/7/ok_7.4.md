# Practice Problem 7.4 (solution page 718)
This problem concerns the relocated program in Figure 7.12(a).

A. What is the hex address of the relocated reference to `sum` in line 5?

B. What is the hex value of the relocated reference to `sum` in line 5?

## Solution:

- A. 
    - The hex address is `0x4004df`

- B.
    - The value of the relocated reference is `0x05` which uses 32-bit PC-relative address
    - The effective address is `0x4004e3 + 0x05 = 0x4004e8` which is the start of `sum`