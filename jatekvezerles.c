#include "debugmalloc.h"

#include "pontkezeles.h"
#include "jatekmenet.h"
#include "jatekvezerles.h"
#include "grafika.h"

void gombFeldolgoz(Gomb *gomb, Allapot *a, const SDL_Event *ev, SDL_Renderer *renderer) {
    bool kiv = false;
    bool nyomva = false;
    if(ev->type == SDL_MOUSEMOTION)
        if(ev->motion.x >= gomb->dGomb.x && ev->motion.x <= (gomb->dGomb.x + gomb->dGomb.w) && ev->motion.y >= gomb->dGomb.y && ev->motion.y <= (gomb->dGomb.y + gomb->dGomb.h))
            kiv = true;
    if(ev->type == SDL_MOUSEBUTTONDOWN) {
        if(ev->button.button == SDL_BUTTON_LEFT && ev->button.x >= gomb->dGomb.x && ev->button.x <= (gomb->dGomb.x + gomb->dGomb.w) && ev->button.y >= gomb->dGomb.y && ev->button.y <= (gomb->dGomb.y + gomb->dGomb.h)) {
        nyomva = true;
        }
    }
    if(kiv)
        SDL_RenderCopy(renderer, gomb->texturekiv, &gomb->sGomb, &gomb->dGomb);
    else
        SDL_RenderCopy(renderer, gomb->texture, &gomb->sGomb, &gomb->dGomb);
    if(nyomva)
        *a=gomb->kov;
}
void fomenu(Allapot* a, Allapot* a_el, Jatek* j, SDL_Renderer* renderer) {
    j->fej[0]=NULL;
    j->fej[1]=NULL;
    *a = Fomenu;
    SDL_Texture* tGombok = IMG_LoadTexture(renderer,"menu.png");
    SDL_Texture* tGombokKiv = IMG_LoadTexture(renderer,"menukiv.png");
    SDL_Texture* menuHatter = IMG_LoadTexture(renderer, "menuhatter.png");
    Gomb egyj = { {0,0,          GOMBSZELES,GOMBMAGAS},{SZELES/2-GOMBSZELES/2, 360,GOMBSZELES, GOMBMAGAS},tGombok,tGombokKiv,Egyjatekos};
    Gomb ketj = { {0,1*GOMBMAGAS,GOMBSZELES,GOMBMAGAS},{SZELES/2-GOMBSZELES/2, 510,GOMBSZELES, GOMBMAGAS},tGombok,tGombokKiv,Ketjatekos};
    Gomb topl =  { {0,2*GOMBMAGAS,GOMBSZELES,GOMBMAGAS},{SZELES/2-GOMBSZELES/2, 660,GOMBSZELES, GOMBMAGAS},tGombok,tGombokKiv,Jatekvege};
    Gomb kil =  { {0,3*GOMBMAGAS,GOMBSZELES,GOMBMAGAS},{SZELES/2-GOMBSZELES/2, 810,GOMBSZELES, GOMBMAGAS},tGombok,tGombokKiv,Kilepes};
    SDL_Rect ablak = {0,0,SZELES,MAGAS};
    while(*a == Fomenu) {
        SDL_Event ev;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menuHatter, NULL, &ablak);
        switch (ev.type) {

            case SDL_QUIT:
                *a=Kilepes;
                break;
            default:
                gombFeldolgoz(&egyj, a, &ev, renderer);
                gombFeldolgoz(&ketj, a, &ev, renderer);
                gombFeldolgoz(&topl, a, &ev, renderer);
                gombFeldolgoz(&kil , a, &ev, renderer);
        }
        SDL_RenderPresent(renderer);
        if(*a == Fomenu) {
            SDL_WaitEvent(&ev);
        }
    }
    SDL_DestroyTexture(tGombok);
    SDL_DestroyTexture(tGombokKiv);
    SDL_DestroyTexture(menuHatter);
    *a_el = Fomenu;
}
void visszaszamol(Jatek* j, SDL_Renderer* renderer, Graf* g, SDL_Rect* ablak) {

    SDL_Texture *cnt3 = IMG_LoadTexture(renderer,"cnt3.png");
    SDL_Texture *cnt2 = IMG_LoadTexture(renderer,"cnt2.png");
    SDL_Texture *cnt1 = IMG_LoadTexture(renderer,"cnt1.png");

    SDL_Event event;
    while(j->jatekido<16)
    {
        SDL_RenderCopy(renderer, g->palya, NULL, ablak);
        kigyoKirajzol(&j->fej[0], renderer, j->d[0], g->sprite, 1);
        if(j->jatekosszam==2) {
            kigyoKirajzol(&j->fej[1], renderer, j->d[1], g->sprite, 2);
        }
        almaKirajzol(&j->alma, renderer, g->sprite);
        pontszamKirajzol(g, renderer, j->jatekosszam);
        SDL_WaitEvent(&event);
        if(event.type == SDL_USEREVENT) {
            if(j->jatekido<=5)
                SDL_RenderCopy(renderer, cnt3, NULL, ablak);
            if(j->jatekido>=6&&j->jatekido<=10)
                SDL_RenderCopy(renderer, cnt2, NULL, ablak);
            if(j->jatekido>=11&&j->jatekido<=15)
                SDL_RenderCopy(renderer, cnt1, NULL, ablak);
            SDL_RenderPresent(renderer);
        }

    }

    SDL_DestroyTexture(cnt3);
    SDL_DestroyTexture(cnt2);
    SDL_DestroyTexture(cnt1);
}
void egyjatekos(Allapot* a, Allapot* a_el,Jatek* j, SDL_Renderer* renderer, Graf* g) {
    *a = Egyjatekos;
    jatekInic(j, 1);
    pontszamUjraRender(g, renderer, j, 1);
    SDL_Event event;
    SDL_Rect ablak = {0, 0, SZELES, MAGAS};
    visszaszamol(j, renderer, g, &ablak);
    while(*a==Egyjatekos) {
        switch(event.type) {
                case SDL_USEREVENT:
                    SDL_RenderCopy(renderer, g->palya, NULL, &ablak);
                    kigyoLeptet(&j->fej[0], j->d[0]);
                    utkozesKezeles(a, j, g, renderer);
                    kigyoKirajzol(&j->fej[0], renderer, j->d[0], g->sprite, 1);
                    almaKirajzol(&j->alma, renderer, g->sprite);
                    pontszamKirajzol(g, renderer, j->jatekosszam);
                    SDL_RenderPresent(renderer);
                    j->del[0] = j->d[0];
                    break;
                case SDL_QUIT:
                    *a = Kilepes;
                    break;
                case SDL_KEYDOWN:
                    billentyuzetKezel(j, &event);
                    break;
                default:
                    break;
                }
        if(*a==Egyjatekos)
            SDL_WaitEvent(&event);

    }
    kigyoFelszabadit(&(j->fej[0]));
    *a_el=Egyjatekos;
}
void ketjatekos(Allapot* a, Allapot* a_el, Jatek* j, SDL_Renderer* renderer, Graf* g) {
    *a = Ketjatekos;
    jatekInic(j, 2);
    SDL_Event event;
    SDL_Rect ablak = {0, 0, SZELES, MAGAS};
    visszaszamol(j, renderer, g, &ablak);

    pontszamUjraRender(g, renderer, j, 1);
    pontszamUjraRender(g, renderer, j, 2);
    while(*a==Ketjatekos) {
        switch(event.type) {
            case SDL_USEREVENT:
                SDL_RenderCopy(renderer, g->palya, NULL, &ablak);
                kigyoLeptet(&j->fej[0], j->d[0]);
                kigyoLeptet(&j->fej[1], j->d[1]);
                utkozesKezeles(a, j, g, renderer);
                kigyoKirajzol(&j->fej[0], renderer, j->d[0], g->sprite, 1);
                kigyoKirajzol(&j->fej[1], renderer, j->d[1], g->sprite, 2);
                almaKirajzol(&j->alma, renderer, g->sprite);
                pontszamKirajzol(g, renderer, j->jatekosszam);
                SDL_RenderPresent(renderer);
                j->del[0] = j->d[0];
                j->del[1] = j->d[1];
                break;
            case SDL_QUIT:
                *a = Kilepes;
                break;
            case SDL_KEYDOWN:
                billentyuzetKezel(j, &event);
                break;
            default:
                break;
        }
        if(j->gyoztes!=NemDoltEl) {
            *a=Jatekvege;
        }
        if(*a==Ketjatekos)
            SDL_WaitEvent(&event);
    }
    if(j->gyoztes==Dontetlen)
    {
        if(j->psz[0].pontszam > j->psz[1].pontszam) {
            j->gyoztes=Nyert1;
        }
        if(j->psz[1].pontszam > j->psz[0].pontszam) {
            j->gyoztes=Nyert2;
        }
    }
    kigyoFelszabadit(&(j->fej[0]));
    kigyoFelszabadit(&(j->fej[1]));
    *a_el=Ketjatekos;
}
void jatekvege(Allapot* a, Allapot* a_el, Jatek* j, SDL_Renderer* renderer, Graf* g) {
    *a = Jatekvege;
    if(*a_el != Fomenu) {
        SDL_RemoveTimer(j->id);
    }
    int eltolas = 0;
    SDL_Texture* tGombok = IMG_LoadTexture(renderer,"Ranglista.png");
    SDL_Texture* tGombokKiv = IMG_LoadTexture(renderer,"RanglistaKiv.png");
    SDL_Texture* menuHatter;
    if(*a_el==Fomenu) {
        menuHatter = IMG_LoadTexture(renderer, "ranglistahettermenubol.png");
    }
    else {
        if(*a_el==Egyjatekos) {
            menuHatter = IMG_LoadTexture(renderer, "ranglistahatteregyjatekos.png");
        }
        else {
            menuHatter = IMG_LoadTexture(renderer, "ranglistahatterketjatekos.png");
        }
        eltolas = 1;
    }
    Gomb ujjatek = { {0,0              ,NAGYGOMBSZELES,NAGYGOMBMAGAS},{25*MERET, (11-3*eltolas)*MERET,NAGYGOMBSZELES, NAGYGOMBMAGAS},tGombok,tGombokKiv, Fomenu};
    Gomb vissza  = { {0,1*NAGYGOMBMAGAS,NAGYGOMBSZELES,NAGYGOMBMAGAS},{25*MERET, (11+3*eltolas)*MERET,NAGYGOMBSZELES, NAGYGOMBMAGAS},tGombok,tGombokKiv, Fomenu};
    if(*a_el==Egyjatekos) {
        ujjatek.kov=Egyjatekos;
    } else {
        ujjatek.kov=Ketjatekos;
    }
    SDL_Rect ablak = {0,0,SZELES,MAGAS};
    Rekord rekordok[REKORDSZAM];
    bool rekordkezelve = false;
    SDL_Texture* tGyoztes = NULL;
    if(*a_el == Egyjatekos || *a_el == Fomenu) {
        rekordBeolvas(rekordok);
        rekordRenderel(rekordok, g, renderer);
    }
    else if(*a_el == Ketjatekos) {
        char gyoztesStr[25];
        sprintf(gyoztesStr, j->gyoztes==Dontetlen? "Döntetlen lett a játék!" : "%d. Játékos a gyõztes!", j->gyoztes);
        SDL_Surface* sGyoztes = TTF_RenderText_Solid(g->fontSzoveg, gyoztesStr, g->szovegszin);
        tGyoztes = SDL_CreateTextureFromSurface(renderer, sGyoztes);
        SDL_FreeSurface(sGyoztes);
    }
    SDL_Event ev;
    while(*a == Jatekvege) {
        SDL_RenderCopy(renderer, menuHatter, NULL, &ablak);
        if(*a_el == Egyjatekos || *a_el == Fomenu) {
            rekordKirajzol(rekordok, g, renderer);
        }
        else {
            SDL_Rect rGyoztes = {360,680,370,25};
            SDL_RenderCopy(renderer, tGyoztes, NULL, &rGyoztes);
        }
        switch (ev.type) {
            case SDL_QUIT:
                *a=Kilepes;
                break;
            default:
                if(*a_el!=Fomenu) {
                    gombFeldolgoz(&ujjatek, a, &ev, renderer);
                }
                gombFeldolgoz(&vissza , a, &ev, renderer);
        }
        SDL_RenderPresent(renderer);
        if(*a_el == Egyjatekos && !rekordkezelve) {
            rekordKezel(j, rekordok, g, renderer);
            rekordRenderel(rekordok, g, renderer);
            SDL_RenderCopy(renderer, menuHatter, NULL, &ablak);
            rekordkezelve = true;
        }
        if(*a==Jatekvege)
            SDL_WaitEvent(&ev);
    }
    if(*a_el==Egyjatekos) {
        rekordMent(rekordok);
    }
    SDL_DestroyTexture(tGombok);
    SDL_DestroyTexture(tGombokKiv);
    SDL_DestroyTexture(menuHatter);
    if(*a_el == Egyjatekos || *a_el == Fomenu)
    for(int i = 0; i < REKORDSZAM; i++) {
        SDL_DestroyTexture(g->rekordtabla.tHely[i]);
        SDL_DestroyTexture(g->rekordtabla.tPont[i]);
        SDL_DestroyTexture(g->rekordtabla.tNev[i]);
    }
    *a_el = Jatekvege;
}
