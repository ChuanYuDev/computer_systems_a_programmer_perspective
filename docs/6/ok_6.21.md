# Practice Problem 6.21 (solution page 666)
Use the memory mountain in Figure 6.41 to estimate the time, in CPU cycles, to read an 8-byte word from the L1 d-cache.

## Solution:

<!-- - `size`: 16 KB, `stride`: 1, `Mhz`: $2.1 \times 10^3$ Mhz
- `size` / `stride` / (`cycles` / `Mhz`) = 14 GB/s
- `cycles`= $\frac{16 KB \times 2.1 \times 10^3 Mhz}{14 GB/s}$ = $\frac{16 \times 10^3 B \times 2.1 \times 10^9 cycles / s}{14 \times 10^9 B/s}$ = $\frac{16 \times 10^3 \times 2.1 cycles}{14} = 2400 cycles$
- $16KB = 2000 \times 8B$
- Reading an 8-byte word from the L1 d-cache uses 1.2 cycles -->

- L1 read throughput 12 GB/s
- The clock frequency is 2.1 GHZ
- Read 8 bytes uses $8B \times \frac{1s}{12GB} = 8B \times \frac{2.1G \ cycles}{12GB} = 1.4$ cycles
- This is roughly 2.5 times faster than the nominal 4-cycle latency from L1
- This is due to the parallelism of the $4 \times 4$ unrolled loop, which allows multiple loads to be in flight at the same time

