#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // for getcwd()

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct {
    int row, col;
} Point;

typedef struct {
    int rows, cols;
    char maze[MAX_ROWS][MAX_COLS + 1];  // +1 to accommodate the null terminator
} Maze;

void initialize_maze(Maze *maze) {
    for (int i = 0; i < maze->rows; ++i) {
        for (int j = 0; j < maze->cols; ++j) {
            maze->maze[i][j] = '*';
        }
        maze->maze[i][maze->cols] = '\0';  // Null-terminate each row
    }
    printf("Maze initialized\n");
}

void carve_passages_from(int r, int c, Maze *maze) {
    int directions[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

    // Shuffle directions
    for (int i = 0; i < 4; ++i) {
        int j = rand() % 4;
        int temp[2] = { directions[i][0], directions[i][1] };
        directions[i][0] = directions[j][0];
        directions[i][1] = directions[j][1];
        directions[j][0] = temp[0];
        directions[j][1] = temp[1];
    }

    for (int i = 0; i < 4; ++i) {
        int nr = r + directions[i][0] * 2;
        int nc = c + directions[i][1] * 2;

        if (nr >= 0 && nr < maze->rows && nc >= 0 && nc < maze->cols && maze->maze[nr][nc] == '*') {
            maze->maze[nr][nc] = ' ';
            maze->maze[r + directions[i][0]][c + directions[i][1]] = ' ';
            carve_passages_from(nr, nc, maze);
        }
    }
}

void place_entry_exit(Maze *maze) {
    maze->maze[1][1] = '1';
    maze->maze[maze->rows - 2][maze->cols - 2] = '2';
    printf("Entry and exit placed\n");
}

void print_maze_to_file(Maze *maze, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("Writing maze to file\n");
    fprintf(file, "%dx%d* o12\n", maze->rows, maze->cols);
    for (int i = 0; i < maze->rows; ++i) {
        fprintf(file, "%s\n", maze->maze[i]);
    }

    fclose(file);
    printf("Maze written to %s\n", filename);
}

int main() {
    srand(time(NULL));

    Maze maze;
    maze.rows = 55; // Adjust this to generate larger or smaller mazes
    maze.cols = 55; // Adjust this to generate larger or smaller mazes

    printf("Generating maze of size %dx%d\n", maze.rows, maze.cols);
    initialize_maze(&maze);
    maze.maze[1][1] = ' ';
    carve_passages_from(1, 1, &maze);
    place_entry_exit(&maze);

    // Get the current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }

    // Specify the full path for the output file
    char filename[1060];
    snprintf(filename, sizeof(filename), "%s/generated_maze.txt", cwd);
    print_maze_to_file(&maze, filename);

    printf("Maze generation completed\n");

    return 0;
}
