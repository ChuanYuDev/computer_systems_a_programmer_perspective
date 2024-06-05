# Practice Problem 4.12
solution page 484

Write HCL code describing a circuit that for word inputs `A`, `B`, and `C` selects the median of the three values. That is, the output equals the word lying between the minimum and maximum of the three inputs.

## Solution:
```
word Median = [
    C <= A && A <= B    : A;
    B <= A && A <= C    : A;
    C <= B && B <= A    : B;
    A <= B && B <= C    : B;
    1                   : C;
]
```