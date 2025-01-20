# Practice Problem 4.21 (solution page 488)
Register ID `dstM` indicates the destination register for write port M, where `valM`, the value read from memory, is stored. This is shown in Figures 4.18 to 4.21 as the second step in the write-back stage. Write HCL code for `dstM`.

## Solution:
```
word dstM = [
    icode in { IMRMOVQ, IPOPQ } : rA;
    1                           : RNONE; # Don't write any register
];
```