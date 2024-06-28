# Practice Problem 4.31 (solution page 490)
The block labeled `dstE` in the decode stage generates the register ID for the E port of the register file, based on fields from the fetched instruction in pipeline register D. The resulting signal is named `d_dstE` in the HCL description of PIPE. Write HCL code for this signal, based on the HCL description of the SEQ signal `dstE`. (See the decode stage for SEQ in Section SEQ stage implementations) Do not concern yourself with the logic to implement conditional moves yet.

## Solution:
```
word d_dstE = [
    # D_icode in { IRRMOVQ, IIRMOVQ, IOPQ}      : rB;
    D_icode in { IRRMOVQ, IIRMOVQ, IOPQ}        : D_rB;
    D_icode in { IPUSHQ, IPOPQ, ICALL, IRET }   : RRSP;
    1                                           : RNONE; # Don't write any register
];
```