#ifndef GRAFIKA_H_INCLUDED
#define GRAFIKA_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

enum {MAGAS = 960};
enum {SZELES = 1760};
enum {MERET = 40};

enum {GOMBMAGAS = 85};
enum {GOMBSZELES = 428};
enum {NAGYGOMBMAGAS = 85};
enum {NAGYGOMBSZELES = 605};

typedef struct Ponttabla {
    SDL_Rect SzovegR1, PontR1, SzovegR2, PontR2;
    SDL_Surface *sSzoveg1, *sPont1, *sSzoveg2, *sPont2;
    SDL_Texture *tSzoveg1, *tPont1, *tSzoveg2, *tPont2;
} Ponttabla;

typedef struct Rekordtabla {
    SDL_Texture* tHely[REKORDSZAM], *tPont[REKORDSZAM], *tNev[REKORDSZAM];
} Rekordtabla;

typedef struct Graf {
TTF_Font *font, *fontSzoveg;
SDL_Texture *sprite, *palya;
SDL_Color szovegszin;
Ponttabla ponttabla;
Rekordtabla rekordtabla;
} Graf;


void grafikaInic(Graf *graf, SDL_Renderer *renderer);
void kigyoKirajzol(Elem** const fej, SDL_Renderer *renderer, Irany i, SDL_Texture *t, int jatekos);
void almaKirajzol(Alma* const alma, SDL_Renderer *renderer, SDL_Texture *t);
void pontszamUjraRender(Graf *graf, SDL_Renderer *renderer, Jatek *j, int jatekos);
void pontszamKirajzol(Graf* graf, SDL_Renderer* renderer, int jatekosszam);
void rekordRenderel(Rekord* r, Graf* g, SDL_Renderer* renderer);
void rekordKirajzol(Rekord* r, Graf* g, SDL_Renderer* renderer);

#endif // GRAFIKA_H_INCLUDED
