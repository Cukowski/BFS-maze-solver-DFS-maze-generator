# BFS maze solver DFS maze generator
Breadth-First Search maze solver and Recursive Backtracking maze generator in C language

## Overview

This project contains two main components: a maze solver and a maze generator. The maze solver finds the shortest path from the entry to the exit using the Breadth-First Search (BFS) algorithm, while the maze generator creates a random maze using a recursive backtracking algorithm.

## Maze Solver

### Description

The maze solver reads a maze from a file and finds the shortest path from the entry point ('1') to the exit point ('2') using the Breadth-First Search (BFS) algorithm.

### Algorithm

**Breadth-First Search (BFS):**

- **BFS** is an algorithm for traversing or searching tree or graph data structures. It starts at the root (or another arbitrarily selected node) and explores all of the neighbor nodes at the present depth prior to moving on to the nodes at the next depth level.

### Benefits

- **Completeness**: BFS will find a solution if one exists.
- **Optimality**: BFS finds the shortest path in an unweighted grid.
- **Simplicity**: BFS is relatively simple to implement.

### Disadvantages

- **Memory Usage**: BFS can consume a lot of memory because it stores all generated nodes in memory.

### Implementation

The solver is implemented in `my_mouse.c` and `my_mouse.h`. It reads the maze from a file, finds the entry and exit points, and uses BFS to find the shortest path. The path is then marked and the maze is printed.

## Maze Generator

### Description

The maze generator creates a random maze using a recursive backtracking algorithm. The maze is then saved to a file.

### Algorithm

**Recursive Backtracking Algorithm:**

- This algorithm works by carving passages in the maze. It starts from a random point and carves out a path to a new random point that hasn't been visited yet. This process is repeated recursively, backtracking when necessary until the entire grid is visited.

### Benefits

- **Simple and Intuitive**: The recursive backtracking algorithm is straightforward to implement and understand.
- **Good Quality Mazes**: The generated mazes are complex and interesting.

### Disadvantages

- **Performance**: For very large mazes, the recursive backtracking algorithm can be slower compared to other algorithms.
- **Memory Usage**: Recursive algorithms can lead to high memory usage due to stack space in deep recursion.

### Implementation

The generator is implemented in `maze_generator.c`. It initializes a grid with walls, then carves out passages recursively. The maze is saved to `generated_maze.txt`.

## How to Use

### Prerequisites

- GCC (GNU Compiler Collection)
- Make (optional, for using the provided Makefile)

### Compiling

To compile both the solver and the generator, you can use the provided Makefile:

```
make
Alternatively, you can compile them manually:


gcc -o maze_solver main.c -Wall
gcc -o maze_generator maze_generator.c -Wall

Running the Maze Generator
To generate a maze:

./maze_generator [rows] [cols]
The generated maze will be saved to generated_maze.txt.

Running the Maze Solver
To solve a maze:

./maze_solver generated_maze.txt
Replace generated_maze.txt with the path to any maze file you want to solve.
```

Example Maze
Here is an example of a generated maze:


21x21* o12
*********************
*1*     * *       * *
* * *** * * *** * * *
*   *   *   * * *   *
***** *** *** * *** *
*   *   *     *   * *
* ***** ***** *** ***
*       *   * * *   *
* ********* * * *** *
* *     *   * *   * *
* * * * * * * * * * *
* * * * * *   * * * *
* * * * * ******* * *
* * * * *         * *
* *** * *********** *
* *   * *       *   *
* * *** * ***** * ***
* * * *   *   * *   *
* * * ***** * * *** *
*   *       *      2*
*********************
After solving, the path is marked, and the solver outputs the number of steps taken to solve the maze.

Conclusion
This project provides a simple yet effective implementation of a maze solver and generator using BFS and recursive backtracking. While both algorithms have their trade-offs, they offer a good balance of simplicity and effectiveness for the purposes of maze generation and solving.