#pragma once

#include <stdint.h>
#include <SDL2/SDL.h>

/* An unsigned integer */
typedef unsigned int uint;

/* A stack */
typedef struct Stack {
    int stackPtr;
    int capacity;
    int *array;
} Stack;

#define NewStack(c) (Stack) {.stackPtr = 0, .capacity = c, .array = (int *)malloc(c * sizeof(int))}
#define Push(stack, x) if (stack.stackPtr < stack.capacity) stack.array[stack.stackPtr++] = x
#define Pop(stack) stack.array[(stack.stackPtr > 0)? --stack.stackPtr : 0]
#define FreeStack(stack) free(stack.array); stack.array = NULL