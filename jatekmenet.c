#include "debugmalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <time.h>
#include <SDL_image.h>

#include "pontkezeles.h"
#include "jatekmenet.h"
#include "jatekvezerles.h"
#include "grafika.h"

Pont pontMozgat(Pont p, Irany i){
    Pont pont = p;
    switch(i) {
        case Fel:
            pont.y--;
            break;
        case Jobb:
            pont.x++;
            break;
        case Le:
            pont.y++;
            break;
        case Bal:
            pont.x--;
            break;
    }
    return pont;
}
bool kigyoInic(Elem** fej, int hossz, Pont kezd, Irany i) {
    Elem* p;
    *fej = (Elem*) malloc(sizeof(Elem));
    if(*fej==NULL)
        return false;
    else {
        p = *fej;
        for(int i =0; i < hossz-1; i++)
        {
            p->kov = (Elem*) malloc(sizeof(Elem));
            if(p->kov==NULL)
                return false;
            p->koord = kezd;
            p->irany = i;
            p=p->kov;
        }
        p->koord = kezd;
        p->kov = NULL;
        p->irany = i;
    return true;
    }
}
Elem* kigyoVegetKeres(Elem** const fej) {
    Elem* p;
    for(p = *fej; p->kov!=NULL; p=p->kov)
        ;
    return p;
}
bool kigyoNyujt(Elem** vege, Pont adat, Irany i) {
    (*vege)->kov = (Elem*) malloc(sizeof(Elem));
    if((*vege)->kov == NULL) {
        return false;
    }
    *vege = (*vege)->kov;
    (*vege)->koord=adat;
    (*vege)->kov=NULL;
    (*vege)->irany=i;
    return true;
}
void kigyoFelszabadit (Elem** fej) {
    Elem* p = *fej;
    while(p != NULL) {
        Elem* kov = p->kov;
        free(p);
        p = kov;
    }
    *fej = NULL;
}
void kigyoLeptet(Elem** const fej, Irany const i) {
    Elem *p = *fej;
    Pont tmp1 = p->koord;
    Pont tmp2 = tmp1;
    Irany tmi1 = p->irany;
    Irany tmi2 = tmi1;
    p->koord = pontMozgat(p->koord, i);
    p->irany = i;
    while(p->kov!=NULL)
    {
        p=p->kov;
        tmp2=p->koord;
        tmi2=p->irany;
        p->koord=tmp1;
        p->irany=tmi1;
        tmp1=tmp2;
        tmi1=tmi2;
    }
}
Utkozes kigyoUtkozes(Elem** const fej1, Elem** const  fej2, Alma* const alma) {
    for(Elem* p  = (*fej1)->kov;p!=NULL;p=p->kov)
        if((*fej1)->koord.x==p->koord.x&&(*fej1)->koord.y==p->koord.y)
                return Kigyoval;
    if(fej2!=NULL)
        for(Elem* p =*fej2;p!=NULL;p=p->kov)
            if((*fej1)->koord.x==p->koord.x&&(*fej1)->koord.y==p->koord.y)
                return Kigyoval;
    if((*fej1)->koord.x<0||(*fej1)->koord.x>41||(*fej1)->koord.y<0||(*fej1)->koord.y>20)
            return Fallal;
    if((*fej1)->koord.x==alma->koord.x&&(*fej1)->koord.y==alma->koord.y)
        return Almaval;
    return Semmivel;
}
void utkozesKezeles(Allapot* a, Jatek* j, Graf* g, SDL_Renderer* renderer) {
    if(j->jatekosszam==1) {
        switch(kigyoUtkozes(&j->fej[0], NULL, &j->alma)) {
            case Fallal:
            case Kigyoval:
                *a=Jatekvege;
                break;
            case Semmivel:
                break;
            case Almaval:
                if(!kigyoNyujt(&j->vege[0], j->vege[0]->koord,j->vege[0]->irany))
                    exit (-1);
                almaUj(&j->alma, &j->fej[0], NULL);
                j->psz[0].pontszam++;
                pontszamUjraRender(g, renderer, j, 1);
                break;
        }
    }
    else {
        switch(kigyoUtkozes(&j->fej[0], &j->fej[1], &j->alma)) {
            case Fallal:
            case Kigyoval:
                j->gyoztes=Nyert2;
                break;
            case Semmivel:
                break;
            case Almaval:
                if(!kigyoNyujt(&j->vege[0], j->vege[0]->koord,j->vege[0]->irany))
                    exit (-1);
                almaUj(&j->alma, &j->fej[0], &j->fej[1]);
                j->psz[0].pontszam++;
                pontszamUjraRender(g, renderer, j, 1);
                break;
            }
        switch(kigyoUtkozes(&j->fej[1], &j->fej[0], &j->alma)) {
            case Fallal:
            case Kigyoval:
                if(j->gyoztes==Nyert2)
                    j->gyoztes=Dontetlen;
                else
                    j->gyoztes=Nyert1;
                break;
            case Semmivel:
                break;
            case Almaval:
                if(!kigyoNyujt(&j->vege[1], j->vege[1]->koord,j->vege[1]->irany))
                    exit (-1);
                almaUj(&j->alma, &j->fej[0], &j->fej[1]);
                j->psz[1].pontszam++;
                pontszamUjraRender(g, renderer, j, 2);
                break;
        }
    }
}
void almaUj(Alma* alma, Elem** const fej1, Elem** const fej2) {
    bool ujat = false;
    do {
        ujat = false;
        alma->koord.x = rand()%42;
        alma->koord.y = rand()%21;
        for(Elem* p = *fej1; p!=NULL; p=p->kov)
            if(alma->koord.x==p->koord.x&&alma->koord.y==p->koord.y) {
            ujat = true;
            break;
            }
        if(fej2!=NULL)
            for(Elem* p = *fej2; p!=NULL; p=p->kov)
                if(alma->koord.x==p->koord.x&&alma->koord.y==p->koord.y) {
                ujat = true;
                break;
            }
    } while(ujat);
}

Uint32 idozit(Uint32 ms, void *jatekido) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    (*((int*) jatekido))++;
    return ms;
}
bool jatekInic(Jatek* j, int jatekosszam) {
    j->psz[0].pontszam = 0;
    j->psz[1].pontszam = 0;
    srand(time(0));
    j->d[0] = Jobb;
    j->del[0] = Jobb;
    j->d[1] = Bal;
    j->del[1] = Bal;
    j->kezd[0].x = 10;
    j->kezd[0].y = 10;
    j->kezd[1].x = 31;
    j->kezd[1].y = 10;
    j->gyoztes=NemDoltEl;

    j->fej[0] = NULL;
    j->fej[1] = NULL;
    if(jatekosszam == 2) {
        j->jatekosszam = 2;
    }
    else {
        j->jatekosszam = 1;
    }
    j->jatekido = 0;

    if(!kigyoInic(&(j->fej[0]), 6, j->kezd[0], j->d[0])) {
        return false;
    }
    j->vege[0] = kigyoVegetKeres(&(j->fej[0]));

    if(jatekosszam == 2) {
        if(!kigyoInic(&(j->fej[1]), 6, j->kezd[1], j->d[1])) {
            return false;
        }
        j->vege[1] = kigyoVegetKeres(&(j->fej[1]));
    }
    if(jatekosszam == 2) {
        almaUj(&(j->alma), &(j->fej[0]), &(j->fej[1]));
    }
    else {
        j->alma.koord.x = 21;
        j->alma.koord.y = 10;
    }
    j->id = SDL_AddTimer(100, idozit, &(j->jatekido));
    return true;
}
void billentyuzetKezel(Jatek* j, SDL_Event* event) {
    if(j->jatekosszam==1){
        switch(event->key.keysym.sym) {
            case SDLK_d:
            case SDLK_RIGHT:
                if(j->del[0]!=Bal)
                    j->d[0] = Jobb;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                if(j->del[0]!=Fel)
                    j->d[0]= Le;
                break;
            case SDLK_a:
            case SDLK_LEFT:
                if(j->del[0]!=Jobb)
                    j->d[0] = Bal;
                break;
            case SDLK_w:
            case SDLK_UP:
                if(j->del[0]!=Le)
                    j->d[0] = Fel;
                break;
            default:
                break;
                    }
    }
    else {
        switch(event->key.keysym.sym) {
            case SDLK_d:
                if(j->del[0]!=Bal)
                    j->d[0] = Jobb;
                break;
            case SDLK_RIGHT:
                if(j->del[1]!=Bal)
                    j->d[1] = Jobb;
                break;
            case SDLK_s:
                if(j->del[0]!=Fel)
                    j->d[0]= Le;
                break;
            case SDLK_DOWN:
                if(j->del[1]!=Fel)
                    j->d[1]= Le;
                break;
            case SDLK_a:
                if(j->del[0]!=Jobb)
                    j->d[0] = Bal;
                break;
            case SDLK_LEFT:
                if(j->del[1]!=Jobb)
                    j->d[1] = Bal;
                break;
            case SDLK_w:
                if(j->del[0]!=Le)
                    j->d[0] = Fel;
                break;
            case SDLK_UP:
                if(j->del[1]!=Le)
                    j->d[1] = Fel;
                break;
            default:
                break;
        }
    }
 }
