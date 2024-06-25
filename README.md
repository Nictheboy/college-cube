# college-cube
A simple BFS search for cube

## Usage
Use ```git clone --recursive``` to clone this repo, and use CMake to compile the project.
```
Usage: bin/cube single-thread <max_depth>
       bin/cube multi-thread <max_depth>

Arguments:
  single-thread:  Use single thread to solve the cube.
  multi-thread:   Use multi thread to solve the cube.
  max_depth:      The maximum depth of the search tree.

Input:
  Initial state should be put into stdin.
  You need to use EOF to end your input.
  The format can be seen from the test folder.

Output:
  All solutions within max_depth steps.
```
## Algorithm & Performance

The program uses a BSF algorithm without any branch cutting
to find all solutions to solve a cube within max_depth steps.

In single-thread mode, the program can try all solutions within
5 steps in just about 600 ms, and
in multi-thread mode with >= 8 thread, and with >= 8GB memory,
the program can try all solutions within 6 steps in just about 12 s,
according to my laptop with Intel i5-8265U (8) @ 3.900GHz and
7654MiB of memory.

## Acknowledgement

A [concurrent queue library](https://github.com/cameron314/concurrentqueue) is used in this repo as a dependency.

## Author
Nictheboy Li <nictheboy@outlook.com>
from Renmin University of China, Jun. 25 2024
