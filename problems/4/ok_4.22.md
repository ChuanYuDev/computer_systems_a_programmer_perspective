# Practice Problem 4.22 (solution page 488)
Only the `popq` instruction uses both register file write ports simultaneously. For the instruction `popq %rsp`, the same address will be used for both the E and M write ports. but with different data. To handle this conflict, we must establish a priority among the two write ports so that when both attempt to write the same register on the same cycle, only the write from the higher-priority port takes place. Which of the two ports should be given priority in order to implement the desired behavior, as determined in Practice Problem 4.8?

## Solution:
`popq %rsp` will pop the value at memory location `%rsp` to register `%rsp`

Hence only R[rA] $\leftarrow$ valM occurs, M write port has higher priority, write via the M port to take priority over the write via the E port