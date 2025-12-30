#include "genmaze.h"

#define NewCell(x) (Cell) {.i = x,/* all walls enabled at init */.topWall = 1,.bottomWall = 1,.rightWall = 1,.leftWall = 1,/* cell not visited */.visited = 0}

Cell *InitializeCells(uint num) {
    // allocates cells
    Cell *cells = (Cell *)malloc(sizeof(Cell) * num);

    // if allocation fails, return NULL
    if (!cells) return NULL;

    // initialize all cells
    for (uint i = 0; i < num; i++)
        cells[i] = NewCell(i);

    // returns the cells
    return cells;
}

Maze *AllocateMaze(uint width, uint height) {
    // allocates the maze
    Maze *maze = (Maze *)malloc(sizeof(Maze));

    // if allocation fails, returns NULL
    if (!maze) return NULL;

    // initialize attributs of maze
    maze->width = width;
    maze->height = height;
    maze->cells = InitializeCells(width * height);

    // returns the maze
    return maze;
}

Maze *GenerateMaze(uint width, uint height) {
    // allocates the maze
    Maze *maze = AllocateMaze(width, height);

    // if allocation fails return NULL
    if (!maze) return NULL;

    // returns the maze
    return maze;
}

void FreeMaze(Maze *maze) {
    // if no maze present
    if (!maze) {
        printf("No maze to be freed.");
        return;
    }

    // free all cells
    free(maze->cells);

    // free the maze
    free(maze);
}