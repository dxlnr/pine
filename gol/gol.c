#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <sys/types.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 960

#define GRID_WIDTH 240
#define GRID_HEIGHT 192

#define CELL_WIDTH ((float) SCREEN_WIDTH / GRID_WIDTH)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / GRID_HEIGHT)

#define N_CELLS 20000

static uint8_t univ[SCREEN_HEIGHT*SCREEN_WIDTH];

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

int *get_random(const int nMin, const int nMax) 
{
   static int r[N_CELLS];
   int i;
   srand((unsigned) time(NULL));
  
   for (i = 0; i < N_CELLS; ++i) {
      r[i] = rand() % (nMax - nMin) + nMin;
   }
   return r;
}

int get_random_int(int nMin, int nMax)
{
    return rand() % (nMax - nMin) + nMin;
}

void init_game_grid_like(uint8_t *u) 
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

void init_game(uint8_t *u) 
{
    for(int i = 0; i < N_CELLS; ++i) {
        univ[get_random_int(0, GRID_HEIGHT) * (int) CELL_HEIGHT * SCREEN_WIDTH + (get_random_int(0, GRID_WIDTH) * (int) CELL_WIDTH)] = 1;
    }
}

void render_sgl_cell(SDL_Renderer *r, int x, int y) 
{
    err(SDL_SetRenderDrawColor(r, 255, 255, 255, 255));
    SDL_Rect rect = {
        (int) floorf(x),
        (int) floorf(y),
        (int) floorf(CELL_WIDTH),
        (int) floorf(CELL_HEIGHT),
    };
    err(SDL_RenderFillRect(r, &rect));
}

void render_state(SDL_Renderer *r, uint8_t *u)
{
    for (int y = 0; y < SCREEN_HEIGHT; y += CELL_HEIGHT) {
        for (int x = 0; x < SCREEN_WIDTH; x += CELL_WIDTH) {
            if (u[y*SCREEN_WIDTH + x] == 1) {
                render_sgl_cell(r, x, y);
            }
        }
    }
}

void update(uint8_t *u) 
{
    uint8_t upd[SCREEN_HEIGHT][SCREEN_WIDTH];
    int n;
    for (size_t y = CELL_HEIGHT; y < SCREEN_HEIGHT; y += CELL_HEIGHT) {
        for (size_t x = CELL_HEIGHT; x < SCREEN_WIDTH; x += CELL_WIDTH) {
            n = 0;
            for (int yy = y - (int) CELL_HEIGHT; yy <= y + (int) CELL_HEIGHT; yy += (int) CELL_HEIGHT) {
                for (int xx = x - (int) CELL_WIDTH; xx <= x + (int) CELL_WIDTH; xx += (int) CELL_WIDTH) {
                    if (u[yy*SCREEN_WIDTH + xx] == 1) {
                        n++;
                    }
                }
            }
            if (u[y*SCREEN_WIDTH + x] == 1) n--;
            upd[y][x] = (n == 3 || (n == 2 && u[y*SCREEN_WIDTH + x]));
        }
    }
    for (size_t y = CELL_HEIGHT; y < SCREEN_HEIGHT; y += CELL_HEIGHT) {
        for (size_t x = CELL_HEIGHT; x < SCREEN_WIDTH; x += CELL_WIDTH) {
            univ[y* SCREEN_WIDTH + x] = upd[y][x];
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
        // Update the Game.
        update(univ);
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
    
    return 0;
}
