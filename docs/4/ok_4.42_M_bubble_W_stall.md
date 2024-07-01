# Practice Problem 4.42 (solution page 493)
Write HCL code for the signals `M_bubble` and `W_stall` in the PIPE implementation. The latter signal requires modifying the exception condition listed in Figure 4.64.

## Solution:
```
bool M_bubble = 
    m_stat in {SADR, SINS, SHLT} || W_stat in {SADR, SINS, SHLT};

bool W_stall = 
    W_stat in {SADR, SINS, SHLT};
```