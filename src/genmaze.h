#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "types.h"

/*
* Structure representing a little portion of the maze
* Each cell has up to 4 walls
*/
typedef struct Cell {
    // the index of the cell in the maze grid
    uint i, x, y;

    // walls: 1 = present, 0 = absent
    uint topWall    : 1;
    uint bottomWall : 1;
    uint rightWall  : 1;
    uint leftWall   : 1;

    // has the cell been visited (for maze generation)
    uint visited    : 1;
} Cell;

/*
* Structure representing the maze as a list of cells,
* along with its dimensions.
*/
typedef struct Maze {
    uint width;   // number of cells in width
    uint height;  // number of cells in height
    Cell *cells;  // pointer to the array of cells

    // maze generation
    int   generated;
    Cell *currentCell;
    Stack vistedCells;
} Maze;

/*
* Initialize cells with all walls on, not visited yet with the right index.
* NOTE : cells are allocated on the heap please free them after usage.
* @param width  How wide is the maze in cells ?
* @param height How high is the maze in cells ?
* @returns The pointer to cells.
*/
Cell *InitializeCells(uint width, uint height);

/*
* Allocates a new maze as long as its cells.
* NOTE : The maze as its cells are allocated on the heap please free them after usage.
* @param width The width of the maze.
* @param height The height of the maze.
* @returns The pointer to the maze.
*/
Maze *AllocateMaze(uint width, uint height);

/*
* Push into the neighbors stack the neighbors of the current cell.
* NOTE : It assumes that the maze and its current cell are not NULL. 
* If thats the case the program will do a segmenation fault, because it tries to dereference a NULL pointer.
* @param maze The pointer to the maze of choice containing the current cell.
* @param neighbors The pointer to the stack of the neighbors (should have at least a capacity of 4).
*/
void GetNeighbors(Maze *maze, Stack *neighbors);

/*
* Destroy the walls between two cells based on there relative positionning.
* It destroys the walls of itersection between cell A and cell B.
* NOTE :  it assumes that the cells are touching.
* @param current A pointer to the current cell aka cell A.
* @param next    A pointer to the next cell aka cell B.
*/
void DestroyWallsBetween(Cell *current, Cell *next);

/*
* Do a signgle iteration of the maze generation.
* @param maze A pointer to the maze.
*/
void Iteration(Maze *maze);

/*
* Generates a random maze of given width and height.
* NOTE : The maze is allocated on the heap and must be freed using FreeMaze().
* @param width  The width of the wanted maze
* @param height The height of the wanted maze.
* @param seed   The seed of the random number generator.
* @param generate Wether it should generate the maze immediatly or that the user wants to do an animation using Iteration().
*/
Maze *GenerateMaze(uint width, uint height, uint seed, int generate);

/*
* Frees the memory allocated for the maze.
* @param maze The maze to free.
*/
void FreeMaze(Maze *maze);