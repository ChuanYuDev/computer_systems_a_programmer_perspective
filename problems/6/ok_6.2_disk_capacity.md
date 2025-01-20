# Practice Problem 6.2 (solution page 661)
What is the capacity of a disk with 2 platters, 10,000 cylinders, an average of 400 sectors per track, and 512 bytes per sector?

## Solution:

- Cylinders number is equal to the number of tracks on one surface

- Capacity:

    $Capacity =
        \frac{\# \ bytes}{sector} \times 
        \frac{average \ \# \ sectors}{track} \times 
        \frac{\# \ tracks}{surface} \times
        \frac{\# \ surfaces}{platter} \times
        \frac{\# \ platters}{disk}
    \\= \frac{512 \ bytes}{sector} \times 
        \frac{400 \ sectors}{track} \times 
        \frac{10,000 \ tracks}{surface} \times
        \frac{2 \ surfaces}{platter} \times
        \frac{2 \ platters}{disk}
    \\= 8,192,000,000 \ bytes
    \\= 8.192 \ GB$