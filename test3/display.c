#include "structures.h"

#include<ncurses.h>

#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
 

void displayListeDePersonnes(WINDOW *fenetre,int etage,int colomne,ListeDePersonnes* liste){
    if(liste->longueur == 0){
        return;
    }
    else{
        /* Affiche la première personne de la liste telle que (12) si 1 est son départ et 2 son arrivée */
        mvwprintw(fenetre,etage,colomne,"(%d%d)",liste->tete->depart,liste->tete->arrivee);
        
        /* On poursuit en affichant la suite de la liste, jusqu'à la fin */
        colomne = colomne + 4;
        displayListeDePersonnes(fenetre,etage,colomne,liste->queue);        
    }
    return;
}
/*
void printImmeuble(Immeuble *immeuble,ListeDePersonnes **satisfaits){
    int hauteur = immeuble->nbredEtages;
    int largeur_asc = immeuble->ascenseur->capacite;
    int etage_asc = immeuble->ascenseur->etageActuel;
    int nb_occupants = immeuble->ascenseur->transportes->longueur;

    printf("N° d'étage  |                "); LIGNE 3 COL COLS/2 - srlen("N° d'étage  |                ")
    Essayer Reverse ??
    printf("                |  En attente\n"); LIGNE 3 COL COLS/2


    int j;
    for (j=hauteur-1;j>=0;j--){
        printf("     %d      |  ",j);

        if(j != etage_asc){
            for (i=0;i<largeur_asc;i++){
                printf("       ");
            }
        }
        if(j == etage_asc){
            printf(" [ ");
            printListeDePersonnes(immeuble->ascenseur->transportes);
            if(nb_occupants < largeur_asc){
                for(i=nb_occupants; i<largeur_asc; i++){
                    printf("    ");
                }
            }
            printf(" ]");
        }

        printf("  |  ");
        
        printListeDePersonnes(immeuble->enAttente[j]);
        printListeDePersonnes(satisfaits[j]);
        printf("\n");
    }
    printf("\n\n");
    return;
}*/