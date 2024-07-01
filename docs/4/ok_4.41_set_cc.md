# Practice Problem 4.41 (solution page 493)
Write HCL code for the signal `set_cc` in the PIPE implementation. This should only occur for `OPq` intructions, and should consider the effects of program exception.

## Solution:
```
bool set_cc = 
    (E_icode == IOPQ) &&
    !(m_stat in {SADR, SINS, SHLT} || W_stat in {SADR, SINS, SHLT});
```