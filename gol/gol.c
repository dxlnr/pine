#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <sys/types.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1280

#define N_CELLS 100

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

int * get_random(const int nMin, const int nMax) {

   static int r[N_CELLS];
   int i;
   srand((unsigned) time(NULL));
  
   for ( i = 0; i < 10; ++i) {
      r[i] = rand()%(nMax-nMin) + nMin;
   }
   return r;
}

static uint8_t univ[SCREEN_HEIGHT*SCREEN_WIDTH];

void init_game(uint8_t *u) 
{
    int *px, *py;
    px = get_random(0, SCREEN_WIDTH);
    py = get_random(0, SCREEN_HEIGHT);

    for(int i = 0; i < N_CELLS; ++i) {
        for(int j = 0; j < N_CELLS; ++j) {
            univ[*(py + i)* SCREEN_WIDTH + *(px + j)] = 1;
        }
    }
}

void render_state(SDL_Renderer *r, uint8_t *u)
{
    /* uint8_t (*univ)[w] = u; */
    err(SDL_SetRenderDrawColor(r, 255, 255, 255, 255));
    
    for (size_t i = 0; i < SCREEN_HEIGHT; ++i) {
        for (size_t j = 0; j < SCREEN_WIDTH; ++j) {
            if (u[i*SCREEN_WIDTH + j] == 1) {
                SDL_RenderDrawPoint(r, i, j);
            }
        }
    }
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

    init_game(univ);

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
        
        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
        err((SDL_RenderClear(r)));
        render_state(r, univ);
        SDL_RenderPresent(r);

    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
    
    return 0;
}
