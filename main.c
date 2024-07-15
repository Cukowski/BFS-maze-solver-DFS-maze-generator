#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR "cls"
    void usleep(__int64 usec) {
        HANDLE timer;
        LARGE_INTEGER ft;
        ft.QuadPart = -(10 * usec);
        timer = CreateWaitableTimer(NULL, TRUE, NULL);
        SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
        WaitForSingleObject(timer, INFINITE);
        CloseHandle(timer);
    }
#else
    #include <unistd.h>
    #define CLEAR "clear"
#endif

#include "maze_solver.h"

void read_maze(FILE *file, Maze *maze) 
{
    fscanf(file, "%dx%d %c %c %c %c\n", &maze->rows, &maze->cols, &maze->full_char, &maze->path_char, &maze->entry_char, &maze->exit_char);
    maze->empty_char = ' ';

    for (int i = 0; i < maze->rows; ++i) {
        if (fgets(maze->maze[i], MAX_COLS + 1, file) == NULL) {
            fprintf(stderr, "Error reading maze.\n");
            exit(EXIT_FAILURE);
        }

        size_t len = strlen(maze->maze[i]);
        if (len > 0 && maze->maze[i][len - 1] == '\n') {
            maze->maze[i][len - 1] = '\0';
        }
    }
}

void print_maze(const Maze *maze) 
{
    for (int i = 0; i < maze->rows; ++i) 
    {
        printf("%s\n", maze->maze[i]);
    }
}

Point find_entry_exit(const Maze *maze, char entry_exit_char) 
{
    Point entry_exit = { -1, -1 , 0};

    for (int i = 0; i < maze->rows; ++i) 
    {
        for (int j = 0; j < maze->cols; ++j) 
        {
            if (maze->maze[i][j] == entry_exit_char) 
            {
                entry_exit.row = i;
                entry_exit.col = j;
                return entry_exit;
            }
        }
    }

    return entry_exit;
}

void print_point(const Point *point) 
{
    printf("Row: %d, Col: %d, Steps: %d\n", point->row, point->col, point->distance);
}

bool is_safe(const Maze *maze, int row, int col) 
{
    return row >= 0 && row < maze->rows && col >= 0 && col < maze->cols && maze->maze[row][col] != maze->full_char;
}

void explore_neighbors(Maze *maze, Point curr, Point *queue, bool visited[MAX_ROWS][MAX_COLS], Point parent[maze->rows][maze->cols], int *rear) 
{
    int dx[] = {-1, 0, 0, 1};
    int dy[] = {0, -1, 1, 0};

    for (int i = 0; i < 4; ++i) 
    {
        int new_row = curr.row + dx[i];
        int new_col = curr.col + dy[i];
        if (is_safe(maze, new_row, new_col) && !visited[new_row][new_col]) 
        {
            queue[(*rear)++] = (Point){new_row, new_col, curr.distance + 1};
            visited[new_row][new_col] = true;
            parent[new_row][new_col] = curr;
            
            maze->maze[new_row][new_col] = maze->path_char;
            print_maze(maze);
            usleep(200000);
            system(CLEAR);
        }
    }
}

void backtrack_path(Maze maze, Point start, Point curr, Point *path, int *path_size, Point parent[maze.rows][maze.cols]) 
{
    int idx = 0;
    while (!(curr.row == start.row && curr.col == start.col)) 
    {
        path[idx++] = curr;
        curr = parent[curr.row][curr.col];
    }
    path[idx++] = start;
    *path_size = idx;
}

int find_shortest_path(Maze *maze, Point start, Point end, Point *path, int *path_size) 
{
    Point queue[maze->rows * maze->cols];
    bool visited[MAX_ROWS][MAX_COLS] = {false};
    Point parent[maze->rows][maze->cols];
    
    int front = 0, rear = 0;
    queue[rear++] = start;
    visited[start.row][start.col] = true;
    
    while (front < rear) {
        Point curr = queue[front++];
        if (curr.row == end.row && curr.col == end.col) 
        {
            backtrack_path(*maze, start, curr, path, path_size, parent);
            return parent[end.row][end.col].distance;
        }
        explore_neighbors(maze, curr, queue, visited, parent, &rear);
    }
    return -1;
}

int solve_maze(Maze *maze) 
{
    Point entry = find_entry_exit(maze, maze->entry_char);
    Point exit = find_entry_exit(maze, maze->exit_char);

    Point path[maze->rows * maze->cols];
    int path_size = 0;
    int steps = find_shortest_path(maze, entry, exit, path, &path_size);
    if (steps >= 0) {
        // Mark the final path in the maze
        mark_path(maze, path, path_size, entry, exit);

        print_maze(maze);
        printf("%d STEPS!\n", steps);
        return EXIT_SUCCESS;
    } else {
        printf("No path found!\n");
        return EXIT_FAILURE;
    }
}

void mark_path(Maze *maze, Point *path, int path_size, Point entry, Point exit) 
{
    for (int i = 0; i < maze->rows; ++i) 
    {
        for (int j = 0; j < maze->cols; ++j) 
        {
            if (maze->maze[i][j] == maze->path_char) 
            {
                maze->maze[i][j] = maze->empty_char;
            }
        }
    }
    for (int i = 0; i < path_size; ++i) 
    {
        Point p = path[i];
        maze->maze[p.row][p.col] = 'o';
    }
    maze->maze[entry.row][entry.col] = maze->entry_char;
    maze->maze[exit.row][exit.col] = maze->exit_char;
}

bool validate_dimensions(const Maze *maze) 
{
    if (maze->rows <= 0 || maze->rows > MAX_ROWS || maze->cols <= 0 || maze->cols > MAX_COLS) 
    {
        return false;
    }
    return true;
}

void count_entrances_and_exits(const Maze *maze, int *entrance_count, int *exit_count) 
{
    *entrance_count = 0;
    *exit_count = 0;

    for (int i = 0; i < maze->rows; ++i) 
    {
        for (int j = 0; j < maze->cols; ++j) 
        {
            char cell = maze->maze[i][j];
            if (cell == maze->entry_char) 
            {
                (*entrance_count)++;
            } 
            else if (cell == maze->exit_char) 
            {
                (*exit_count)++;
            }
        }
    }
}

bool check_invalid_characters(const Maze *maze) 
{
    for (int i = 0; i < maze->rows; ++i) 
    {
        for (int j = 0; j < maze->cols; ++j) 
        {
            char cell = maze->maze[i][j];
            if (cell != maze->full_char && cell != ' ' && cell != maze->path_char &&
                cell != maze->entry_char && cell != maze->exit_char) 
                {
                return false;
            }
        }
    }
    return true;
}

bool validate_maze(const Maze *maze) 
{
    if (!validate_dimensions(maze)) { 
        return false;
    }

    int entrance_count, exit_count;
    count_entrances_and_exits(maze, &entrance_count, &exit_count);

    if (!check_invalid_characters(maze)) { 
        return false;
    }

    if (entrance_count != 1 || exit_count != 1) {
        return false;
    }

    return true;
}

int parse_arguments(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <maze_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int process_file(const char *filename, Maze *maze) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    read_maze(file, maze);
    fclose(file);

    if (!validate_maze(maze)) 
    {
        fprintf(stderr, "Invalid maze configuration.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) 
{
    if (parse_arguments(argc, argv) != EXIT_SUCCESS) 
    {
        return EXIT_FAILURE;
    }

    Maze maze;
    if (process_file(argv[1], &maze) != EXIT_SUCCESS) 
    {
        return EXIT_FAILURE;
    }

    if (solve_maze(&maze) != EXIT_SUCCESS) 
    {
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
