#include "structures.h"
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>


void sortirDelAscenseur(Ascenseur *ptr_ascenseur){
    /* Observe une à une les personnes transportées par l'ascenseur de l'immeuble et les efface de la liste si elles souhaitent sortir à l'étage atteint*/

    int etage = ptr_ascenseur->etageActuel;
    ListeDePersonnes *ptr_danslAscenseur = ptr_ascenseur->transportes;
    ListeDePersonnes *ptr_restants = insererPersonneListe(NULL,NULL);

    while(ptr_danslAscenseur->longueur != 0){

        Personne *ptr_personneObservee = ptr_danslAscenseur->tete; /* On observe la personne en haut de la pile de celles présentes dans l'ascenseur */

        if(ptr_personneObservee->arrivee != etage){ /* La personne en question descend-elle à cet étage ? */
             ptr_restants = insererPersonneListe(ptr_personneObservee,ptr_restants); /* Non => elle restera dans l'ascenseur */
        }

        ptr_danslAscenseur = ptr_danslAscenseur->queue; /* On descend dans la pile pour observer les personnes suivantes*/
    }
    ptr_ascenseur->transportes = ptr_restants; /* On actualise l'ascenseur */

    return;
}

void entrerDanslAscenseur(WINDOW *fenetre,Immeuble *ptr_immeuble){
    /* Fait entrer dans l'ascenseur autant de personnes en attente à l'étage atteint qu'il y a de place */

    Ascenseur *ptr_ascenseur = ptr_immeuble->ascenseur;
    int etage = ptr_ascenseur->etageActuel;
    int capacite = ptr_ascenseur->capacite;

    ListeDePersonnes *ptr_en_attente_ici = ptr_immeuble->enAttente[etage];
    int i = 1;
    displayListeDePersonnes(fenetre,LINES-i,COLS/2,ptr_en_attente_ici);
    wrefresh(fenetre);
    i++;

    /* Tant qu'il y a de la place dans l'ascenseur */
    while(ptr_ascenseur->transportes->longueur < capacite){

        Personne *personne_qui_monte = ptr_en_attente_ici->tete; /* On fait rentrer le premier de la pile d'attente */
        ptr_ascenseur->transportes = insererPersonneListe(personne_qui_monte,ptr_ascenseur->transportes);
        displayAscenseur(fenetre,ptr_immeuble);
        wrefresh(fenetre);

        ptr_en_attente_ici = ptr_en_attente_ici->queue; /* et on le sort de la liste d'attente de l'étage */
        displayListeDePersonnes(fenetre,LINES-i,COLS/2,ptr_en_attente_ici);
        wrefresh(fenetre);

        displayAscenseur(fenetre,ptr_immeuble);
        wrefresh(fenetre);

        int dest = rand() % (ptr_immeuble->nbredEtages);
        Personne *nouvelle_personne = creerPersonne(etage,dest);
        ptr_en_attente_ici = insererPersonneListe(nouvelle_personne,ptr_en_attente_ici); /* On ajoute une nouvelle personne à la file d'attente */
        displayListeDePersonnes(fenetre,LINES-i,COLS/2 - 20,ptr_en_attente_ici);
        wrefresh(fenetre);
        i ++;

        displayImmeuble(fenetre,ptr_immeuble);
        wrefresh(fenetre);
        getch();
    }

    return;
}
