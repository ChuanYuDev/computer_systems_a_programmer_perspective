# Practice Problem 4.24 (solution page 488)
The conditional move instructions, abbreviated `cmovXX`, have instruction code IRRMOVQ. As Figure 4.28 shows, we can implement these instructions by making use of the `Cnd` signal, generated in the execute stage. Modify the HCL code for `dstE` to implement these instructions.

## Solution:
```
word dstE = [
    # icode in { IRRMOVQ } && (icode == 32 || Cnd)  : rB;
    icode in { IRRMOVQ } && Cnd                     : rB;
    icode in { IIRMOVQ, IOPQ}                       : rB;
    icode in { IPUSHQ, IPOPQ, ICALL, IRET }         : RRSP;
    1                                               : RNONE; # Don't write any register
];
```