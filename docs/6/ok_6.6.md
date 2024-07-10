# Practice Problem 6.6 (solution page 662)
Using the data from the years 2005 to 2015 in Figure 6.15(c), estimate the year when you will be able to buy a petabyte ($10^{15}$ bytes) of rotating disk storage for $500. Assume actual dollars (no inflation).

## Solution:

- Dollars per GB = $500 \div (10^{15} \div 10^{9}) = 5 \times 10^{-4}$
- From the years 2005 to 2015, the cost decreases $5 \div 0.03 = 166.667$ times

- Compared with 2005
    - The cost decreased factor should be $5 \div (5 \times 10^{-4}) = 10^4$
    - The number of years $n$ is $166.667^{n \div 10} = 10^4$, $n = 18$
    - The year is about 2023

- Compared with 2015
    - The cost decreased factor should be $0.03 \div (5 \times 10^{-4}) = 60$
    - The number of years $n$ is $166.667^{n \div 10} = 60$, $n = 8$
    - The year is about 2023

- Answer 2025?