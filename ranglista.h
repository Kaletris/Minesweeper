//
// Created by Kaletris on 2021. 11. 28..
//
#ifndef AKNAKERESO_RANGLISTA_H
#define AKNAKERESO_RANGLISTA_H

#include "typedefs.h"

void uj_eredmeny_beszuras(Eredmeny **, char *, int);

Eredmeny *eredmenyek_listazasa(char *);

void free_eredmeny_lista(Eredmeny *);

Menu ranglista_kiiras(char *fajlnev);

void eredmenyek_kiirasa_kepernyore(Eredmeny *);

bool eredmeny_beolvasas_megerosites();

void eredmeny_beolvasas(Nehezseg, int);

bool vanbenneamineknemkene(char *);

void eredmenylista_elmentese(Eredmeny *, Nehezseg);

void eredmenyek_kiirasa_fajlba(Eredmeny *eredmeny, FILE *file);

#endif //AKNAKERESO_RANGLISTA_H