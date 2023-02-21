#ifndef PONTKEZELES_H_INCLUDED
#define PONTKEZELES_H_INCLUDED

#include "debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

enum {REKORDSZAM = 10};

typedef struct Pontszam {
int pontszam;
char pontstr[4];
} Pontszam;


typedef struct Rekord {
    Pontszam pont;
    char nev[26];
} Rekord;

#include "jatekmenet.h"


void rekordBeolvas(Rekord* r);
void rekordKezel(Jatek* j, Rekord* r, Graf* graf, SDL_Renderer* renderer);
void rekordMent(Rekord* r);
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);


#endif // PONTKEZELES_H_INCLUDED
