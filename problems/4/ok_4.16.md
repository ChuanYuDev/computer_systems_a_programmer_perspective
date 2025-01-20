# Practice Problem 4.16 (solution page 486)

Assume the two register writes in the write-back stage for `popq` occur in the order listed in Figure 4.20. What would be the effect of executing `popq %rsp`? Does this conform to the desired behavior for Y86-64, as determined in Problem 4.8?

## Solution:

First, `R[%rsp]` is assigned to `valE` which is the decremented value of `%rsp`, then `R[rA]` is assigned to `valM`, hence, register `%rsp` will be assigned to value in memory

This conforms to the desired behavior for Y86-64