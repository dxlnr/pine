#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1280

int err(int c) { 
    if (c < 0) {
        printf("SDL error : %s\n", SDL_GetError());
        exit(1);
    }
    return c;
}

void *errptr(void *ptr) {
    if (ptr == NULL) {
        printf("SDL error : %s\n", SDL_GetError());
        exit(1);
    }

    return ptr;
}

int main(int argc, char *argv[])
{
    err(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window *w = errptr(SDL_CreateWindow(
            "Game of Life", 
            100, 100, 
            SCREEN_WIDTH, SCREEN_HEIGHT, 
            SDL_WINDOW_RESIZABLE));

    SDL_Renderer *const r = errptr(SDL_CreateRenderer(
            w, -1,
            SDL_RENDERER_ACCELERATED));

    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = 1;
            } break;
            }
        }

        err((SDL_RenderClear(r)));
        SDL_RenderPresent(r);
    }

    SDL_Quit();
    return 0;
}
