# Practice Problem 4.40 (solution page 493)
Write HCL code for the signal `E_bubble` in the PIPE implementation.

## Solution:
```
bool E_bubble = 
    (E_icode in {IMRMOVQ, IPOPQ} && E_dstM in {d_srcA, d_srcB}) ||
    (E_icode == IJXX && !e_Cnd);
```