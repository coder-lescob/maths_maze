#include <stdio.h>
#include "renderer.h"

void Render(uint32_t *pixels, int width, VideoStatus *status) {
    // all pixels in red
    for (uint32_t y = 0; y < status->vp_height; y++) {
        for (uint32_t x = 0; x < status->vp_width; x++) {
            uint32_t *pix = (uint32_t *)((uint8_t *)pixels + y * width) + x;
            *pix = 0xFFFF0000; // darkblue
        }
    }
}

int main(void) {
    StartVideo("maths maze", 640, 480, NULL, NULL, Render);
    return 0;
}