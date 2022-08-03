#include "structures.h"
#include <stddef.h>
#include<stdio.h>

#define NBREDETAGES 4
#define CAPACITE 5
#define DEPART 0

int main() {
    Personne *p = createPerson(1,2);
    ListeDePersonnes *lst = insertPersonList(p,NULL);
    Ascenseur *asc = createElevator(3,DEPART,lst);
    ListeDeListes *enAttente = createWaiting();
    Immeuble *immeuble = creataBuilding(NBREDETAGES,asc,enAttente);

    printf("destination de p : %d\n",p->arrivee);
    printf("currentFloor : %d\n",asc->etageActuel);
    int i;
    for (i=0;i<=NBREDETAGES;i++){
        printf("etage %d : \n",i);
    }

    return 0;
}
