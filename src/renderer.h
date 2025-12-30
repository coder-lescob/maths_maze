#pragma once

#include <SDL2/SDL.h>

typedef struct VideoStatus {
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
} VideoStatus;

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