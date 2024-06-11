# Practice Problem 4.1S (solution page 486)

What would be the effect of the instruction `pushq %rsp` according to the steps listed in Figure 4.20? Does this conform to the desired behavior for Y86-64, as determined in Problem 4.7?

## Solution:
`pushq %rsp` pushes the original value of register `%rsp` to the stack, because `valA` is `R[rA] = R[%rsp]`, `valE` is the decremented value of `%rsp`, according to Figure 4.20, `valA` will be assigned to $M8$[valE] 

This conforms the desired behavior for Y86-64