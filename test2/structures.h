#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct _Personne {
    int depart;
    int arrivee;
} Personne;

typedef struct _ListeDePersonnes {
    Personne *tete;
    struct _ListeDePersonnes *queue;
} ListeDePersonnes;

typedef struct _Ascenseur {
    int capacite;
    int etageActuel;
    int destination;
    ListeDePersonnes *transportes;
} Ascenseur;

typedef struct _Immeuble {
    int nbredEtages;
    Ascenseur *ascenseur;
    ListeDePersonnes **enAttente;
} Immeuble;

Personne* createPerson(int dep, int dest);
ListeDePersonnes* insertPersonList(Personne *personne, ListeDePersonnes *liste);
Ascenseur* createElevator(int capacity, int actualFloor,ListeDePersonnes *personsIn);


#endif
