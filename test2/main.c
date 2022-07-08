#include "structures.h"
#include <stddef.h>
#include<stdio.h>

int main() {
    Personne *p = createPerson(1,2);
    ListeDePersonnes *lst = insertPersonList(p,NULL);
    Ascenseur *asc = createElevator(3,1,lst);

    printf("destination de p : %d\n",p->arrivee);
    printf("currentFloor : %d\n",asc->etageActuel);

    return 0;
}
