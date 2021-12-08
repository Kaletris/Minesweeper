//
// Created by Kaletris on 2021. 11. 24..
//

#include "debugmalloc.h"
#include "menuvezerles.h"
#include "ranglista.h"
#include "jatek.h"

#include "econio.h"
#include <stdio.h>

void gotoxy(Koordinata *kurzor, int x, int y) {
    econio_gotoxy(x, y);
    kurzor->x = x;
    kurzor->y = y;
}

void menu_navigacio_kiiras() {
    printf("\nA navig�l�s nyilakkal t�rt�nik:\n"
           "Fel ny�l: Felfele l�p (legfel�l nem csin�l semmit)\n"
           "Lefele ny�l: Lefele l�p (legalul nem csin�l semmit)\n"
           "Enter gomb: Bel�p egy men�pontba\n"
           "Esc: F�men�ben egy meger�s�t�s ut�n kil�p a programb�l.\n"
           "Esc: Almen�ben lenyomva visszal�p f�men�be.\n");
}

Menu menu_fomenu_vezerles(Koordinata kurzor) {
    kurzor.y = 1; //ebben az oszlopban van jelezve a kiv�lasztott men�pont;
    menu_fomenu_kiiras();
    econio_rawmode();
    int vezerles = KEY_HOME;
    while (vezerles != KEY_ENTER && vezerles != KEY_ESCAPE) {
        vezerles = econio_getch();
        switch (vezerles) {
            case KEY_UP:
                switch (kurzor.x) {
                    case 1:
                        break;
                    case 2:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 1, kurzor.y);
                        printf("X");
                        break;
                    default:
                        gotoxy(&kurzor, 2, kurzor.y);
                        printf("X");
                        break;
                }
                break;
            case KEY_DOWN:
                switch (kurzor.x) {
                    case 1:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 2, kurzor.y);
                        printf("X");
                        break;
                    case 2:
                        break;
                    default:
                        gotoxy(&kurzor, 1, kurzor.y);
                        printf("X");
                        break;
                }
                break;
            default:
                break;
        }
    }
    econio_normalmode();
    switch (vezerles) {
        case KEY_ENTER:
            switch (kurzor.x) {
                case 1:
                    return jatekvalasztomenu;
                case 2:
                    return ranglistamenu;
                default:
                    return fomenu;
            }
        case KEY_ESCAPE:
            return kilepomenu;
        default:
            break;
    }
}

void menu_fomenu_kiiras() {
    econio_clrscr();
    printf("F�men�\n");
    printf("[ ]�j j�t�k\n");
    printf("[ ]Ranglista\n");
    menu_navigacio_kiiras();
}

Menu menu_kilepes_megerosites_vezerles(Koordinata kurzor) {
    menu_kilepes_megerosites_kiiras();
    econio_rawmode();
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
    switch (kurzor.y) {
        case 1:
            return kilepes;
        case 11:
            return fomenu;
        default:
            return kilepomenu;
    }
}

void menu_kilepes_megerosites_kiiras() {
    econio_clrscr();
    printf("Biztosan ki szeretn�l l�pni?\n");
    printf("[ ]Igen / [ ]Nem\n");
    printf("\nA navig�l�s nyilakkal t�rt�nik:\n"
           "Balra ny�l: Igen\n"
           "Jobbra ny�l: Nem\n"
           "Enter gomb: Meger�s�t�s\n");
}

Menu menu_ranglista_vezerles(Koordinata kurzor) {
    kurzor.y = 1;
    menu_ranglista_kiiras();
    econio_rawmode();
    int vezerles = KEY_HOME;
    while (vezerles != KEY_ENTER && vezerles != KEY_ESCAPE) {
        vezerles = econio_getch();
        switch (vezerles) {
            case KEY_UP:
                switch (kurzor.x) {
                    case 1:
                        break;
                    case 2:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 1, kurzor.y);
                        printf("X");
                        break;
                    case 3:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 2, kurzor.y);
                        printf("X");
                        break;
                    default:
                        gotoxy(&kurzor, 3, kurzor.y);
                        printf("X");
                        break;
                }
                break;
            case KEY_DOWN:
                switch (kurzor.x) {
                    case 1:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 2, kurzor.y);
                        printf("X");
                        break;
                    case 2:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 3, kurzor.y);
                        printf("X");
                        break;
                    case 3:
                        break;
                    default:
                        gotoxy(&kurzor, 1, kurzor.y);
                        printf("X");
                        break;
                }
                break;
            default:
                break;
        }
    }
    econio_normalmode();
    switch (vezerles) {
        case KEY_ENTER:
            switch (kurzor.x) {
                case 1:
                    return konnyuRanglista;
                case 2:
                    return kozepesRanglista;
                case 3:
                    return nehezRanglista;
                default:
                    return ranglistamenu;
            }
        case KEY_ESCAPE:
            return fomenu;
        default:
            return ranglistamenu;
    }
}

void menu_uj_jatek_menu_kiiras() {
    econio_clrscr();
    printf("�j J�t�k:\n");
    printf("[ ]K�nny�\n");
    printf("[ ]K�zepes\n");
    printf("[ ]Neh�z\n");
    menu_navigacio_kiiras();
}

Menu menu_uj_jatek_menu_vezerles(Koordinata kurzor) {
    kurzor.y = 1;
    menu_uj_jatek_menu_kiiras();
    econio_rawmode();
    int vezerles = KEY_HOME;
    while (vezerles != KEY_ENTER && vezerles != KEY_ESCAPE) {
        vezerles = econio_getch();
        switch (vezerles) {
            case KEY_UP:
                switch (kurzor.x) {
                    case 1:
                        break;
                    case 2:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 1, kurzor.y);
                        printf("X");
                        break;
                    case 3:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 2, kurzor.y);
                        printf("X");
                        break;
                    default:
                        gotoxy(&kurzor, 3, kurzor.y);
                        printf("X");
                        break;
                }
                break;
            case KEY_DOWN:
                switch (kurzor.x) {
                    case 1:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 2, kurzor.y);
                        printf("X");
                        break;
                    case 2:
                        econio_gotoxy(kurzor.x, kurzor.y);
                        printf(" ");
                        gotoxy(&kurzor, 3, kurzor.y);
                        printf("X");
                        break;
                    case 3:
                        break;
                    default:
                        gotoxy(&kurzor, 1, kurzor.y);
                        printf("X");
                        break;
                }
                break;
            default:
                break;
        }
    }
    econio_normalmode();
    switch (vezerles) {
        case KEY_ENTER:
            switch (kurzor.x) {
                case 1:
                    return konnyuJatek;
                case 2:
                    return kozepesJatek;
                case 3:
                    return nehezJatek;
                default:
                    return jatekvalasztomenu;
            }
        case KEY_ESCAPE:
            return fomenu;
        default:
            break;
    }
}

void menu_ranglista_kiiras() {
    econio_clrscr();
    printf("Ranglist�k:\n");
    printf("[ ]K�nny�\n");
    printf("[ ]K�zepes\n");
    printf("[ ]Neh�z\n");
    menu_navigacio_kiiras();
}

