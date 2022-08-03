#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct _Personne {
    int depart;
    int arrivee;
} Personne;

typedef struct _ListeDePersonnes {
    int longueur;
    Personne *tete;
    struct _ListeDePersonnes *queue;
} ListeDePersonnes;

typedef struct _Ascenseur {
    int capacite;
    int etageActuel;
    int destination;
    ListeDePersonnes *transportes;
} Ascenseur;

typedef struct _ListeDeListes {
    ListeDePersonnes *etage0;
    ListeDePersonnes *etage1;
    ListeDePersonnes *etage2;
    ListeDePersonnes *etage3;
    ListeDePersonnes *etage4;
} ListeDeListes;

typedef struct _Immeuble {
    int nbredEtages;
    Ascenseur *ascenseur;
    ListeDeListes *enAttente;
} Immeuble;


// Fonctions de création
Personne* createPerson(int dep, int dest);
ListeDePersonnes* insertPersonList(Personne *personne, ListeDePersonnes *liste);
Ascenseur* createElevator(int capacity, int actualFloor,ListeDePersonnes *personsIn);
ListeDeListes* createWaiting(void);
Immeuble* creataBuilding(int nbFloor, Ascenseur *elevator, ListeDeListes *waiting);

// Fonctions d'affichage
void printPerson(Personne* p);
void printPersonList(ListeDePersonnes* lst);
void printBuilding(Immeuble *building);

#endif
