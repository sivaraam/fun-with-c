## Maze Solver

A programs that tries to find the **shortest path** from the source
to the destination in the maze given in the form of an BMP image.
The program uses the **A-star algorithm** with Manhattan distance
heuristic with ties-broken using the heuristic value.


### Analysis

A detailed analysis of the run-times of the program on a particular
environment can be found in the analysis/ folder in this repo.


### Build

```
make
```

### Try it out

#### To try out all (simple) test cases

```
./run_all
```

#### To try out a single test

```
./replace_tests
./maze_solver.out test_inputs/BMP7.bmp
```

#### Note about test inputs
The size of the maze increases with increasing test-file-suffix number.
Specific details about the test cases could be found in the analysis
document found in the [analysis folder](analysis/).

Some very large test inputs are available in the repository. They are tracked
using [Git LFS](https://git-lfs.github.com). One very very large test input is
not put into this repository due to its size.

#### Solved test cases
Some solved test cases could are be found in the following folders:

- [Test cases solved using the A-star algorithm](test_inputs.solved.a_star__manhattan_heuristic)

- [Test cases solved using Breadth-First Search algorithm](test_inputs.solved.bfs)
