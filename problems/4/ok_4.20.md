# Practice Problem 4.20 (solution page 488)
The register signal `srcB` indicates which register should be read to generate the signal `valB`. The desired value is shown as the second step in the decode stage in Figures 4.18 to 4.21. Write HCL code for `srcB`.

## Solution:

```
word srcB = [
    icode in { IOPQ, IRMMOVQ, IMRMOVQ }     : rB;
    icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
    1                                       : RNONE; # Don't need register
];
```