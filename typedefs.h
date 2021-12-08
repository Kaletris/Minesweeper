//
// Created by Kaletris on 2021. 11. 28..
//

#ifndef AKNAKERESO_TYPEDEFS_H
#define AKNAKERESO_TYPEDEFS_H

#include <stdbool.h>

#define konnyuLista "dicsoseglistaKonnyu.txt"
#define kozepesLista "dicsoseglistaKozepes.txt"
#define nehezLista "dicsoseglistaNehez.txt"

typedef enum Menu {
    fomenu,
    kilepomenu,
    jatekvalasztomenu,
    ranglistamenu,
    kilepes,
    konnyuJatek,
    kozepesJatek,
    nehezJatek,
    konnyuRanglista,
    kozepesRanglista,
    nehezRanglista
} Menu;

//x koordinata a sort, y az oszlopot jeloli
typedef struct Koordinata {
    int x;
    int y;
} Koordinata;

typedef enum Nehezseg {
    konnyu,
    kozepes,
    nehez
} Nehezseg;

typedef struct Eredmeny {
    char *nev;
    int ido;
    struct Eredmeny *kov;
} Eredmeny;

typedef struct Mezo {
    bool akna;
    int kozelbenAknak;
    bool lathato;
    bool zaszlo;
} Mezo;

#endif //AKNAKERESO_TYPEDEFS_H
