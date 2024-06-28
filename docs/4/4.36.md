# Practice Problem 4.36 (solution page 492)
In this stage, we can complete the computation of the status code `Stat` by detecting the case of an invalid address for the data memory. Write HCL code for the signal `m_stat`.

## Solution:
```
word m_stat = [
    dmem_error  : SADR;
    1           : M_stat;
];
```