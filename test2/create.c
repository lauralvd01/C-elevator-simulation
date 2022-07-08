#include "structures.h"
#include<stdlib.h>

Personne* createPerson(int dep, int dest) {
    Personne *p = malloc(sizeof(Personne));
    p -> depart = dep;
    p -> arrivee = dest;
    return p;
}

ListeDePersonnes* insertPersonList(Personne *personne, ListeDePersonnes *liste) {
    ListeDePersonnes *newliste = malloc(sizeof(ListeDePersonnes));
    newliste -> tete = personne;
    newliste -> queue = liste;
    return newliste;
}

Ascenseur* createElevator(int capacity, int actualFloor,ListeDePersonnes *personsIn) {
    Ascenseur *ascenseur = malloc(sizeof(Ascenseur));
    ascenseur -> capacite = capacity;
    ascenseur -> etageActuel = actualFloor;
    ascenseur -> transportes = personsIn;
    return ascenseur;
}