# Practice Problem 2.21 (solution page 149)
Assuming the expressions are evaluated when executing a 32-bit program on a machine that uses two's-complement arithmetic, fill in the following table describing the effect of casting and relational operations, in the style of Figure 2.19:

![](images/2.21.jpg)

## Solution:
|Expression|Type|Evaluation|
|-|-|-|
|`-2147483647-1 == 2147483648U`|Unsigned|1|
|`-2147483647-1 < 2147483647`|Signed|1|
|`-2147483647-1U < 2147483647`|Unsigned|0|
|`-2147483647-1 < -2147483647`|Signed|1|
|`-2147483647-1U < -2147483647`|Unsigned|1|

- `-2147483647`: $-2^{31} + 1$
- `-2147483647-1U`: convert `-2147483647` to unsigned which is $2^{31} + 1$, minus `1`, yield $2^{31}$
