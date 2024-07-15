#ifndef MY_MOUSE_H
#define MY_MOUSE_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct {
    int rows, cols;
    char full_char, path_char, entry_char, exit_char, empty_char;
    char maze[MAX_ROWS][MAX_COLS + 1];
} Maze;

typedef struct {
    int row, col, distance;
} Point;

void read_maze(FILE *file, Maze *maze);
void print_maze(const Maze *maze);
Point find_entry_exit(const Maze *maze, char entry_exit_char);
void print_point(const Point *point);
bool is_safe(const Maze *maze, int row, int col);
void explore_neighbors(Maze *maze, Point curr, Point *queue, bool visited[MAX_ROWS][MAX_COLS], Point parent[maze->rows][maze->cols], int *rear);
void backtrack_path(Maze maze, Point start, Point curr, Point *path, int *path_size, Point parent[maze.rows][maze.cols]);
int find_shortest_path(Maze *maze, Point start, Point end, Point *path, int *path_size);
int solve_maze(Maze *maze);
void mark_path(Maze *maze, Point *path, int path_size, Point entry, Point exit);
bool validate_dimensions(const Maze *maze);
void count_entrances_and_exits(const Maze *maze, int *entrance_count, int *exit_count);
bool check_invalid_characters(const Maze *maze);
bool validate_maze(const Maze *maze);
int parse_arguments(int argc, char *argv[]);
int process_file(const char *filename, Maze *maze);

#endif // MY_MOUSE_H
