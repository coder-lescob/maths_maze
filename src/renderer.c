#include "renderer.h"

#define InitError (VideoStatus){.initialized = 0}
#define ErrorCheck(ptr, ret) if (!ptr) { fprintf(stderr, "Error occurred: %s\n", SDL_GetError()); return ret;}

int InitVideo(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Video Initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}

VideoStatus InitRendering(char *name, uint32_t width, uint32_t height) {
    VideoStatus status;

    if (width == 0 || height == 0) {
        SDL_Rect rect;
        SDL_GetDisplayBounds(0, &rect);
        width = rect.w, height = rect.h;
    }

    // creates window
    status.window   = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_FULLSCREEN_DESKTOP);
    ErrorCheck(status.window, InitError);

    // creates renderer
    status.renderer = SDL_CreateRenderer(status.window, -1, SDL_RENDERER_ACCELERATED);
    ErrorCheck(status.renderer, InitError);

    // create the screen texture
    status.screen   = SDL_CreateTexture(status.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    ErrorCheck(status.screen, InitError);

    // view port size
    status.vp_width = width, status.vp_height = height;

    // Video metadata
    status.running     = 1;
    status.frameCount  = 0;
    status.initialized = 1;

    return status;
}

void FrameUpdate(VideoStatus *status) {
    SDL_RenderClear(status->renderer);                            // fullscreen clear
    SDL_RenderCopy(status->renderer, status->screen, NULL, NULL); // fullscreen copy
    SDL_RenderPresent(status->renderer);                          // swap buffers

    SDL_Delay(16); // ~60 FPS
    status->frameCount++;
}

void PollEvents(VideoStatus *status, void (*HandleEvents)(VideoStatus *)) {
    while (SDL_PollEvent(&status->event)) {
        // if we shall quit set the running flag to false
        if (status->event.type == SDL_QUIT) status->running = 0;

        // call the user's handle event function
        if (HandleEvents) (*HandleEvents)(status);
    }
}

int StartVideo(char *name, const uint32_t width, const uint32_t height, void (*Init)(VideoStatus *),
void (*HandleEvents)(VideoStatus *), void (*Update)(VideoStatus *), void (*Render)(uint32_t *, int, VideoStatus *)) {
    // Init video
    if (InitVideo()) return 1;

    // create video status
    VideoStatus status = InitRendering(name, width, height);

    // init fails
    if (!status.initialized) {
        // cleanup allocated mem
        DestroyVideo(&status);
        return 1;
    }

    // run Init
    if (Init) 
        (*Init)(&status);

    // runtime error flag
    int runtimeError = 0;

    while (status.running) {
        // first poll events
        PollEvents(&status, HandleEvents);

        // call the Update function of user
        if (Update) 
            (*Update)(&status);

        // lock the texture data declaration
        void *pixels;
        int  bytesPerRow;

        // lock texture
        if (SDL_LockTexture(status.screen, NULL, &pixels, &bytesPerRow)) {
            fprintf(stderr, "Error while locking texture: %s\n", SDL_GetError());
            runtimeError = 1;
            break;
        }

        // call the Render function
        if (Render) 
            (*Render)(pixels, bytesPerRow, &status);

        // unlock texture
        SDL_UnlockTexture(status.screen);

        // swap buffers
        FrameUpdate(&status);
    }

    // destroy every thing
    DestroyVideo(&status);

    return runtimeError;
}

void DestroyVideo(VideoStatus *status) {
    // destroy
    if (status->screen)   SDL_DestroyTexture (status->screen);   // screen texture
    if (status->renderer) SDL_DestroyRenderer(status->renderer); // renderer
    if (status->window)   SDL_DestroyWindow  (status->window);   // window

    // quits SDL
    SDL_Quit();
}