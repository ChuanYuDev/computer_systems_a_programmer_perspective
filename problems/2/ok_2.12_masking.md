# Practice Problem 2.12
solution page 146

Write C expressions, in terms of variable x, for the following values. Your code should work for any word size $w \geq 8$. For reference, we show the result of evalu­ating the expressions for `x = 0x87654321`, with $w=32$.

A. The least significant byte of x, with all other bits set to `0`. `[0x00000021]`

B. All but the least significant byte of `x` complemented,with the least significant byte left unchanged. `[0x789ABC21]`

C. The least significant byte set to all ones, and all other bytes of x left un­changed. [0x876543FF]

## Solution:
A. Expression: `x & 0xFF`

- `x & 0` can set bit to `0`. 

B. Expression: `x ^ (~0xFF)`
- `x ^ 1` can set bit complemented. 

C. Expression: `x | 0xFF`
- `x | 1` can set bit to `1`