#ifndef MENUKEZELES_H_INCLUDED
#define MENUKEZELES_H_INCLUDED

typedef enum Allapot { Fomenu = 0, Egyjatekos = 1, Ketjatekos = 2, Jatekvege = 3, Kilepes = 4} Allapot;

typedef struct Gomb {
    SDL_Rect sGomb, dGomb;
    SDL_Texture *texture, *texturekiv;
    Allapot kov;
} Gomb;

#include "grafika.h"

void gombFeldolgoz(Gomb *gomb, Allapot *a_el, const SDL_Event *ev, SDL_Renderer *renderer);
void fomenu(Allapot* a, Allapot* a_el, Jatek* j, SDL_Renderer* renderer);
void visszaszamol(Jatek* j, SDL_Renderer* renderer, Graf* g, SDL_Rect* ablak);
void egyjatekos(Allapot* a, Allapot* a_el, Jatek* j, SDL_Renderer* renderer, Graf* g);
void ketjatekos(Allapot* a, Allapot* a_el, Jatek* j, SDL_Renderer* renderer, Graf* g);
void jatekvege(Allapot* a, Allapot* a_el, Jatek* j,SDL_Renderer* renderer, Graf* g);


#endif // MENUKEZELES_H_INCLUDED
