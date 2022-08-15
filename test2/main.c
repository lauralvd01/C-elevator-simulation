#include "structures.h"
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define NBREDETAGES 5 /* Pour le moment ne pas changer ce paramètre */ /* NBREDETAGES = 5 => 4 étages et 1 rez-de-chaussée */
#define CAPACITE 2
#define DEPART 0 /* Pour le moment ne pas changer ce paramètre */

ListeDePersonnes *fin;
Ascenseur *ascenseur;
ListeDePersonnes **ptr_enAttente;
ListeDePersonnes **ptr_satisfaits;
Immeuble *immeuble;


int tousSatisfaits(ListeDePersonnes **en_attente){
    /* DETERMINE S IL Y A ENCORE DES GENS EN ATTENTE DANS L ASCENSEUR OU LES ETAGES */

    int yes = 1; /* Plus personne en attente ? */
    int i;
    for (i=0;i<NBREDETAGES;i++){
        if(en_attente[i]->longueur != 0){
                yes = 0; /* Certains le sont toujours */
            }
    }
    return yes;
}

/* int determinateDestination(Immeuble *building){
    / DETERMINE LA PROCHAINE DESTINATION DE L ASCENSEUR /
    int dest;
    
    return dest
} */

/* Immeuble* moove(Immeuble *building,int destination){
    / EFFECTUE ET AFFICHE LE DEPLACEMENT DE L ASCENSEUR JUSQU A SA DESTINATION /

} */

/*void sortirDelAscenseur(Immeuble *immeuble,ListeDePersonnes **satisfaits){ */
    /* Observe une à une les personnes transportées par l'ascenseur de l'immeuble et les classe selon si elles sortent à l'étage atteint (elles rentrent alors dans la liste des satisfaits) ou si elles restent dans l'ascenseur (liste restants qui devient la nouvelle liste des personnes transportées dans l'ascenseur) */
    /*Ascenseur *ascenseur = immeuble->ascenseur;
    int etage = ascenseur->etageActuel;
    ListeDePersonnes *candidats_sortants = ascenseur->transportes;
    ListeDePersonnes *restants = insererPersonneListe(NULL,NULL);

    while(candidats_sortants->longueur != 0){
        if(candidats_sortants->tete->arrivee == etage){ */ /* La personne en tête de la liste des transportés descend-elle à cet étage ? */
            /*satisfaits[etage] = insererPersonneListe(candidats_sortants->tete,satisfaits[etage]); */ /* Oui => elle sort de l'ascenseur et rentre dans la liste des gens satisfaits à cet étage */
        /*}
        else{
            restants = insererPersonneListe(candidats_sortants->tete,restants); */ /* Non => elle reste dedans */
        /*}
        candidats_sortants = supprimerTeteListe(candidats_sortants); *//* On passe au candidat suivant */
    /*}
    immeuble->ascenseur->transportes = restants; */ /* On actualise l'ascenseur */
    /*return;
}*/

/*void entrerDanslAscenseur(Immeuble *immeuble){ */
    /* Fait entrer autant de personnes en attente à l'étage visité dans l'ascenseur qu'il y a de place dans celui-ci */
    /*Ascenseur *ascenseur = immeuble->ascenseur;
    int etage = ascenseur->etageActuel;
    ListeDePersonnes *en_attente_ici = immeuble->enAttente[etage];*/
    
    /* Tant qu'il y a de la place dans l'ascenseur et qu'il reste des gens en attente à l'étage */
    /*while( (ascenseur->transportes->longueur < ascenseur->capacite) & (en_attente_ici->longueur != 0) ){
        Personne *personne_qui_monte = en_attente_ici->tete;
        ascenseur->transportes = insererPersonneListe(personne_qui_monte,ascenseur->transportes);*/ /* Le premier en attente rentre dans l'ascenseur */
        
        /*en_attente_ici = supprimerTeteListe(en_attente_ici);*/ /* et sort de la liste d'attente de l'étage */
    /*}
    return;
}*/

int main() {
    /* CONSTANTES */
    fin = (ListeDePersonnes*)malloc(sizeof(ListeDePersonnes));
    fin->longueur = 0;
    fin->tete = NULL;
    fin->queue = NULL;
    /* Normalement équivalent à fin = insererTetePseronne(NULL,NULL); */


    /* INITIALISATION */

    ListeDePersonnes* enAttente[NBREDETAGES];
    ListeDePersonnes* satisfaits[NBREDETAGES];
    
    /* Remplissage du tableau des listes de personnes en attente */
    int i;
    for(i=0;i<NBREDETAGES;i++){
        enAttente[i] = (ListeDePersonnes*)malloc(sizeof(ListeDePersonnes));
        Personne *personne_i = creerPersonne(i,i);
        enAttente[i] = insererPersonneListe(personne_i,fin);
    }
    ptr_enAttente = (ListeDePersonnes**)malloc(sizeof(ListeDePersonnes*)*NBREDETAGES);
    ptr_enAttente = enAttente;

    /* Idem pour le tableau des gens satisfaits */
    int j;
    for(j=0;j<NBREDETAGES;j++){
        satisfaits[j] = (ListeDePersonnes*)malloc(sizeof(ListeDePersonnes));
        satisfaits[j] = fin;
    }
    ptr_satisfaits = (ListeDePersonnes**)malloc(sizeof(ListeDePersonnes*)*NBREDETAGES);
    ptr_satisfaits = satisfaits;

    ascenseur = creerAscenseur(CAPACITE,DEPART,fin);
    immeuble = creerImmeuble(NBREDETAGES,ascenseur,ptr_enAttente);

    printf("                  SITUATION DE DEPART\n\n");
    printImmeuble(immeuble,satisfaits);


    /* LANCEMENT DE L AUTOMATE */
    int stop = 1;
    while(stop == 0){

        if((immeuble->ascenseur->transportes->longueur == 0) & (tousSatisfaits(immeuble->enAttente) == 1)){
            stop = 1;
        }

        stop = 1;
    }

    /* FIN */
    printf("\n\n                  SITUATION FINALE\n\n");
    printImmeuble(immeuble,satisfaits);
    

    /* NETTOYAGE DE LA MEMOIRE */

    int k;
    for(k=0;k<NBREDETAGES;k++){
        while((immeuble->enAttente[k])->tete != NULL){
            immeuble->enAttente[k] = supprimerTeteListe(immeuble->enAttente[k]);
        }
        while((satisfaits[k])->tete != NULL){
            satisfaits[k] = supprimerTeteListe(satisfaits[k]);
        }
    }
    while(immeuble->ascenseur->transportes->tete != NULL){
            immeuble->ascenseur->transportes = supprimerTeteListe(immeuble->ascenseur->transportes);
    }
    free(immeuble->ascenseur);
    free(immeuble);
    free(fin);

    return 0;
}
