# Practice Problem 6.16 (solution page 665)
For the cache in Problem 6.12, list all of the hexadecimal memory addresses that will hit in set 3 

## Solution:

CI: `0x3`; CT: `0x32`

|12|11|10|9|8|7|6|5|4|3|2|1|0|
|-|-|-|-|-|-|-|-|-|-|-|-|-|
|CT|CT|CT|CT|CT|CT|CT|CT|CI|CI|CI|CO|CO|
|0|0|1|1|0|0|1|0|0|1|1|0|0|
|0|0|1|1|0|0|1|0|0|1|1|0|1|
|0|0|1|1|0|0|1|0|0|1|1|1|0|
|0|0|1|1|0|0|1|0|0|1|1|1|1|

addresses: `0x064C`, `0x064D`, `0x064E`, `0x064F`
