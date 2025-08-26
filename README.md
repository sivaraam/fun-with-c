## c

A repository that contains:

- playful implementations that test different concepts in C
- simple implementations for some simple programs
- serious implementations for some big problems
  (Maze solving, Sudoku solving, etc.)

The repository is not so neatly organized, though. It should be, though.

The ['intern' folder](intern/) consists of some programs I created during an
internship I attended during the period of Feb-Mar 2018.

In case you're interested in serious implementations, see the ['intern'
folder](intern/). Specifically,

- [Maze solver](intern/files/maze/) - a program that finds the
  shortest path from source to destination in a given maze using the
  A-star algorithm.

- [Sudoku solver](intern/sudoku/sudoku_solver/) - a program that tries to solve
  sudoku puzzles using some well-known solving techniques

- [Chess move validator](intern/chess_move_validate/) - a simple library
  that could be used to validate basic chess moves

- [Chess GUI](intern/gui-gtk/chess/) - a simple GUI for the game of chess
  (using GTK+-3.0)

- [Socket Chat Application](sockets) - a simple client-server chat application
  implemented in C using socket programming.

### Dependencies
#### Git LFS
This repository tracks some very large test inputs using Git LFS. So,
Git LFS needs to be installed for the clone to succeed. You can
install Git LFS by following the instructions in [its website](https://git-lfs.github.com).

In case you have already cloned the repository without installing
Git LFS, run the following command **after installing** Git LFS
to fix the invalid state in which Git leaves the repo:

```
git checkout -f HEAD
```
