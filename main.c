//Papp Krisztián EAYMQL Aknakereso

#include <stdio.h>
#include <stdlib.h>
#include "econio.h"
#include <time.h>
#include "debugmalloc.h"
#include "menuvezerles.h"
#include "jatek.h"
#include "ranglista.h"
#include "typedefs.h"

#ifdef _WIN32

#include <windows.h>

#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);
#endif
    srand(time(0));
    econio_set_title("Aknakeresõ");

    Koordinata kurzor;
    kurzor.x = -1;
    kurzor.y = -1;
    Menu menu = fomenu;
    while (menu != kilepes) {
        econio_rawmode();
        switch (menu) {
            case fomenu:
                menu = menu_fomenu_vezerles(kurzor);
                break;
            case kilepomenu:
                menu = menu_kilepes_megerosites_vezerles(kurzor);
                break;
            case jatekvalasztomenu:
                menu = menu_uj_jatek_menu_vezerles(kurzor);
                break;
            case konnyuJatek:
                menu = jatek_kezdes(8, 10, 10, konnyu);
                break;
            case kozepesJatek:
                menu = jatek_kezdes(18, 18, 40, kozepes);
                break;
            case nehezJatek:
                menu = jatek_kezdes(20, 24, 99, nehez);
                break;
            case ranglistamenu:
                menu = menu_ranglista_vezerles(kurzor);
                break;
            case konnyuRanglista:
                menu = ranglista_kiiras(konnyuLista);
                break;
            case kozepesRanglista:
                menu = ranglista_kiiras(kozepesLista);
                break;
            case nehezRanglista:
                menu = ranglista_kiiras(nehezLista);
                break;
            default:
                perror("Menüvezérlés probléma");
                break;
        }
    }

    econio_clrscr();
    return 0;
}