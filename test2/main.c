#include "structures.h"
#include <stddef.h>
#include<stdio.h>

#define NBREDETAGES 4
#define CAPACITE 5
#define DEPART 0


int main() {
    Personne *p = createPerson(1,2);
    ListeDePersonnes *lst = insertPersonList(p,insertPersonList(NULL,NULL));
    lst = insertPersonList(createPerson(2,3),lst);
    lst = insertPersonList(createPerson(4,0),lst);
    Ascenseur *asc = createElevator(CAPACITE,DEPART,lst);
    ListeDeListes *enAttente = createWaiting();
    Immeuble *immeuble = creataBuilding(NBREDETAGES,asc,enAttente);

    printf("longueur de la liste des transportés (normalement 3) : %d\n",asc->transportes->longueur);
    printBuilding(immeuble);

    return 0;
}
