#include "structures.h"
#include <stddef.h>
#include<stdio.h>

#define NBREDETAGES 4 //Pour le moment ne pas changer ce paramètre
#define CAPACITE 5
#define DEPART 0 //Pour le moment ne pas changer ce paramètre

//int determinateDestination(Immeuble *building){
//    int dest;
//    if(building->ascenseur->etageActuel == 0){
//        dest = min(1,building->nbredEtages);
//    }
//    else if(building->ascenseur->etageActuel == building->nbredEtages){
//        dest = max(0,(building->nbredEtages -1));
//    }
//    else{
//        dest = ?
//    }
//}

int tousSatisfaits(ListeDeListes *wait){
    int yes = 1; //Plus personne en attente ?
    int i;
    for (i=0;i<=NBREDETAGES;i++){
        if(i == 0){
            if(wait->etage0->longueur != 0){
                yes = 0; //Certains le sont toujours
            }
        }
        else if(i == 1){
            if(wait->etage1->longueur != 0){
                yes = 0; //Certains le sont toujours
            }
        }
        else if(i == 2){
            if(wait->etage2->longueur != 0){
                yes = 0; //Certains le sont toujours
            }
        }
        else if(i == 3){
            if(wait->etage3->longueur != 0){
                yes = 0; //Certains le sont toujours
            }
        }
        else{
            if(wait->etage4->longueur != 0){
                yes = 0; //Certains le sont toujours
            }
        }
    }
    return yes;
}

int main() {
    // INITIALISATION
    printf("                  SITUATION DE DEPART\n\n");
    Ascenseur *asc = createElevator(CAPACITE,DEPART,insertPersonList(NULL,NULL));
    ListeDeListes *wait = createWaiting();
    Immeuble *imm = createBuilding(NBREDETAGES,asc,wait);
    printBuilding(imm);

    int stop = 0;
    while(stop == 0){

        if((imm->ascenseur->transportes->longueur == 0) & (tousSatisfaits(imm->enAttente) == 1)){
            stop = 1;
        }

        int dest;
        for(dest=DEPART;dest<=NBREDETAGES;dest++){
            //L'ascenseur monte, dépose et récupère à chaque étage, jusqu'à être arrivé en haut
        }

        for(dest=NBREDETAGES;dest>=0;dest--){
            //L'ascenseur descend, dépose et récupère à chaque étage, jusqu'à être arrivé en bas
        }


        stop = 1;
    }

    // FIN
    printf("\n\n                  SITUATION FINALE\n\n");
    printBuilding(imm);
    
    return 0;
}
