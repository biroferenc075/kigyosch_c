#include "debugmalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "pontkezeles.h"
#include "jatekmenet.h"
#include "jatekvezerles.h"
#include "grafika.h"



void grafikaInic(Graf *graf, SDL_Renderer *renderer) {
    TTF_Init();
    graf->font = TTF_OpenFont("3Dventure.ttf", 24);
    if (graf->font == NULL) {
        SDL_Log("Nem lehet inicializalni a szovegkezelest: %s", SDL_GetError());
        exit(2);
    }
    graf->fontSzoveg = TTF_OpenFont("VCR_OSD_MONO_1.001.ttf", 24);
    if (graf->font == NULL) {
        SDL_Log("Nem lehet inicializalni a szovegkezelest: %s", SDL_GetError());
        exit(2);
    }
    graf->palya = IMG_LoadTexture(renderer,"map.png");
    if (graf->palya == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
    graf->sprite=IMG_LoadTexture(renderer,"snake.png");
    if (graf->sprite == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
    graf->szovegszin = (SDL_Color){255, 255, 200, 255};
    graf->ponttabla.sSzoveg1 = TTF_RenderText_Solid(graf->font, "Jatekos 1 pontszam:", graf->szovegszin);
    graf->ponttabla.sSzoveg2 = TTF_RenderText_Solid(graf->font, "Jatekos 2 pontszam:", graf->szovegszin);
    graf->ponttabla.tSzoveg1 = SDL_CreateTextureFromSurface(renderer, graf->ponttabla.sSzoveg1);
    graf->ponttabla.tSzoveg2 = SDL_CreateTextureFromSurface(renderer, graf->ponttabla.sSzoveg2);

    graf->ponttabla.sPont1 = TTF_RenderText_Solid(graf->font, "0", graf->szovegszin);
    graf->ponttabla.sPont2 = TTF_RenderText_Solid(graf->font, "0", graf->szovegszin);
    graf->ponttabla.tPont1 = SDL_CreateTextureFromSurface(renderer, graf->ponttabla.sPont1);
    graf->ponttabla.tPont2 = SDL_CreateTextureFromSurface(renderer, graf->ponttabla.sPont2);

    graf->ponttabla.SzovegR1.x = 40;
    graf->ponttabla.SzovegR1.y = 0;
    graf->ponttabla.SzovegR1.w = 400;
    graf->ponttabla.SzovegR1.h = 40;

    graf->ponttabla.PontR1.x = 460;
    graf->ponttabla.PontR1.y = 0;
    graf->ponttabla.PontR1.w = 20;
    graf->ponttabla.PontR1.h = 40;

    graf->ponttabla.SzovegR2.x = 1260;
    graf->ponttabla.SzovegR2.y = 0;
    graf->ponttabla.SzovegR2.w = 400;
    graf->ponttabla.SzovegR2.h = 40;

    graf->ponttabla.PontR2.x = 1700;
    graf->ponttabla.PontR2.y = 0;
    graf->ponttabla.PontR2.w = 20;
    graf->ponttabla.PontR2.h = 40;

    SDL_FreeSurface(graf->ponttabla.sSzoveg1);
    SDL_FreeSurface(graf->ponttabla.sSzoveg2);
    SDL_FreeSurface(graf->ponttabla.sPont1);
    SDL_FreeSurface(graf->ponttabla.sPont2);

    graf->ponttabla.sSzoveg1 = NULL;
    graf->ponttabla.sSzoveg2 = NULL;
    graf->ponttabla.sPont1 = NULL;
    graf->ponttabla.sPont1 = NULL;
}
void kigyoKirajzol(Elem** const fej, SDL_Renderer *renderer, Irany i, SDL_Texture *t, int jatekos) {
    int nyelvx = 0;
    int nyelvy = 0;
    switch(i){
        case Fel:
            nyelvy=-1;
            break;
        case Le:
            nyelvy=1;
            break;
        case Bal:
            nyelvx=-1;
            break;
        case Jobb:
            nyelvx=1;
            break;
    }
    SDL_Rect sfej = {((jatekos-1)*4+i)*MERET, 0, MERET, MERET };
    SDL_Rect dfej = {((*fej)->koord.x+1)*MERET,((*fej)->koord.y+2)*MERET, MERET, MERET };
    SDL_RenderCopy(renderer, t, &sfej, &dfej);

    SDL_Rect snyelv = {((jatekos-1)*4+i)*MERET, 5*MERET, MERET, MERET};
    SDL_Rect dnyelv = {((*fej)->koord.x+1+nyelvx)*MERET,((*fej)->koord.y+2+nyelvy)*MERET, MERET, MERET };
    SDL_RenderCopy(renderer, t, &snyelv, &dnyelv);


    SDL_Rect sTest = {((jatekos-1)*4+(*fej)->irany)*MERET, 3*MERET, MERET, MERET};

    Elem *lemarado = *fej;
    Elem *p;
    for(p = (*fej)->kov; p->kov!=NULL; p=p->kov) {
        if(p->kov->koord.x==p->koord.x&&p->kov->koord.y==p->koord.y) {
            break;
        }
        sTest.x = ((jatekos-1)*4+(p->irany))*MERET;
        sTest.y = 3*MERET;
        sTest.w = MERET;
        sTest.h = MERET;
        SDL_Rect dTest = {(p->koord.x+1)*MERET,(p->koord.y+2)*MERET, MERET, MERET };
        if(p->irany==lemarado->irany) {
                sTest.y = 1*MERET;
                sTest.x = ((jatekos-1)*4+lemarado->irany)*MERET;

        }
        else
            if((lemarado->irany==Jobb&&p->irany==Le)||(lemarado->irany==Le&&p->irany==Bal)||(lemarado->irany==Bal&&p->irany==Fel)||(lemarado->irany==Fel&&p->irany==Jobb)) {
                sTest.y = 2*MERET;
                sTest.x = ((jatekos-1)*4+lemarado->irany)*MERET;
            }
        SDL_RenderCopy(renderer, t, &sTest, &dTest);
        lemarado=p;
    }
    if((lemarado->kov->koord.x !=((*fej)->koord.x))||(lemarado->kov->koord.y!=((*fej)->koord.y))) {
        SDL_Rect sFarok = {((jatekos-1)*4+lemarado->irany)*MERET, 4*MERET, MERET, MERET};
        SDL_Rect dFarok = {(p->koord.x+1)*MERET,(p->koord.y+2)*MERET, MERET, MERET};
        SDL_RenderCopy(renderer, t, &sFarok, &dFarok);
    }
}
void almaKirajzol(Alma* const alma, SDL_Renderer *renderer, SDL_Texture *t) {
    SDL_Rect sAlma = {0, 6*MERET, MERET, MERET};
    SDL_Rect dAlma = {(alma->koord.x+1)*MERET,(alma->koord.y+2)*MERET, MERET, MERET };
    SDL_RenderCopy(renderer, t, &sAlma, &dAlma);
}
void pontszamUjraRender(Graf *graf, SDL_Renderer *renderer, Jatek* j, int jatekos) {
    if(jatekos == 1) {
        if(j->psz[0].pontszam>9) {
            graf->ponttabla.PontR1.w = 40;
        }
        else {
            graf->ponttabla.PontR1.w = 20;
        }
        sprintf(j->psz[0].pontstr, "%d",j->psz[0].pontszam);
        graf->ponttabla.sPont1 = TTF_RenderText_Solid(graf->font, j->psz[0].pontstr, graf->szovegszin);
        graf->ponttabla.tPont1 = SDL_CreateTextureFromSurface(renderer, graf->ponttabla.sPont1);
        SDL_FreeSurface(graf->ponttabla.sPont1);
    }
    else {
        if(j->psz[1].pontszam>9) {
            graf->ponttabla.PontR2.w = 40;
        }
        else {
            graf->ponttabla.PontR2.w = 20;
        }
        sprintf(j->psz[1].pontstr, "%d",j->psz[1].pontszam);
        graf->ponttabla.sPont2 = TTF_RenderText_Solid(graf->font, j->psz[1].pontstr, graf->szovegszin);
        graf->ponttabla.tPont2 = SDL_CreateTextureFromSurface(renderer, graf->ponttabla.sPont2);
        SDL_FreeSurface(graf->ponttabla.sPont2);
    }
}
void pontszamKirajzol(Graf* graf, SDL_Renderer* renderer, int jatekosszam) {
    SDL_RenderCopy(renderer, graf->ponttabla.tPont1, NULL, &(graf->ponttabla.PontR1));
    SDL_RenderCopy(renderer, graf->ponttabla.tSzoveg1, NULL, &(graf->ponttabla.SzovegR1));
    if(jatekosszam==2) {
    SDL_RenderCopy(renderer, graf->ponttabla.tPont2, NULL, &(graf->ponttabla.PontR2));
    SDL_RenderCopy(renderer, graf->ponttabla.tSzoveg2, NULL, &(graf->ponttabla.SzovegR2));
    }
}
void rekordRenderel(Rekord* r, Graf* g, SDL_Renderer* renderer) {
    for(int i = 0; i < REKORDSZAM; i++) {
        char hely[4];
        sprintf(hely, "%.0d",i+1);
        sprintf(r[i].pont.pontstr, "%d",r[i].pont.pontszam);
        SDL_Surface* sHely = TTF_RenderText_Solid(g->fontSzoveg, hely, g->szovegszin);
        g->rekordtabla.tHely[i] = SDL_CreateTextureFromSurface(renderer, sHely);
        SDL_FreeSurface(sHely);
        SDL_Surface* sPont = TTF_RenderText_Solid(g->fontSzoveg, r[i].pont.pontstr, g->szovegszin);
        g->rekordtabla.tPont[i] = SDL_CreateTextureFromSurface(renderer, sPont);
        SDL_FreeSurface(sPont);
        SDL_Surface* sNev = TTF_RenderUTF8_Solid(g->fontSzoveg, r[i].nev, g->szovegszin);;
        g->rekordtabla.tNev[i] = SDL_CreateTextureFromSurface(renderer, sNev);
        SDL_FreeSurface(sNev);
    }
}
void rekordKirajzol(Rekord* r, Graf* g, SDL_Renderer* renderer) {
    for(int i =0; i < REKORDSZAM; i++) {
        int helySzeles = (1+(i+1)/10);
        SDL_Rect dHely = {360-(11*helySzeles)/2, 350+i*21, 11*helySzeles, 21};
        SDL_RenderCopy(renderer, g->rekordtabla.tHely[i], NULL, &dHely);
        SDL_Rect dPont = {430-(11*strlen(r[i].pont.pontstr))/2,350+i*21, 11*strlen(r[i].pont.pontstr), 21};
        SDL_RenderCopy(renderer, g->rekordtabla.tPont[i], NULL, &dPont);
        SDL_Rect dNev = {595-(11*strlen(r[i].nev))/2,350+i*21, 11*strlen(r[i].nev), 21};
        SDL_RenderCopy(renderer, g->rekordtabla.tNev[i], NULL, &dNev);
    }
}


