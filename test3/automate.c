#include "structures.h"

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>


void sortirDelAscenseur(Immeuble *ptr_immeuble){
    /* Observe une à une les personnes transportées par l'ascenseur de l'immeuble et les efface de la liste si elles souhaitent sortir à l'étage atteint*/

    Ascenseur *ptr_ascenseur = ptr_immeuble->ascenseur;
    int etage = ptr_ascenseur->etageActuel;
    ListeDePersonnes *ptr_danslAscenseur = ptr_ascenseur->transportes;
    ListeDePersonnes *ptr_restants = insererPersonneListe(NULL,NULL);

    while(ptr_danslAscenseur->longueur != 0){

        Personne *ptr_personneObservee = ptr_danslAscenseur->tete; /* On observe la personne en haut de la pile de celles présentes dans l'ascenseur */

        if(ptr_personneObservee->arrivee != etage){ /* La personne en question descend-elle à cet étage ? */
             ptr_restants = insererPersonneFile(ptr_restants,ptr_personneObservee); /* Non => elle restera dans l'ascenseur */
        }

        ptr_danslAscenseur = ptr_danslAscenseur->queue; /* On descend dans la pile pour observer les personnes suivantes*/
    }

    ptr_immeuble->ascenseur->transportes = ptr_restants; /* On actualise l'ascenseur */
    return;
}

void entrerDanslAscenseur(WINDOW *fenetre,Immeuble *ptr_immeuble){
    /* Fait entrer dans l'ascenseur autant de personnes en attente à l'étage atteint qu'il y a de place */

    Ascenseur *ptr_ascenseur = ptr_immeuble->ascenseur;
    int etage = ptr_ascenseur->etageActuel;
    int capacite = ptr_ascenseur->capacite;

    ListeDePersonnes *ptr_en_attente_ici = ptr_immeuble->enAttente[etage];

    /* Tant qu'il y a de la place dans l'ascenseur */
    while(ptr_ascenseur->transportes->longueur < capacite){

        Personne *personne_qui_monte = ptr_en_attente_ici->tete; /* On fait rentrer le premier de la pile d'attente */
        ptr_ascenseur->transportes = insererPersonneFile(ptr_ascenseur->transportes,personne_qui_monte);

        ptr_en_attente_ici = ptr_en_attente_ici->queue; /* et on le sort de la liste d'attente de l'étage */
        ptr_immeuble->enAttente[etage] = ptr_en_attente_ici;

        displayImmeuble(fenetre,ptr_immeuble); /* Affichage */
        wrefresh(fenetre);
        sleep(1);

        int dest = rand() % (ptr_immeuble->nbredEtages);
        Personne *nouvelle_personne = creerPersonne(etage,dest);
        ptr_en_attente_ici = insererPersonneFile(ptr_en_attente_ici,nouvelle_personne); /* On ajoute une nouvelle personne sur le haut de la pile d'attente */
        ptr_immeuble->enAttente[etage] = ptr_en_attente_ici;

        displayImmeuble(fenetre,ptr_immeuble); /* Affichage */
        wrefresh(fenetre);
        sleep(1);
    }

    return;
}
