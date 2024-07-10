# Practice Problem 6.5 (solution page 662)
As we have seen, a potential drawback of SSDs is that the underlying flash memory can wear out. For example, for the SSD in Figure 6.14, Intel guarantees about 128 petabytes ($128 \times 10^{15}$ bytes) of writes before the drive wears out. Given this assumption, estimate the lifetime (in years) of this SSD for the following workloads:

A. Worst case for sequential writes: The SSD is written to continuously at a rate of 470 MB/s (the average sequential write throughput of the device).

B. Worst case for random writes: The SSD is written to continuously at a rate of 303 MB/s (the average random write throughput of the device).

C. Average case: The SSD is written to at a rate of 20 GB/day (the average daily write rate assumed by some computer manufacturers in their mobile computer workload simulations).

## Solution:

- A:
    - $128 \times 10^{15}B \div (470 MB/s) = 128 \times 10^{15} \div (470 \times 10^{6}) \div (365 \times 24 \times 60 \times 60)$ years $= 8.636$ years

- B:
    - $128 \times 10^{15}B \div (303 MB/s) = 128 \times 10^{15} \div (303 \times 10^{6}) \div (365 \times 24 \times 60 \times 60)$ years $= 13.396$ years

- C:
    - $128 \times 10^{15}B \div (20 GB/s) = 128 \times 10^{15} \div (20 \times 10^{9}) \div 365$ years $= 17534.247$ years
    - Answer 17535?