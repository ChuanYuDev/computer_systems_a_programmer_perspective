# Practice Problem 4.11
solution page 484 

The HCL code given for computing the minimum of three words contains four comparison expressions of the form `X <= Y`. Rewrite the code to compute the same result, but using only three comparisons.

## Solution

```
word Min3 = [
    A <= B && A <= C    : A;
    B <= C              : B;
    1                   : C;
]
```

If the first case is not satisfied, then `A > B` or `A > C`, we only need to compare `B` and `C`.