#include "debugmalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <time.h>
#include <SDL2/SDL_image.h>

#include "pontkezeles.h"
#include "jatekmenet.h"
#include "jatekvezerles.h"
#include "grafika.h"

void sdlInic(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
void sdlKilep(Graf* graf, SDL_Renderer* renderer, SDL_Window *window);

int main(int argc, char *argv[]) {
    Allapot a = Fomenu;
    Allapot a_el = Fomenu;
    SDL_Window *window = NULL;
    SDL_Renderer* renderer = NULL;
    sdlInic(SZELES, MAGAS, &window, &renderer);
    Graf graf;
    grafikaInic(&graf, renderer);
    Jatek j;

    while (a != Kilepes) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch(a) {
            case Fomenu:
                fomenu(&a, &a_el, &j, renderer);
                break;
            case Egyjatekos:
                egyjatekos(&a, &a_el, &j, renderer, &graf);
                break;
            case Ketjatekos:
                ketjatekos(&a, &a_el, &j, renderer, &graf);
                break;
            case Jatekvege:
                jatekvege(&a, &a_el, &j, renderer, &graf);
                break;
            default:
                fomenu(&a, &a_el, &j, renderer);
                break;
        }

    }
    sdlKilep(&graf, renderer, window);
    return 0;
}

void sdlInic(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(2);
    }
    SDL_Window *window = SDL_CreateWindow("Kigyosch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(2);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(2);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}
void sdlKilep(Graf* graf, SDL_Renderer* renderer, SDL_Window *window) {
    TTF_CloseFont(graf->font);
    TTF_CloseFont(graf->fontSzoveg);
    SDL_FreeSurface(graf->ponttabla.sSzoveg1);
    SDL_FreeSurface(graf->ponttabla.sSzoveg2);
    SDL_DestroyTexture(graf->ponttabla.tPont1);
    SDL_DestroyTexture(graf->ponttabla.tPont2);
    SDL_DestroyTexture(graf->ponttabla.tSzoveg1);
    SDL_DestroyTexture(graf->ponttabla.tSzoveg2);
    SDL_DestroyTexture(graf->sprite);
    SDL_DestroyTexture(graf->palya);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
