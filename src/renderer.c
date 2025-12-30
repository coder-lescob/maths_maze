#include "renderer.h"

int InitVideo(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Video Initialization fails: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}

VideoStatus InitRendering(char *name, const uint32_t width, const uint32_t height) {
    VideoStatus status;

    // create the window needed
    status.window   = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);                 // window
    status.renderer = SDL_CreateRenderer(status.window, -1, SDL_RENDERER_ACCELERATED);                                          // renderer
    status.screen   = SDL_CreateTexture(status.renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height); // screen texture

    // view port size
    status.vp_width = width, status.vp_height = height;

    // Video metadata
    status.running    = 1;
    status.frameCount = 0;

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

int StartVideo(char *name, const uint32_t width, const uint32_t height, 
void (*HandleEvents)(VideoStatus *), void (*Update)(VideoStatus *), void (*Render)(uint32_t *, int, VideoStatus *)) {
    // Init video
    if (InitVideo()) return 1;

    // create video status
    VideoStatus status = InitRendering(name, width, height);

    while (status.running) {
        // first poll events
        PollEvents(&status, HandleEvents);

        // call the Update function of user
        if (Update) (*Update)(&status);

        // lock the texture data declaration
        void *pixels;
        int  lockWidth;

        // lock texture
        if (SDL_LockTexture(status.screen, NULL, &pixels, &lockWidth)) {
            fprintf(stderr, "Error while locking texture: %s\n", SDL_GetError());
            return 1;
        }

        // call the Render function
        if (Render) (*Render)(pixels, lockWidth, &status);

        // unlock texture
        SDL_UnlockTexture(status.screen);

        // swap buffers
        FrameUpdate(&status);
    }

    // destroy every thing
    DestroyVideo(&status);

    return 0;
}

void DestroyVideo(VideoStatus *status) {
    // destroy
    SDL_DestroyTexture (status->screen);   // screen texture
    SDL_DestroyRenderer(status->renderer); // renderer
    SDL_DestroyWindow  (status->window);   // window

    // quits SDL
    SDL_Quit();
}