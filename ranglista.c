//
// Created by Kaletris on 2021. 11. 28..
//

#include "debugmalloc.h"
#include "ranglista.h"
#include "menuvezerles.h"
#include "econio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void uj_eredmeny_beszuras(Eredmeny **eredmeny, char *nev, int ido) {
    if (*eredmeny == NULL) {
        Eredmeny *ujEredmeny = (Eredmeny *) malloc(sizeof(Eredmeny));
        ujEredmeny->nev = nev;
        ujEredmeny->ido = ido;
        ujEredmeny->kov = NULL;
        *eredmeny = ujEredmeny;
    } else {
        if (ido < (*eredmeny)->ido) {
            Eredmeny *hatrebb = (Eredmeny *) malloc(sizeof(Eredmeny));
            hatrebb->nev = (*eredmeny)->nev;
            hatrebb->ido = (*eredmeny)->ido;
            hatrebb->kov = (*eredmeny)->kov;
            (*eredmeny)->nev = nev;
            (*eredmeny)->ido = ido;
            (*eredmeny)->kov = hatrebb;
        } else {
            uj_eredmeny_beszuras(&(*eredmeny)->kov, nev, ido);
        }
    }
}

Eredmeny *eredmenyek_listazasa(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        //perror("Nem sikerult listazni az eredmenyeket.\n");
        return NULL;
    }
    Eredmeny *eredmeny = NULL;
    char line[51];
    while (fgets(line, sizeof(line), file)) {
        char *nev = (char *) malloc(sizeof(char) * 21);
        int ido;
        int i = 0;
        while (i < 21 && line[i] != ';') {
            nev[i] = line[i];
            i++;
        }
        nev[i] = '\0';
        ido = atoi(line + i + 1);

        uj_eredmeny_beszuras(&eredmeny, nev, ido);
    }
    fclose(file);

    return eredmeny;
}

void free_eredmeny_lista(Eredmeny *eredmeny) {
    if (eredmeny != NULL) {
        free_eredmeny_lista(eredmeny->kov);
        free(eredmeny->nev);
        free(eredmeny);
    }
}

Menu ranglista_kiiras(char *fajlnev) {
    econio_clrscr();
    Eredmeny *eredmenyek;
    eredmenyek = eredmenyek_listazasa(fajlnev);
    eredmenyek_kiirasa_kepernyore(eredmenyek);
    free_eredmeny_lista(eredmenyek);
    printf("\nNyomj meg egy billentyût a visszatéréshez\n");
    econio_getch();
    return fomenu;
}

void eredmenyek_kiirasa_kepernyore(Eredmeny *eredmeny) {
    if (eredmeny != NULL) {
        printf("%s: %d perc %d masodperc\n", eredmeny->nev, eredmeny->ido / 60, eredmeny->ido % 60);
        eredmenyek_kiirasa_kepernyore(eredmeny->kov);
    }
}

bool eredmeny_beolvasas_megerosites() {
    econio_clrscr();
    printf("Szeretnéd elmenteni az eredményed?\n");
    printf("[ ]Igen / [ ]Nem\n");
    printf("\nA navigálás nyilakkal történik:\n"
           "Balra nyíl: Igen\n"
           "Jobbra nyíl: Nem\n"
           "Enter gomb: Megerõsítés\n");
    econio_rawmode();
    Koordinata kurzor;
    kurzor.x = -1;
    kurzor.y = -1;
    int vezerles = KEY_HOME;
    while (vezerles != KEY_ENTER) {
        vezerles = econio_getch();
        switch (vezerles) {
            case KEY_LEFT:
                gotoxy(&kurzor, 1, 11);
                printf(" ");
                gotoxy(&kurzor, 1, 1);
                printf("X");
                break;
            case KEY_RIGHT:
                gotoxy(&kurzor, 1, 1);
                printf(" ");
                gotoxy(&kurzor, 1, 11);
                printf("X");
                break;
            default:
                break;
        }
    }
    if (kurzor.y == 1)return true;
    else return false;
}

void eredmeny_beolvasas(Nehezseg nehezseg, int ido) {
    if (!eredmeny_beolvasas_megerosites()) return;
    econio_clrscr();
    econio_normalmode();
    printf("Kérlek írd be a nevet amivel híressé szeretnél válni!\n");
    printf("Sajnos nem lehet 20 karakternél hosszabb vagy számokat tartalmazó, mert azt nehéz lenne megjegyeznie az embereknek\n");
    printf("Ha beírtad a neved egy ENTER lenyomásával jelezd!\n");
    char *nev = (char *) malloc(sizeof(char) * 21);
    char c;
    do {
        fflush(stdin);
        fgets(nev, 20, stdin);
    } while (vanbenneamineknemkene(nev));

    Eredmeny *eredmenyek;
    switch (nehezseg) {
        case konnyu:
            eredmenyek = eredmenyek_listazasa(konnyuLista);
            uj_eredmeny_beszuras(&eredmenyek, nev, ido);
            break;
        case kozepes:
            eredmenyek = eredmenyek_listazasa(kozepesLista);
            uj_eredmeny_beszuras(&eredmenyek, nev, ido);
            break;
        case nehez:
            eredmenyek = eredmenyek_listazasa(nehezLista);
            uj_eredmeny_beszuras(&eredmenyek, nev, ido);
            break;
        default:
            perror("Eredmeny felvetele sikertelen!");
            break;
    }

    eredmenylista_elmentese(eredmenyek, nehezseg);
    printf("Eredménylista felszabadítása\n");
    free_eredmeny_lista(eredmenyek);
    printf("Eredménylista felszabadítva\n");
    econio_rawmode();
}

bool vanbenneamineknemkene(char *nev) {
    for (int i = 0; i < 21 && nev[i] != '\n' && nev[i] != '\0'; ++i) {
        if ('9' >= nev[i] && nev[i] >= '0' || nev[i] == ';') {
            printf("Hibás bemenet!\n", nev);
            return true;
        }
    }
    for (int i = 0; i < 21; ++i) {
        if (nev[i] == '\n') {
            nev[i] = '\0';
        }
    }
    return false;
}

void eredmenylista_elmentese(Eredmeny *eredmeny, Nehezseg nehezseg) {
    FILE *file;
    switch (nehezseg) {
        case konnyu:
            file = fopen(konnyuLista, "w");
            eredmenyek_kiirasa_fajlba(eredmeny, file);
            fclose(file);
            break;
        case kozepes:
            file = fopen(kozepesLista, "w");
            eredmenyek_kiirasa_fajlba(eredmeny, file);
            fclose(file);
            break;
        case nehez:
            file = fopen(nehezLista, "w");
            eredmenyek_kiirasa_fajlba(eredmeny, file);
            fclose(file);
            break;
        default:
            perror("Eredmeny elmentese sikertelen!");
            break;
    }
}

void eredmenyek_kiirasa_fajlba(Eredmeny *eredmeny, FILE *file) {
    if (eredmeny != NULL) {
        fprintf(file, "%s;%d\n", eredmeny->nev, eredmeny->ido);
        eredmenyek_kiirasa_fajlba(eredmeny->kov, file);
    }
}