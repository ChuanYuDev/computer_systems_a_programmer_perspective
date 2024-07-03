# Practice Problem 4.44 (solution page 494)
Let us analyze the relative performance of using conditional data transfers versus conditional control transfers for the programs you wrote for Problems 4.5 and 4.6. Assume that we are using these programs to compute the sum of the absolute values of a very long array, and so the overall performance is determined largely by the number of cycles required by the inner loop. Assume that our jump instructions are predicted as being taken, and that around 50% of the array values are positive.

A. On average, how many instructions are executed in the inner loops of the two programs?

B. On average, how many bubbles would be injected into the inner loops of the two programs?

C. What is the average number of clock cycles required per array element for the two programs?

## Solution:
- `jne loop` should be counted into the inner loop
- A:
    - jxx:
        - Branch not taken: ~~8~~ 9
        - Branch taken: ~~7~~ 8
        - 50% predicted correctly
        - Average: ~~7.5~~ 8.5

    - cmovxx:
        - ~~7~~ 8 instructions

- B:
    - cmovxx: 0
    - jxx: 50% * 2 = 1 bubbles
    - THe loop-closing jump will be predicted correctly, except when the loop terminates
        - For a very long array, this one misprediction will have a negligible effect on the performance

- C:
    - cmovxx: ~~7~~ 8 clock cycles
    - jxx: ~~7.5 + 1 = 8.5~~ 8.5 + 1 = 9.5 clock cycles