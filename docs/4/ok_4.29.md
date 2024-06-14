# Practice Problem 4.29 (solution page 490)
Suppose we could take the system of Figure 4.32 and divide it into an arbitrary number of pipeline stages $k$, each having a delay of $300/k$, and with each pipeline register having a delay of 20 ps.

A. What would be the latency and the throughput of the system, as functions of $k$?

B. What would be the ultimate limit on the throughput?

## Solution:

A.

Clock cycle = $300 / k + 20$ ps

Latency = $(300 / k + 20) \times k = 300 + 20k$

Throughput = $\frac{1}{300 / k + 20 \ ps} = \frac{1000}{300/k + 20}$ GIPS

B.

$k \rightarrow \infty$, througput = 50 GIPS