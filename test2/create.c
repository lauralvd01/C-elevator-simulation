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

ListeDeListes* createWaiting(void){
    ListeDeListes *enAttente = malloc(sizeof(ListeDeListes));
    enAttente -> etage0 = insertPersonList(createPerson(0,2),NULL);
    enAttente -> etage1 = insertPersonList(createPerson(1,2),NULL);
    enAttente -> etage2 = insertPersonList(NULL,NULL);
    enAttente -> etage3 = insertPersonList(createPerson(3,0),NULL);
    enAttente -> etage4 = insertPersonList(createPerson(4,3),NULL);
    return enAttente;
}

Immeuble* creataBuilding(int nbFloor, Ascenseur *elevator, ListeDeListes *waiting) {
    Immeuble *immeuble = malloc(sizeof(Immeuble));
    immeuble -> nbredEtages = nbFloor;
    immeuble -> ascenseur = elevator;
    immeuble -> enAttente = waiting;
    return immeuble;
}
