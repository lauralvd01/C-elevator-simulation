#include "structures.h"
#include <stddef.h>
#include<stdio.h>
#include<stdlib.h>

#define NBREDETAGES 4 //Pour le moment ne pas changer ce paramètre
#define CAPACITE 2
#define DEPART 0 //Pour le moment ne pas changer ce paramètre

ListeDePersonnes *fin;
Ascenseur *ascenseur;
ListeDePersonnes *enAttente[];
ListeDePersonnes *satisfaits[];
ListeDePersonnes **ptr_enAttente;
ListeDePersonnes **ptr_satisfaits;
Immeuble *immeuble;


int tousSatisfaits(ListeDeListes *wait){
    // DETERMINE S IL Y A ENCORE DES GENS EN ATTENTE DANS L ASCENSEUR OU LES ETAGES

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

//int determinateDestination(Immeuble *building){
    // DETERMINE LA PROCHAINE DESTINATION DE L ASCENSEUR

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

//Immeuble* moove(Immeuble *building,int destination){
    // EFFECTUE ET AFFICHE LE DEPLACEMENT DE L ASCENSEUR JUSQU A SA DESTINATION

//}

ListeDePersonnes* exitElevator(Immeuble *building){
    int etage = building->ascenseur->etageActuel;
    ListeDePersonnes *sortants = insertPersonList(NULL,NULL);
    ListeDePersonnes *transportesActualises = insertPersonList(NULL,NULL);

    while(building->ascenseur->transportes->longueur != 0){
        if(building->ascenseur->transportes->tete->arrivee == etage){
            sortants = insertPersonList(building->ascenseur->transportes->tete,sortants); // La personne dans l'ascenseur sort
        }
        else{
            transportesActualises = insertPersonList(building->ascenseur->transportes->tete,transportesActualises); // Sinon elle reste dedans
        }
        building->ascenseur->transportes->tete = building->ascenseur->transportes->queue->tete;
        building->ascenseur->transportes->queue = building->ascenseur->transportes->queue->queue;
        building->ascenseur->transportes->longueur -= 1;
    }
    building->ascenseur->transportes = transportesActualises; // On actualise l'ascenseur

    return sortants;
}

void enterElevator(Immeuble *building){
    // Fait entrer autant de personnes en attente à l'étage visité dans l'ascenseur qu'il y a de place dans celui-ci
    int etage = building->ascenseur->etageActuel;
    if(etage == 0){
        while(building->ascenseur->transportes->longueur < building->ascenseur->capacite){
            ListeDePersonnes *liste;
            liste = insertPersonList(building->enAttente->etage0->tete, building->ascenseur->transportes); // Le premier en attente rentre dans l'ascenseur
            building->ascenseur->transportes = realloc(building->ascenseur->transportes,sizeof(liste));
            building->ascenseur->transportes = liste;
            building->enAttente->etage0->tete = building->enAttente->etage0->queue->tete; // et sort de la liste d'attente de l'étage
            building->enAttente->etage0->queue = building->enAttente->etage0->queue->queue;
            building->enAttente->etage0->longueur -= 1;
        }
    }
    else if(etage == 1){
        while(building->ascenseur->transportes->longueur < building->ascenseur->capacite){
            building->ascenseur->transportes = insertPersonList(building->enAttente->etage1->tete,building->ascenseur->transportes); // Le premier en attente rentre dans l'ascenseur
            building->enAttente->etage1->tete = building->enAttente->etage1->queue->tete; // et sort de la liste d'attente de l'étage
            building->enAttente->etage1->queue = building->enAttente->etage1->queue->queue;
            building->enAttente->etage1->longueur -= 1;
        }
    }
    else if(etage == 2){
        while(building->ascenseur->transportes->longueur < building->ascenseur->capacite){
            building->ascenseur->transportes = insertPersonList(building->enAttente->etage2->tete,building->ascenseur->transportes); // Le premier en attente rentre dans l'ascenseur
            building->enAttente->etage2->tete = building->enAttente->etage2->queue->tete; // et sort de la liste d'attente de l'étage
            building->enAttente->etage2->queue = building->enAttente->etage2->queue->queue;
            building->enAttente->etage2->longueur -= 1;
        }
    }
    else if(etage == 3){
        while(building->ascenseur->transportes->longueur < building->ascenseur->capacite){
            building->ascenseur->transportes = insertPersonList(building->enAttente->etage3->tete,building->ascenseur->transportes); // Le premier en attente rentre dans l'ascenseur
            building->enAttente->etage3->tete = building->enAttente->etage3->queue->tete; // et sort de la liste d'attente de l'étage
            building->enAttente->etage3->queue = building->enAttente->etage3->queue->queue;
            building->enAttente->etage3->longueur -= 1;
        }
    }
    else if(etage == 4){
        while(building->ascenseur->transportes->longueur < building->ascenseur->capacite){
            building->ascenseur->transportes = insertPersonList(building->enAttente->etage4->tete,building->ascenseur->transportes); // Le premier en attente rentre dans l'ascenseur
            building->enAttente->etage4->tete = building->enAttente->etage4->queue->tete; // et sort de la liste d'attente de l'étage
            building->enAttente->etage4->queue = building->enAttente->etage4->queue->queue;
            building->enAttente->etage4->longueur -= 1;
        }
    }
    return;
}

int main() {
    // CONSTANTES
    fin->longueur = 0;
    fin->tete = NULL;
    fin->queue = NULL;
    // Normalement équivalent à fin = insererTetePseronne(NULL,NULL);


    // INITIALISATION

    // Remplissage du tableau des listes de personnes en attente
    int i;
    for(i=0;i<=NBREDETAGES;i++){
        enAttente[i] = (ListeDePersonnes*)malloc(sizeof(ListeDePersonnes));
        Personne *personne_i = creerPersonne(i,i);
        enAttente[i] = insererPersonneListe(personne_i,fin);
    }
    ptr_enAttente = (ListeDePersonnes**)calloc(NBREDETAGES+1,sizeof(ListeDePersonnes*));
    ptr_enAttente = enAttente;

    // Idem pour le tableau des gens satisfaits
    int j;
    for(j=0;j<=NBREDETAGES;j++){
        satisfaits[j] = (ListeDePersonnes*)malloc(sizeof(ListeDePersonnes));
        satisfaits[j] = fin;
    }
    ptr_satisfaits = (ListeDePersonnes**)calloc(NBREDETAGES+1,sizeof(ListeDePersonnes*));
    ptr_satisfaits = enAttente;

    ascenseur = createElevator(CAPACITE,DEPART,fin);
    immeuble = createBuilding(NBREDETAGES,ascenseur,ptr_enAttente);

    printf("                  SITUATION DE DEPART\n\n");
    printBuilding(immeuble,satisfaits);


    // LANCEMENT DE L AUTOMATE
    int stop = 1;
    while(stop == 0){

        if((imm->ascenseur->transportes->longueur == 0) & (tousSatisfaits(imm->enAttente) == 1)){
            stop = 1;
        }

        int dest;
        for(dest=DEPART;dest<=NBREDETAGES;dest++){
            //L'ascenseur monte, dépose et récupère à chaque étage, jusqu'à être arrivé en haut
            printf("\n");
            imm->ascenseur->etageActuel = dest;
            ListeDePersonnes *sortants = exitElevator(imm); //Ceux dans l'ascenseur qui veulent sortir à cet étage sortent de l'ascenseur puis
            insertSatisfied(sortants,sat,dest); // entrent dans la liste des gens satisfaits de l'étage

            enterElevator(imm); //S'il y a de la place dans l'ascenseur, le maximum de gens en attente y rentrent et sorte de la liste d'attente de l'étage
            imm->ascenseur->destination = dest+1;
            printBuilding(imm,sat);
            break;
        }

        for(dest=NBREDETAGES;dest>=0;dest--){
            break;
            //L'ascenseur descend, dépose et récupère à chaque étage, jusqu'à être arrivé en bas
            printf("\n");
            imm->ascenseur->etageActuel = dest;
            ListeDePersonnes *sortants = exitElevator(imm); //Ceux dans l'ascenseur qui veulent sortir à cet étage sortent de l'ascenseur puis
            insertSatisfied(sortants,sat,dest); // entrent dans la liste des gens satisfaits de l'étage

            enterElevator(imm); //S'il y a de la place dans l'ascenseur, le maximum de gens en attente y rentrent et sorte de la liste d'attente de l'étage
            imm->ascenseur->destination = dest-1;
            printBuilding(imm,sat);
        }


        stop = 1;
    }

    // FIN
    printf("\n\n                  SITUATION FINALE\n\n");
    printBuilding(immeuble,satisfaits);
    
    return 0;
}
