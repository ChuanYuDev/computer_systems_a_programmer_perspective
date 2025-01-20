# Practice Problem 6.10 (solution page 663)
In the previous dotprod example, what fraction of the total references to `x` and `y` will be hits once we have padded array `x`?

## Solution:
- Total references: 16
- Misses: `x[0]`, `y[0]`, `x[4]`, `y[4]`
- Hits: `x[1]`-`x[3]`, `y[1]`-`y[3]`, `x[5]`-`x[7]`, `y[5]`-`y[7]`
- The fraction of hits is 12/16 = 3/4