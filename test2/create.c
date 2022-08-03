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
    if(personne == NULL & liste == NULL){
        newliste->longueur = 0;
    }
    else{
        newliste->longueur = liste->longueur+1;
    }
    return newliste;
}

Ascenseur* createElevator(int capacity, int actualFloor,ListeDePersonnes *personsIn) {
    Ascenseur *ascenseur = malloc(sizeof(Ascenseur));
    ascenseur -> capacite = capacity;
    ascenseur -> etageActuel = actualFloor;
    ascenseur -> destination = actualFloor;
    if(personsIn->longueur <= capacity){
        ascenseur -> transportes = personsIn;
    }
    return ascenseur;
}

ListeDeListes* createWaiting(void){
    ListeDeListes *enAttente = malloc(sizeof(ListeDeListes));
    enAttente -> etage0 = insertPersonList(createPerson(0,2),insertPersonList(NULL,NULL));
    enAttente -> etage1 = insertPersonList(createPerson(1,0),insertPersonList(createPerson(1,2),insertPersonList(NULL,NULL)));
    enAttente -> etage2 = insertPersonList(NULL,NULL);
    enAttente -> etage3 = insertPersonList(createPerson(3,0),insertPersonList(NULL,NULL));
    enAttente -> etage4 = insertPersonList(createPerson(4,3),insertPersonList(NULL,NULL));
    return enAttente;
}

Immeuble* createBuilding(int nbFloor, Ascenseur *elevator, ListeDeListes *waiting) {
    Immeuble *immeuble = malloc(sizeof(Immeuble));
    immeuble -> nbredEtages = nbFloor;
    immeuble -> ascenseur = elevator;
    immeuble -> enAttente = waiting;
    return immeuble;
}

ListeDeListes* createSatisfied(void){
    ListeDeListes *satisfaits = malloc(sizeof(ListeDeListes));
    satisfaits -> etage0 = insertPersonList(NULL,NULL);
    satisfaits -> etage1 = insertPersonList(NULL,NULL);
    satisfaits -> etage2 = insertPersonList(NULL,NULL);
    satisfaits -> etage3 = insertPersonList(NULL,NULL);
    satisfaits -> etage4 = insertPersonList(NULL,NULL);
    return satisfaits;
}

void insertSatisfied(ListeDePersonnes *news, ListeDeListes *satisfied, int floor){
    if(floor == 0){
        while(news->longueur != 0){
            satisfied->etage0 = insertPersonList(news->tete,satisfied->etage0);
            news->tete = news->queue->tete;
            news->queue = news->queue->queue;
            news->longueur -= 1;
        }
    }
    if(floor == 1){
        while(news->longueur != 0){
            satisfied->etage1 = insertPersonList(news->tete,satisfied->etage1);
            news->tete = news->queue->tete;
            news->queue = news->queue->queue;
            news->longueur -= 1;
        }
    }
    if(floor == 2){
        while(news->longueur != 0){
            satisfied->etage2 = insertPersonList(news->tete,satisfied->etage2);
            news->tete = news->queue->tete;
            news->queue = news->queue->queue;
            news->longueur -= 1;
        }
    }
    if(floor == 3){
        while(news->longueur != 0){
            satisfied->etage3 = insertPersonList(news->tete,satisfied->etage3);
            news->tete = news->queue->tete;
            news->queue = news->queue->queue;
            news->longueur -= 1;
        }
    }
    if(floor == 4){
        while(news->longueur != 0){
            satisfied->etage4 = insertPersonList(news->tete,satisfied->etage4);
            news->tete = news->queue->tete;
            news->queue = news->queue->queue;
            news->longueur -= 1;
        }
    }
    return;
}