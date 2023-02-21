#ifndef GAMELOGIC_H_INCLUDED
#define GAMELOGIC_H_INCLUDED

#include <stdbool.h>
#include "SDL.h"

typedef enum Irany {
    Jobb=0, Le=1, Bal=2, Fel=3
} Irany;
typedef struct Pont {
int x, y;
} Pont;
typedef struct Elem {
    Pont koord;
    Irany irany;
    struct Elem* kov;
} Elem;
typedef enum Utkozes {
    Semmivel = 0, Kigyoval = 1, Almaval = 2, Fallal = 3
} Utkozes;
typedef struct Alma {
Pont koord;
} Alma;
typedef enum Gyoztes {
 NemDoltEl = 0, Nyert1 = 1, Nyert2 = 2, Dontetlen = 3
} Gyoztes;


typedef struct Jatek {
    Elem* fej[2], *vege[2];
    Pontszam psz[2];
    Pont kezd[2];
    Alma alma;
    int jatekosszam;
    int jatekido;
    Irany d[2], del[2];
    SDL_TimerID id;
    Gyoztes gyoztes;
} Jatek;

#include "jatekvezerles.h"


Pont pontMozgat(Pont p, Irany i);
bool kigyoInic(Elem** fej, int hossz, Pont kezd, Irany i);
Elem* kigyoVegetKeres(Elem** const fej);
bool kigyoNyujt(Elem** vege, Pont adat, Irany i);
void kigyoFelszabadit (Elem** fej);
void kigyoLeptet(Elem** const fej, Irany const irany);
Utkozes kigyoUtkozes(Elem** fej1, Elem** fej2, Alma* alma);
void utkozesKezeles(Allapot* a, Jatek* j, Graf* g, SDL_Renderer* renderer);
void almaUj(Alma* alma, Elem** const fej1, Elem** const fej2);

Uint32 idozit(Uint32 ms, void *param);
bool jatekInic(Jatek* j, int jatekosszam);
void billentyuzetKezel(Jatek* j, SDL_Event* event);
#endif // GAMELOGIC_H_INCLUDED
