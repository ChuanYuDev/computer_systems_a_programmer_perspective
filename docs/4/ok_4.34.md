# Practice Problem 4.34 (solution page 491)
Write HCL code for the signal `d_valB`, giving the value for source operand `valB` supplied to pipeline register E.

## Solution:

```
word d_valB = [
    # d_srcA == e_dstE  : e_valE; # Forward valE from execute
    # d_srcA == M_dstM  : m_valM; # Forward valM from memory
    # d_srcA == M_dstE  : M_valE; # Forward valE from memory
    # d_srcA == W_dstM  : W_valM; # Forward valM from write back
    # d_srcA == W_dstE  : W_valE; # Forward valE from write back
    
    d_srcB == e_dstE    : e_valE; # Forward valE from execute
    d_srcB == M_dstM    : m_valM; # Forward valM from memory
    d_srcB == M_dstE    : M_valE; # Forward valE from memory
    d_srcB == W_dstM    : W_valM; # Forward valM from write back
    d_srcB == W_dstE    : W_valE; # Forward valE from write back
    1                   : d_rvalB; # Use value read from register file
];
```