#include "structures.h"
#include<stdlib.h>
#include<stdio.h>


void printPerson(Personne* p){
    printf("(%d%d)",p->depart,p->arrivee);
    return;
}

void printPersonList(ListeDePersonnes* lst){
    // Copie "en profondeur" de la liste pour ne pas l'effacer en l'affichant
    ListeDePersonnes *liste = insertPersonList(lst->tete,lst->queue);
    while(liste -> tete != NULL){
        printPerson(liste->tete);
        liste ->tete = liste ->queue ->tete;
        liste ->queue = liste ->queue ->queue;
    }
    return;
}

void printBuilding(Immeuble *building){
    int largeur = building->ascenseur->capacite;
    int hauteur = building->nbredEtages;
    int Nasc = building->ascenseur->etageActuel;

    printf("N° d'étage  |  ");
    int i;
    for (i=0;i<largeur;i++){
        printf("     ");
    }
    printf("  |  En attente\n");


    int j;
    for (j=hauteur;j>=0;j--){
        printf("     %d      |  ",j);

        if(j != Nasc){
            for (i=0;i<largeur;i++){
                printf("     ");
            }
        }
        if(j == Nasc){
            printf(" [ ");
            printPersonList(building->ascenseur->transportes);
            if(building->ascenseur->transportes->longueur < largeur){
                for(i=building->ascenseur->transportes->longueur; i<largeur; i++){
                    printf("    ");
                }
            }
            printf(" ]");
        }

        printf("  |  ");
        if(j == 0){
            printPersonList(building->enAttente->etage0);
        }
        if(j == 1){
            printPersonList(building->enAttente->etage1);
        }
        if(j == 2){
            printPersonList(building->enAttente->etage2);
        }
        if(j == 3){
            printPersonList(building->enAttente->etage3);
        }
        if(j == 4){
            printPersonList(building->enAttente->etage4);
        }
        printf("\n");
    }
    return;
}