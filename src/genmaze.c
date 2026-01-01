#include "genmaze.h"

#define NewCell(idx, X, Y) (Cell) {.i = idx, .x = X, .y = Y,/* all walls enabled at init */.topWall = 1,.bottomWall = 1,.rightWall = 1,.leftWall = 1,/* cell not visited */.visited = 0}

Cell *InitializeCells(uint width, uint height) {
    // allocates cells
    Cell *cells = (Cell *)malloc(sizeof(Cell) * width * height);

    // if allocation fails, return NULL
    if (!cells) return NULL;

    // initialize all cells
    for (uint y = 0; y < height; y++)
        for (uint x = 0; x < width; x++) {
            uint i = x + y * width;
            cells[i] = NewCell(i, x, y);
        }

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
    maze->cells = InitializeCells(width, height);

    // initialize the maze for generation
    maze->generated   = 0;
    maze->vistedCells = NewStack(width * height);
    maze->currentCell = NULL;

    // returns the maze
    return maze;
}

void GetNeighbors(Maze *maze, Stack *neighbors) {
    for (int y = -1; y <= 1; y ++) {
        for (int x = -1; x <= 1; x ++) {
            // discard diagonal neighbors
            if (fabs((double)x) > 0.0 && fabs((double)y) > 0.0) continue;

            // this cell is not a neighbor of itself
            if (x == 0 && y == 0) continue;

            // discard neighbors outside of the screen
            if (maze->currentCell->x + x >= maze->width)  continue;
            if (maze->currentCell->y + y >= maze->height) continue;

            uint idx = (maze->currentCell->x + x) + (maze->currentCell->y + y) * maze->width;

            // discard already visited neighbors
            if (maze->cells[idx].visited) continue;

            // push the neighbor
            Push((*neighbors), idx);
        }
    }
}

void DestroyWallsBetween(Cell *current, Cell *next) {
    // set the corresponding wall to off,
    // by assuming than the cells are touching.
    // left wall
    if (current->x < next->x){
        next->leftWall     = 0;
        current->rightWall = 0;
    }

    // right wall
    else if (current->x > next->x){
        next->rightWall   = 0;
        current->leftWall = 0;
    }

    // top wall
    else if (current->y > next->y){
        next->topWall       = 0;
        current->bottomWall = 0;
    }

    // bottom wall
    else if (current->y < next->y){
        next->bottomWall = 0;
        current->topWall = 0;
    }
}

void Iteration(Maze *maze) {
    if (!maze->currentCell) return;
    if (maze->currentCell->visited) return;

    maze->currentCell->visited = 1;

    Stack neighbors = NewStack(4);
    Cell  *neighbor;

    while (1) {
        // get the neighbors
        GetNeighbors(maze, &neighbors);

        // if there are any neighbor
        if (neighbors.stackPtr > 0) {
            // choose a random neighbor
            uint idx = rand() % neighbors.stackPtr;
            neighbor = &maze->cells[neighbors.array[idx]];

            break;
        }
        // if there is no neighbor go into the previous visited cell and try again
        else if (maze->vistedCells.stackPtr > 0) {
            maze->currentCell = &maze->cells[Pop(maze->vistedCells)];
        }
        // finally if that is impossible the maze is generated
        else {
            // finish generating maze
            maze->generated = 1;
            return;
        }
    }
    // the neighbor stack
    FreeStack(neighbors);

    // making the path
    DestroyWallsBetween(maze->currentCell, neighbor);

    // push to the visited cells
    Push(maze->vistedCells, maze->currentCell->i);

    // go the neighbor
    maze->currentCell = neighbor;
}

Maze *GenerateMaze(uint width, uint height, uint seed, int generate) {
    // allocates the maze
    Maze *maze = AllocateMaze(width, height);

    // if allocation fails return NULL
    if (!maze) return NULL;

    maze->currentCell = &maze->cells[0];
    Push(maze->vistedCells, 0);

    srand(seed);

    if (generate) {
        while (!maze->generated)
            Iteration(maze);
    }

    // returns the maze
    return maze;
}

void FreeMaze(Maze *maze) {
    // if no maze present
    if (!maze) {
        printf("No maze to be freed.\n");
        return;
    }

    // free all cells
    if (maze->cells){
        free(maze->cells);
        maze->cells = NULL;
    }

    FreeStack(maze->vistedCells);

    // free the maze
    free(maze);
    maze = NULL;
}