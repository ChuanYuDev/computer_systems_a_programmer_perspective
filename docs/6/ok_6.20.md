# Practice Problem 6.20 (solution page 666)
Given the assumptions of Practice Problem 6.18, determine the cache performance of the following code:

![](./images/6.20.png)

A. What is the total number of reads?

B. What is the total number of reads that miss in the cache?

C. What is the miss rate?

D. What would the miss rate be if the cache were twice as big?

## Solution:

- Mapping of `grid` to sets 

    |row col|0|1|2|3|...|14|15|
    |-|-|-|-|-|-|-|-|
    |0|0|0|1|1|...|7|7|
    |...||||||||
    |7|56|56|57|57|...|63|63|
    |8|0|0|1|1|...|7|7|
    |...||||||||
    |15|56|56|57|57|...|63|63|

- Only misses are the initial code misses

- A. Total reads: 512
- B. Total reads that miss: 128
- C. Miss rate: 128/512 = 25%
- D. Increasing the cache size by any amount would not change the miss rate, miss rate: 25%
