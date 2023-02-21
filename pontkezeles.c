#include "debugmalloc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pontkezeles.h"
#include "jatekmenet.h"
#include "jatekvezerles.h"
#include "grafika.h"

void rekordBeolvas(Rekord* r) {
    FILE* ftop = fopen("toplista.txt", "r");
    for(int i = 0; i< REKORDSZAM; i++) {
        int szam;
        fscanf(ftop, "#%d %d ", &szam, &(r[i].pont.pontszam));
        fgets(r[i].nev, 25, ftop);
        bool tulhosszu=true;
        for(int j = 0; r[i].nev[j]!='\0'; j++) {
            if(r[i].nev[j]=='\n')
                {
                    r[i].nev[j]='\0';
                    tulhosszu = false;
                }
        }
        if(tulhosszu) {
            return;
        }
    }
    fclose(ftop);
}
void rekordKezel(Jatek* j, Rekord* r, Graf* graf, SDL_Renderer* renderer) {
    bool rekorde = false;
    int helyezes = -1;
    for(int i = 0; i < REKORDSZAM; i++) {
        if(r[i].pont.pontszam<j->psz[0].pontszam) {
         rekorde = true;
         helyezes = i;
         break;
        }
    }
    if(rekorde) {
        char neve[25];
        bool szerepelnagyobbal = false;
        do {
        input_text(neve, 24, (SDL_Rect){380, 800, 320, 60},(SDL_Color){75,75,60,255}, (SDL_Color){200,200,145,255}, graf->fontSzoveg, renderer);
        } while(neve[0]=='\0');
        for(int i = 0; i<helyezes; i++) {
            if(strcmp(r[i].nev,neve)==0) {
                szerepelnagyobbal = true;
                break;
            }
        }
        if(!szerepelnagyobbal) {
            Rekord tmp1;
            tmp1.pont = j->psz[0];
            strcpy(tmp1.nev,neve);
            for(int i = helyezes; i<REKORDSZAM; i++) {
                Rekord tmp2 = r[i];
                strcpy(r[i].nev,tmp1.nev);
                r[i].pont.pontszam=tmp1.pont.pontszam;
                if(strcmp(tmp2.nev,neve)==0)
                    break;
                tmp1 = tmp2;
                }
        }
    }
}
void rekordMent(Rekord* r) {
    FILE* ftop = fopen("toplista.txt", "w");
    for(int i = 0; i< REKORDSZAM; i++) {
        fprintf(ftop, "#%.0d %d %s\n",i+1, r[i].pont.pontszam, r[i].nev);
    }
    fclose(ftop);
}
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) {
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;

    dest[0] = '\0';

    bool enter = false;
    bool kilep = false;

    SDL_StartTextInput();
    while (!kilep && !enter) {

        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);

        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x+(teglalap.w-felirat->w)/2, teglalap.y+teglalap.h/2-felirat->h/2, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, teglalap.x+(teglalap.w+w)/2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }

        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;

            /* A feldolgozott szoveg bemenete */
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) < hossz) {
                    strcat(dest, event.text.text);
                }

                /* Az eddigi szerkesztes torolheto */
                composition[0] = '\0';
                break;

            /* Szoveg szerkesztese */
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;

            case SDL_QUIT:
                /* visszatesszuk a sorba ezt az eventet, mert
                 * sok mindent nem tudunk vele kezdeni */
                SDL_PushEvent(&event);
                kilep = true;
                break;
        }
    }

    /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);

        /* szoveg kirajzolasa */
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x+(teglalap.w-felirat->w)/2, teglalap.y + teglalap.h/2 - felirat->h/2, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);

    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}

