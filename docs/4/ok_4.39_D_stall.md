# Practice Problem 4.39 (solution page 493)
Write HCL code for the signal `D_stall` in the PIPE implementation.

## Solution:
```
bool D_stall =
    # Load/use hazard
    E_icode in { IMRMOVQ, IPOPQ } &&
        E_dstM in { d_srcA, d_srcB };
```