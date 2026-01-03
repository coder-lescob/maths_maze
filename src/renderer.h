#pragma once

#include <SDL2/SDL.h>
#include <stdio.h>
#include "types.h"
#include "genmaze.h"

typedef struct VideoStatus {
    // init stuff
    int initialized;

    // window rendering ptrs
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture  *screen;

    // window data
    uint32_t vp_width, vp_height;

    // runtime data
    int          running;
    SDL_Event    event;
    uint32_t     frameCount;
    const Uint8  *keyboardState;
} VideoStatus;

/*
* Polls the keyboard and stores it in the keyboard status field
*/
void PollKeyboard();

/*
* Init video chanel.
* @returns 0 if init work and 1 if it fails
*/
int InitVideo(void);

/*
* Init the rendering by creating a video status with the corresponding width and height.
* @param name The name of the window.
* @param width The width of the window view port.
* @param height The height of the view port.
* @returns The videoStatus of the rendering.
*/
VideoStatus InitRendering(char *name, uint32_t width, uint32_t height);

/*
* Start video rendering with event handling and Update.
* NOTE : Every thing is initialized here, so it calls InitVideo() and InitRendering().
* NOTE : This function also cleans everything behind it so it calls DestroyVideo().
* @param name The name of the window.
* @param width The width of the window view port.
* @param height The height of the view port.
* @param Inir A pointer to a function run once at the begining.
* @param HandleEvents A pointer to a function that handles events.
* @param Update A pointer to a function that will be run every frame, not meant for rendering.
* @param Render A pointer to a function that will be run every frame with the screen pointer lock to render stuff on the screen.
* @returns 0 if it exited normally, 1 if there were an error.
*/
int StartVideo(char *name, const uint32_t width, const uint32_t height, void (*Init)(VideoStatus *),
    void (*HandleEvents)(VideoStatus *), void (*Update)(VideoStatus *), void (*Render)(uint32_t *, int, VideoStatus *));

/*
* Destroy the video.
* @param status The video status to destroy.
*/
void DestroyVideo(VideoStatus *status);

/*
* Compute the pointer to the corresponding pixel.
* TODO: Make it safe to disallow pixels outside of the screen.
* @param pix         The pointer to the first pixel aka to the array of pixels.
* @param bytesPerRow The number of byte in a row of pixel refered also as pitch.
* @param x           The x coordonate of the pixel.
* @param y           The y coordonate of the pixel.
* @returns A pointer to the corresponding pixel.
*/
uint32_t *GetPix(uint32_t *pix, int bytesPerRow, uint32_t x, uint32_t y);

/*
* Draws a pixel at (x,y) of color col.
* @param pixels      The pointer to the first pixel aka to the array of pixels.
* @param bytesPerRow The number of byte in a row of pixel refered also as pitch.
* @param x           The x coordonate of the pixel.
* @param y           The y coordonate of the pixel.
*/
void DrawPix(uint32_t *pixels, int bytesPerRow, uint x, uint y, uint32_t col);

/*
* Draws an vertical line from (x, y1) to (x, y2) on the pixels.
* NOTE: y1 must be less than y2.
* @param pixels      The pointer to the first pixel aka to the array of pixels.
* @param bytesPerRow The number of byte in a row of pixel refered also as pitch.
* @param x           The x coordonate of the line.
* @param y1          The initial coordonate of the line.
* @param y2          The final coordonate of the line.
*/
void DrawVert(uint32_t *pixels, int bytesPerRow, uint x, uint y1, uint y2, uint32_t color);

/*
* Draws an horizonatal line from (x1, y) to (x2, y) on the pixels.
* NOTE: x1 must be less than x2.
* @param pixels      The pointer to the first pixel aka to the array of pixels.
* @param bytesPerRow The number of byte in a row of pixel refered also as pitch.
* @param y           The y coordonate of the line.
* @param x1          The initial coordonate of the line.
* @param x2          The final coordonate of the line.
*/
void DrawHorz(uint32_t *pixels, int bytesPerRow, uint y, uint x1, uint x2, uint32_t color);

/*
* Render the cell at (x, y) from a maze in the pixels buffer.
* @param maze        The maze where is the cell.
* @param cellSize    The size of a cell on the screen.
* @param x           The x coords of the cell.
* @param y           The y coords of the cell.
* @param pixels      The pointer to the first pixel aka to the array of pixels.
* @param bytesPerRow The number of byte in a row of pixel refered also as pitch.
*/
void RenderCell(Maze *maze, uint cellSize, uint x, uint y, uint32_t *pixels, int bytesPerRow, int highlight);