# Practice Problem 4.43 (solution page 494)
Suppose we use a branch prediction strategy that achieves a success rate of 65%, such as backward taken, forward not taken (BTFNT), as described in Section 4.5.4. What would be the impact on CPI, assuming all of the other frequencies are not affected?

## Solution:

|Cause|Name|Instruction frequency|Condition frequency|Bubbles|Product|
|-|-|-|-|-|-|
|Load/use|$lp$|0.25|0.20|1|0.05|
|Mispredict|$mp$|0.20|0.35|2|0.14|
|Return|$rp$|0.02|1.00|3|0.06|
|Total penalty|||||0.25|

- $CPI = 1.0 + lp + mp + rp = 1.25$ 