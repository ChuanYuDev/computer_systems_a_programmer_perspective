# Practice Problem 6.1 (solution page 660)
In the following, let $r$ be the number of rows in a DRAM array, $c$ the number of columns, $b_r$ the number of bits needed to address the rows, and $b_c$ the number of bits needed to address the columns. For each of the following DRAMs, determine the power-of-2 array dimensions that minimize max($b_r$, $b_c$), the maximum number of bits needed to address the rows or columns of the array.

![](./images/6.1.png)

## Solution:

<!-- |Organization|$r$|$c$|$b_r$|$b_c$|max($b_r$, $b_c$)|
|-|-|-|-|-|-|
|$16 \times 1$|4|4|2|2|2|
|$16 \times 4$|8|8|3|3|3|
|$128 \times 8$|32|32|5|5|5|
|$512 \times 4$|64|32|6|5|6|
|$1024 \times 4$|64|64|6|6|6| -->

- $16 \times 4$: 16 supercells, 4 cells per supercell
- Target: `min(max(r, c))`, the squarer the array is, the fewer the address bits are

|Organization|$r$|$c$|$b_r$|$b_c$|max($b_r$, $b_c$)|
|-|-|-|-|-|-|
|$16 \times 1$|4|4|2|2|2|
|$16 \times 4$|4|4|2|2|2|
|$128 \times 8$|16|8|4|3|4|
|$512 \times 4$|32|16|5|4|5|
|$1024 \times 4$|32|32|5|5|5|