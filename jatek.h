//
// Created by Kaletris on 2021. 11. 28..
//
#ifndef AKNAKERESO_JATEK_H
#define AKNAKERESO_JATEK_H

Menu jatek_kezdes(int, int, int, Nehezseg);

Mezo **tabla_foglalas(int, int);

void tabla_felszabaditas(Mezo **, int);

void tabla_inicializacio(Mezo **, int, int);

Menu tabla_vezerles(Mezo **tabla, int sorszam, int oszlopszam, int aknaszam, Nehezseg nehezseg);

void tabla_akna_szetszoras(Mezo **, int, int, int, Koordinata);

void tabla_mezok_szamozasa(Mezo **, int, int);

int aknae(Mezo);

void tabla_rajzolas(Mezo **, int, int);

void kurzor_kiir(Koordinata, int);

void mezo_rajzolas(Mezo **, Koordinata, bool, int, int);

void mezo_felfedes(Mezo **, Koordinata, bool, int, int, int *);

void kornyezo_mezo_megjelenites(Mezo **, Koordinata, int, int, int *);

void aknak_robbantasa(Mezo **, int, int);

void tabla_iranyitas_kiiras(int);

Menu jatekos_nyert(int, Nehezseg, int);

Menu jatekos_vesztett(int, int);

#endif //AKNAKERESO_JATEK_H