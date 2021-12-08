//
// Created by Kaletris on 2021. 11. 28..
//

#include "debugmalloc.h"
#include "menuvezerles.h"
#include "jatek.h"
#include "ranglista.h"
#include "econio.h"
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

Menu jatek_kezdes(int sorszam, int oszlopszam, int aknaszam, Nehezseg nehezseg) {
    Menu visszateres;
    Mezo **tabla = tabla_foglalas(sorszam, oszlopszam);
    tabla_inicializacio(tabla, sorszam, oszlopszam);
    visszateres = tabla_vezerles(tabla, sorszam, oszlopszam, aknaszam, nehezseg);
    tabla_felszabaditas(tabla, sorszam);
    return visszateres;
}

Menu tabla_vezerles(Mezo **tabla, int sorszam, int oszlopszam, int aknaszam, Nehezseg nehezseg) {
    econio_clrscr();
    tabla_rajzolas(tabla, sorszam, oszlopszam);
    Koordinata kurzor;
    gotoxy(&kurzor, sorszam / 2 + 1, oszlopszam / 2 + 1);
    tabla_iranyitas_kiiras(sorszam);

    econio_rawmode();
    bool jatekVege = false;
    bool aknakSzetszorva = false;
    int felfedettMezok = 0;
    int vezerles;
    time_t start_t, end_t;
    int ido;
    time(&start_t);
    while (!jatekVege) {
        kurzor_kiir(kurzor, oszlopszam);
        vezerles = econio_getch();
        switch (vezerles) {
            case KEY_UP:
                if (kurzor.x == 0) break;
                else {
                    gotoxy(&kurzor, kurzor.x, kurzor.y);
                    mezo_rajzolas(tabla, kurzor, false, sorszam, oszlopszam);
                    gotoxy(&kurzor, kurzor.x - 1, kurzor.y);
                    mezo_rajzolas(tabla, kurzor, true, sorszam, oszlopszam);
                    break;
                }
            case KEY_DOWN:
                if (kurzor.x == sorszam - 1) break;
                else {
                    gotoxy(&kurzor, kurzor.x, kurzor.y);
                    mezo_rajzolas(tabla, kurzor, false, sorszam, oszlopszam);
                    gotoxy(&kurzor, kurzor.x + 1, kurzor.y);
                    mezo_rajzolas(tabla, kurzor, true, sorszam, oszlopszam);
                    break;
                }
            case KEY_LEFT:
                if (kurzor.y == 0) break;
                else {
                    gotoxy(&kurzor, kurzor.x, kurzor.y);
                    mezo_rajzolas(tabla, kurzor, false, sorszam, oszlopszam);
                    gotoxy(&kurzor, kurzor.x, kurzor.y - 1);
                    mezo_rajzolas(tabla, kurzor, true, sorszam, oszlopszam);
                    break;
                }
            case KEY_RIGHT:
                if (kurzor.y == oszlopszam - 1) break;
                else {
                    gotoxy(&kurzor, kurzor.x, kurzor.y);
                    mezo_rajzolas(tabla, kurzor, false, sorszam, oszlopszam);
                    gotoxy(&kurzor, kurzor.x, kurzor.y + 1);
                    mezo_rajzolas(tabla, kurzor, true, sorszam, oszlopszam);
                    break;

                }
            case KEY_ESCAPE:
                if (menu_kilepes_megerosites_vezerles(kurzor) == kilepes) {
                    jatekVege = true;
                } else tabla_rajzolas(tabla, sorszam, oszlopszam);
                break;
            case KEY_ENTER:
                if (!tabla[kurzor.x][kurzor.y].zaszlo) {
                    if (!aknakSzetszorva) {
                        tabla_akna_szetszoras(tabla, sorszam, oszlopszam, aknaszam, kurzor);
                        tabla_mezok_szamozasa(tabla, sorszam, oszlopszam);
                        aknakSzetszorva = true;
                        gotoxy(&kurzor, kurzor.x, kurzor.y);
                        mezo_felfedes(tabla, kurzor, true, sorszam, oszlopszam, &felfedettMezok);
                    } else if (tabla[kurzor.x][kurzor.y].akna) {
                        aknak_robbantasa(tabla, sorszam, oszlopszam);
                        time(&end_t);
                        ido = difftime(end_t, start_t);
                        jatekos_vesztett(oszlopszam, ido);
                        jatekVege = true;
                    } else {
                        gotoxy(&kurzor, kurzor.x, kurzor.y);
                        mezo_felfedes(tabla, kurzor, true, sorszam, oszlopszam, &felfedettMezok);
                    }

                }
                break;
            case ' ': //SPACE
                if (tabla[kurzor.x][kurzor.y].lathato) break;
                if (tabla[kurzor.x][kurzor.y].zaszlo) {
                    tabla[kurzor.x][kurzor.y].zaszlo = false;
                } else {
                    tabla[kurzor.x][kurzor.y].zaszlo = true;
                }
                gotoxy(&kurzor, kurzor.x, kurzor.y);
                mezo_rajzolas(tabla, kurzor, true, sorszam, oszlopszam);
                break;
            default:
                break;
        }
        if (felfedettMezok == sorszam * oszlopszam - aknaszam) {
            time(&end_t);
            ido = difftime(end_t, start_t);
            jatekos_nyert(oszlopszam, nehezseg, ido);
            jatekVege = true;
        }
    }
    econio_rawmode();
    econio_normalmode();

    return fomenu;
}

void kurzor_kiir(Koordinata kurzor, int oszlopszam) {
    econio_gotoxy(0, oszlopszam + 2);
    printf("Kurzor x=%d sor", kurzor.x);
    econio_gotoxy(1, oszlopszam + 2);
    printf("Kurzor y=%d oszlop", kurzor.y);
}

void mezo_rajzolas(Mezo **tabla, Koordinata kurzor, bool kijelolve, int sorszam, int oszlopszam) {
    int x = kurzor.x, y = kurzor.y;
    //tablan kivul van a kurzor
    if (x < 0 || y < 0 || x > sorszam - 1 || y > oszlopszam - 1) {
        return;
    }

    //kijelolt mezo szines rajzolasa
    if (kijelolve) {
        econio_textbackground(COL_YELLOW);
        econio_textcolor(COL_BLACK);
    }
    if (tabla[x][y].zaszlo) {
        econio_textcolor(COL_LIGHTRED);
        printf("F");
        econio_textcolor(COL_RESET);
    } else {
        if (!tabla[x][y].lathato) {
            printf("~");
        } else {
            if (tabla[x][y].akna) {
                printf("X");
            } else {
                if (tabla[x][y].kozelbenAknak == 0) {
                    printf(" ");
                } else {
                    printf("%d", tabla[x][y].kozelbenAknak);
                }
            }
        }
    }
    if (kijelolve) {
        econio_textbackground(COL_RESET);
        econio_textcolor(COL_RESET);
    }
}

void mezo_felfedes(Mezo **tabla, Koordinata kurzor, bool kijelolve, int sorszam, int oszlopszam, int *felfedettMezok) {
    int x = kurzor.x, y = kurzor.y;
    //tablan kivul van a kurzor
    if (x < 0 || y < 0 || x > sorszam - 1 || y > oszlopszam - 1) return;
    //lathatova teszi a mezot, ha ures, akkor a mellette levo mezoket is lathatova teszi
    if (!tabla[x][y].zaszlo) {
        if (!tabla[x][y].lathato) {
            tabla[x][y].lathato = true;
            (*felfedettMezok)++;
            econio_gotoxy(x, y);
            mezo_rajzolas(tabla, kurzor, kijelolve, sorszam, oszlopszam);
            if (tabla[x][y].kozelbenAknak == 0) {
                kornyezo_mezo_megjelenites(tabla, kurzor, sorszam, oszlopszam, felfedettMezok);
            }
        }
    }
}

void kornyezo_mezo_megjelenites(Mezo **tabla, Koordinata kurzor, int sorszam, int oszlopszam, int *felfedettMezok) {
    int x = kurzor.x, y = kurzor.y;
    //tablan kivul van a kurzor
    if (x < 0 || y < 0 || x > sorszam - 1 || y > oszlopszam - 1) return;

    //esetszetbontva mezo felfedes

    //bal felso sarok
    if (x == 0 && y == 0) {
        if (!tabla[0][1].lathato) {
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y -= 1;
        }
        if (!tabla[1][1].lathato) {
            kurzor.x += 1;
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
            kurzor.y -= 1;
        }
        if (!tabla[1][0].lathato) {
            kurzor.x += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
        }
    }
    //jobb felso sarok
    if (x == 0 && y == oszlopszam - 1) {
        if (!tabla[0][y - 1].lathato) {
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y += 1;
        }
        if (!tabla[1][y - 1].lathato) {
            kurzor.x += 1;
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
            kurzor.y += 1;
        }
        if (!tabla[1][y].lathato) {
            kurzor.x += 1;
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
            kurzor.y -= 1;
        }
    }
    //bal also sarok
    if (x == sorszam - 1 && y == 0) {
        if (!tabla[x - 1][0].lathato) {
            kurzor.x -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
        }
        if (!tabla[x - 1][1].lathato) {
            kurzor.x -= 1;
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
            kurzor.y -= 1;
        }
        if (!tabla[x][1].lathato) {
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y -= 1;
        }
    }
    //jobb also sarok
    if (x == sorszam - 1 && y == oszlopszam - 1) {
        if (!tabla[x - 1][y].lathato) {
            kurzor.x -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
        }
        if (!tabla[x - 1][y - 1].lathato) {
            kurzor.x -= 1;
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
            kurzor.y += 1;
        }
        if (!tabla[x][y - 1].lathato) {
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y += 1;
        }
    }
    //fent
    if (x == 0 && y != 0 && y != oszlopszam - 1) {
        if (!tabla[0][y - 1].lathato) {
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y += 1;
        }
        if (!tabla[0][y + 1].lathato) {
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y -= 1;
        }
        if (!tabla[1][y - 1].lathato) {
            kurzor.x += 1;
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
            kurzor.y += 1;
        }
        if (!tabla[1][y].lathato) {
            kurzor.x += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
        }
        if (!tabla[1][y + 1].lathato) {
            kurzor.x += 1;
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
            kurzor.y -= 1;
        }
    }
    //lent
    if (x == sorszam - 1 && y != 0 && y != oszlopszam - 1) {
        if (!tabla[x][y - 1].lathato) {
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y += 1;
        }
        if (!tabla[x][y + 1].lathato) {
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y -= 1;
        }
        if (!tabla[x - 1][y - 1].lathato) {
            kurzor.x -= 1;
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
            kurzor.y += 1;
        }
        if (!tabla[x - 1][y].lathato) {
            kurzor.x -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
        }
        if (!tabla[x - 1][y + 1].lathato) {
            kurzor.x -= 1;
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
            kurzor.y -= 1;
        }
    }
    //baloldalt
    if (x != 0 && x != sorszam - 1 && y == 0) {
        if (!tabla[x - 1][y].lathato) {
            kurzor.x -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
        }
        if (!tabla[x + 1][y].lathato) {
            kurzor.x += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
        }
        if (!tabla[x - 1][y + 1].lathato) {
            kurzor.x -= 1;
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
            kurzor.y -= 1;
        }
        if (!tabla[x][y + 1].lathato) {
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y -= 1;
        }
        if (!tabla[x + 1][y + 1].lathato) {
            kurzor.x += 1;
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
            kurzor.y -= 1;
        }
    }
    //jobboldalt
    if (x != 0 && x != sorszam - 1 && y == oszlopszam - 1) {
        if (!tabla[x - 1][y].lathato) {
            kurzor.x -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
        }
        if (!tabla[x + 1][y].lathato) {
            kurzor.x += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
        }
        if (!tabla[x - 1][y - 1].lathato) {
            kurzor.x -= 1;
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
            kurzor.y += 1;
        }
        if (!tabla[x][y - 1].lathato) {
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y += 1;
        }
        if (!tabla[x + 1][y - 1].lathato) {
            kurzor.x += 1;
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
            kurzor.y += 1;
        }
    }
    //belso
    if (x != 0 && x != sorszam - 1 && y != 0 && y != oszlopszam - 1) {
        if (!tabla[x - 1][y - 1].lathato) {
            kurzor.x -= 1;
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
            kurzor.y += 1;
        }
        if (!tabla[x - 1][y].lathato) {
            kurzor.x -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
        }
        if (!tabla[x - 1][y + 1].lathato) {
            kurzor.x -= 1;
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x += 1;
            kurzor.y -= 1;
        }
        if (!tabla[x][y - 1].lathato) {
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y += 1;
        }
        if (!tabla[x][y + 1].lathato) {
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.y -= 1;
        }
        if (!tabla[x + 1][y - 1].lathato) {
            kurzor.x += 1;
            kurzor.y -= 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
            kurzor.y += 1;
        }
        if (!tabla[x + 1][y].lathato) {
            kurzor.x += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
        }
        if (!tabla[x + 1][y + 1].lathato) {
            kurzor.x += 1;
            kurzor.y += 1;
            mezo_felfedes(tabla, kurzor,
                          false, sorszam, oszlopszam, felfedettMezok);
            kurzor.x -= 1;
            kurzor.y -= 1;
        }
    }
}

void tabla_rajzolas(Mezo **tabla, int sorszam, int oszlopszam) {
    econio_clrscr();
    for (int i = 0; i < sorszam; ++i) {
        for (int j = 0; j < oszlopszam; ++j) {
            if (tabla[i][j].lathato) {
                if (tabla[i][j].akna) {
                    printf("X");
                } else if (tabla[i][j].kozelbenAknak == 0) printf(" ");
                else printf("%d", tabla[i][j].kozelbenAknak);
            } else printf("~");
        }
        printf("\n");
    }
}

int aknae(Mezo mezo) {
    if (mezo.akna) return 1;
    else return 0;
}

void aknak_robbantasa(Mezo **tabla, int sorszam, int oszlopszam) {
    for (int x = 0; x < sorszam; ++x) {
        for (int y = 0; y < oszlopszam; ++y) {
            if (tabla[x][y].akna) {
                econio_gotoxy(x, y);
                econio_textbackground(COL_RED);
                printf("X");
                econio_textbackground(COL_RESET);
            }
        }
    }
}

void tabla_mezok_szamozasa(Mezo **tabla, int sorszam, int oszlopszam) {
    //inicializálás
    for (int i = 0; i < sorszam; i++) {
        for (int j = 0; j < oszlopszam; j++) {
            tabla[i][j].kozelbenAknak = 0;
        }
    }

    //sarkok számozása
    //bal fent
    tabla[0][0].kozelbenAknak = aknae(tabla[0][1]) + aknae(tabla[1][0]) + aknae(tabla[1][1]);
    //jobb fent
    tabla[0][oszlopszam - 1].kozelbenAknak =
            aknae(tabla[0][oszlopszam - 1 - 1]) + aknae(tabla[1][oszlopszam - 1]) + aknae(tabla[1][oszlopszam - 1 - 1]);
    //bal lent
    tabla[sorszam - 1][0].kozelbenAknak =
            aknae(tabla[sorszam - 1][1]) + aknae(tabla[sorszam - 1 - 1][0]) + aknae(tabla[sorszam - 1 - 1][1]);
    //jobb lent
    tabla[sorszam - 1][oszlopszam - 1].kozelbenAknak =
            aknae(tabla[sorszam - 1][oszlopszam - 1 - 1]) + aknae(tabla[sorszam - 1 - 1][oszlopszam - 1]) +
            aknae(tabla[sorszam - 1 - 1][oszlopszam - 1 - 1]);

    //szelso oszlopok/sorok szamozasa
    //baloldalt
    for (int i = 1; i < oszlopszam - 1; ++i) {
        tabla[0][i].kozelbenAknak = aknae(tabla[0][i - 1]) + aknae(tabla[0][i + 1])
                                    + aknae(tabla[1][i - 1]) + aknae(tabla[1][i]) + aknae(tabla[1][i + 1]);
    }
    //jobboldalt
    for (int i = 1; i < oszlopszam - 1; ++i) {
        tabla[sorszam - 1][i].kozelbenAknak =
                aknae(tabla[sorszam - 1][i - 1]) + aknae(tabla[sorszam - 1][i + 1])
                + aknae(tabla[sorszam - 1 - 1][i - 1]) + aknae(tabla[sorszam - 1 - 1][i]) +
                aknae(tabla[sorszam - 1 - 1][i + 1]);
    }
    //fent
    for (int i = 1; i < sorszam - 1; ++i) {
        tabla[i][0].kozelbenAknak = aknae(tabla[i - 1][0]) + aknae(tabla[i + 1][0])
                                    + aknae(tabla[i - 1][1]) + aknae(tabla[i][1]) + aknae(tabla[i + 1][1]);
    }
    //lent
    for (int i = 1; i < sorszam - 1; ++i) {
        tabla[i][oszlopszam - 1].kozelbenAknak =
                aknae(tabla[i - 1][oszlopszam - 1]) + aknae(tabla[i + 1][oszlopszam - 1])
                + aknae(tabla[i - 1][oszlopszam - 1 - 1]) +
                aknae(tabla[i][oszlopszam - 1 - 1]) + aknae(tabla[i + 1][oszlopszam - 1 - 1]);
    }
    //nem szelso mezok szamozasa
    for (int i = 1; i < sorszam - 1; i++) {
        for (int j = 1; j < oszlopszam - 1; j++) {
            tabla[i][j].kozelbenAknak = aknae(tabla[i - 1][j - 1]) + aknae(tabla[i][j - 1]) + aknae(tabla[i + 1][j - 1])
                                        + aknae(tabla[i - 1][j]) + aknae(tabla[i + 1][j])
                                        + aknae(tabla[i - 1][j + 1]) + aknae(tabla[i][j + 1]) +
                                        aknae(tabla[i + 1][j + 1]);
        }
    }
}

Mezo **tabla_foglalas(int sorszam, int oszlopszam) {
    Mezo **tabla = (Mezo **) malloc(sizeof(Mezo *) * sorszam);
    for (int i = 0; i < sorszam; i++) {
        tabla[i] = (Mezo *) malloc(sizeof(Mezo) * oszlopszam);
    }
    return tabla;
}

void tabla_felszabaditas(Mezo **tabla, int sorszam) {
    for (int i = 0; i < sorszam; i++) {
        free(tabla[i]);
    }
    free(tabla);
}

void tabla_akna_szetszoras(Mezo **tabla, int sorszam, int oszlopszam, int aknaszam, Koordinata kurzor) {
    int x = kurzor.x, y = kurzor.y;
    //inicializalas
    for (int i = 0; i < sorszam; i++) {
        for (int j = 0; j < oszlopszam; j++) {
            tabla[i][j].akna = false;
            tabla[i][j].lathato = false;
        }
    }

    //aknak letrehozasa
    int aknak[aknaszam];
    for (int i = 0, tmp; i < aknaszam; i++) {
        bool letezik;
        do {
            letezik = false;
            tmp = rand() % (sorszam * oszlopszam);
            if (tmp == x * oszlopszam + y) {
                letezik = true;
            } else {
                for (int j = 0; j < i; j++) {
                    if (tmp == aknak[j]) {
                        letezik = true;
                        break;
                    }
                }
            }
        } while (letezik);
        aknak[i] = tmp;
    }

    //aknak szetszorasa
    for (int i = 0; i < aknaszam; i++) {
        tabla[aknak[i] / oszlopszam][aknak[i] % oszlopszam].akna = true;
    }
}

void tabla_inicializacio(Mezo **tabla, int sorszam, int oszlopszam) {
    for (int i = 0; i < sorszam; ++i) {
        for (int j = 0; j < oszlopszam; ++j) {
            tabla[i][j].zaszlo = false;
            tabla[i][j].akna = false;
            tabla[i][j].lathato = false;
        }
    }
}

void tabla_iranyitas_kiiras(int sorszam) {
    econio_gotoxy(sorszam + 2, 0);
    printf("A navigálás nyilakkal történik:\n"
           "Fel nyíl: Felfele lép\n"
           "Lefele nyíl: Lefele lép\n"
           "Balra nyíl: Balra lép\n"
           "Jobbra nyíl: Jobbra lép\n"
           "Enter gomb: Felfedi a mezõt, ha nincs rajta zászló\n"
           "Esc: Kilépõ menüt felhozza\n"
           "SPACE: Zászlót tesz le/szed fel");
}

Menu jatekos_nyert(int oszlopszam, Nehezseg nehezseg, int ido) {
    econio_gotoxy(4, oszlopszam + 2);
    printf("Gratulálok, nyertük. Felfedtél minden biztonságos mezõt, ez mindössze %d perc %d másodpercbe került!",
           ido / 60, ido % 60);
    econio_gotoxy(5, oszlopszam + 2);
    printf("Mostmár tudjuk hol helyezkednek el az aknák, a többit a tûzszerészek elintézik.");
    econio_gotoxy(7, oszlopszam + 2);
    printf("A helyszín elhagyásához nyomd meg az ESC billentyût.");

    econio_rawmode();
    Menu vezerles = KEY_HOME;
    while (vezerles != KEY_ESCAPE) {
        vezerles = econio_getch();
    }

    eredmeny_beolvasas(nehezseg, ido);
    econio_normalmode();
    return fomenu;
}

Menu jatekos_vesztett(int oszlopszam, int ido) {
    econio_gotoxy(4, oszlopszam + 2);
    printf("Ráléptél egy aknára! Ez láncreakcióban felrobbantotta az összes többit!");
    econio_gotoxy(5, oszlopszam + 2);
    printf("Micsoda tragédia! %d perc %d másodpercig kerulted el a sorsodat.", ido / 60, ido % 60);
    econio_gotoxy(7, oszlopszam + 2);
    printf("Veszteséges elfogadásához nyomd meg az ESC billentyût.");
    //econio_rawmode();
    Menu vezerles = KEY_HOME;
    while (vezerles != KEY_ESCAPE) {
        vezerles = econio_getch();
    }
    //econio_normalmode();
    return fomenu;
}