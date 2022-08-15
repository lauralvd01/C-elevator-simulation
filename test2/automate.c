#include "structures.h"
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>

/* int determinateDestination(Immeuble *building){
    / DETERMINE LA PROCHAINE DESTINATION DE L ASCENSEUR /
    int dest;
    
    return dest
} */

/* Immeuble* moove(Immeuble *building,int destination){
    / EFFECTUE ET AFFICHE LE DEPLACEMENT DE L ASCENSEUR JUSQU A SA DESTINATION /

} */


void sortirDelAscenseur(Immeuble *immeuble,ListeDePersonnes **satisfaits){
    /* Observe une à une les personnes transportées par l'ascenseur de l'immeuble et les classe selon si elles sortent à l'étage atteint (elles rentrent alors dans la liste des satisfaits) ou si elles restent dans l'ascenseur (liste restants qui devient la nouvelle liste des personnes transportées dans l'ascenseur) */
    Ascenseur *ascenseur = immeuble->ascenseur;
    int etage = ascenseur->etageActuel;
    ListeDePersonnes *candidats_sortants = ascenseur->transportes;
    ListeDePersonnes *restants = insererPersonneListe(NULL,NULL);

    while(candidats_sortants->longueur != 0){
        if(candidats_sortants->tete->arrivee == etage){ /* La personne en tête de la liste des transportés descend-elle à cet étage ? */
            satisfaits[etage] = insererPersonneListe(candidats_sortants->tete,satisfaits[etage]); /* Oui => elle sort de l'ascenseur et rentre dans la liste des gens satisfaits à cet étage */
        }
        else{
            restants = insererPersonneListe(candidats_sortants->tete,restants); /* Non => elle reste dedans */
        }
        candidats_sortants = supprimerTeteListe(candidats_sortants); /* On passe au candidat suivant */
    }
    immeuble->ascenseur->transportes = realloc(immeuble->ascenseur->transportes,sizeof(restants));
    immeuble->ascenseur->transportes = restants; /* On actualise l'ascenseur */
    return;
}

void entrerDanslAscenseur(Immeuble *immeuble){
    /* Fait entrer autant de personnes en attente à l'étage visité dans l'ascenseur qu'il y a de place dans celui-ci */
    Ascenseur *ascenseur = immeuble->ascenseur;
    int etage = ascenseur->etageActuel;
    ListeDePersonnes *en_attente_ici = immeuble->enAttente[etage];
    
    /* Tant qu'il y a de la place dans l'ascenseur et qu'il reste des gens en attente à l'étage */
    while( (ascenseur->transportes->longueur < ascenseur->capacite) & (en_attente_ici->longueur != 0) ){
        Personne *personne_qui_monte = en_attente_ici->tete;
        ascenseur->transportes = insererPersonneListe(personne_qui_monte,ascenseur->transportes); /* Le premier en attente rentre dans l'ascenseur */
        en_attente_ici = supprimerTeteListe(en_attente_ici); /* et sort de la liste d'attente de l'étage */
    }
    immeuble->enAttente[etage] = realloc(immeuble->enAttente[etage],sizeof(en_attente_ici));
    immeuble->enAttente[etage] = en_attente_ici;
    return;
}
