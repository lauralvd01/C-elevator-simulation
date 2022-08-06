#include "structures.h"
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>


void printPersonne(Personne* personne){
    printf("(%d%d)",personne->depart,personne->arrivee);
    return;
}

void printListeDePersonnes(ListeDePersonnes* liste){
    Personne *personne_a_afficher = liste->tete;
    ListeDePersonnes *suivant = liste->queue;

    while(personne_a_afficher != NULL){
        printPersonne(personne_a_afficher);
        personne_a_afficher = suivant->tete; // Passage à la personne désignée comme suivant par la queue
        suivant = suivant->queue; // Le suivant devient la personne suivant la personne suivante
    }
    return;
}

void printImmeuble(Immeuble *immeuble,ListeDePersonnes **satisfaits){
    int largeur = immeuble->ascenseur->capacite;
    int hauteur = immeuble->nbredEtages;
    int etage_asc = immeuble->ascenseur->etageActuel;
    int nb_occupants = immeuble->ascenseur->transportes->longueur;

    printf("N° d'étage  |  ");
    int i;
    for (i=0;i<largeur;i++){
        printf("       ");
    }
    printf("  |  En attente\n");


    int j;
    for (j=hauteur;j>=0;j--){
        printf("     %d      |  ",j);

        if(j != etage_asc){
            for (i=0;i<largeur;i++){
                printf("       ");
            }
        }
        if(j == etage_asc){
            printf(" [ ");
            printListeDePersonnes(immeuble->ascenseur->transportes);
            if(nb_occupants < largeur){
                for(i=nb_occupants; i<largeur; i++){
                    printf("    ");
                }
            }
            printf(" ]");
        }

        printf("  |  ");
        
        printListeDePersonnes(immeuble->enAttente +i);
        printListeDePersonnes(satisfaits+i);
        printf("\n");
    }
    return;
}