# Practice Problem 4.19 (solution page 487)
Write HCL code for the signal `need_valC` in the SEQ implementation.

## Solution:

```
bool need_valC = 
    icode in {
        IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL
    };
```