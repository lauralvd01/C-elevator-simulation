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

    printf("               SITUATION DE DEPART\n\n");
    printImmeuble(immeuble,satisfaits);


    /* TESTS */
    printf("\n\n               TESTS\n\n");
    entrerDanslAscenseur(immeuble);
    printImmeuble(immeuble,satisfaits);
    immeuble->ascenseur->destination = 2;
    immeuble->ascenseur->etageActuel = 2;
    entrerDanslAscenseur(immeuble);
    printImmeuble(immeuble,satisfaits);
    
    immeuble->ascenseur->destination = 0;
    immeuble->ascenseur->etageActuel = 0;
    sortirDelAscenseur(immeuble,satisfaits);
    printImmeuble(immeuble,satisfaits);


    /* COMMENTAIRES */
/*  Les fonctionnements de entrerDanslAscenseur et sortirDelAscenseur n'ont pas encore été testés
    maintenant qu'ils peuvent l'être, il est évident qu'ils ont des problèmes

    printListeDePersonnes doit produire une erreur également car quand utilisée dans la fonction sortirDelAscenseur pour afficher des tests, cela produit une Segmentation fault
*/

    /* LANCEMENT DE L AUTOMATE */
    int stop = 1;
    while(stop == 0){

        if((immeuble->ascenseur->transportes->longueur == 0) & (tousSatisfaits(immeuble->enAttente) == 1)){
            stop = 1;
        }
        stop = 1;
    }

    /* FIN */
    printf("\n\n               SITUATION FINALE\n\n");
    printImmeuble(immeuble,satisfaits);
    

    /* NETTOYAGE DE LA MEMOIRE */

    int k;
    for(k=0;k<NBREDETAGES;k++){
        printf("\nEtage %d",k);
        while((immeuble->enAttente[k])->tete != NULL){
            printf("\nen attente :");
            printListeDePersonnes(immeuble->enAttente[k]);
            immeuble->enAttente[k] = supprimerTeteListe(immeuble->enAttente[k]);
            /* => C'est le free(tete) qui fait tout capoter et affiche free(): double free detected in tcache 2 | make: *** [makefile:4: run] Aborted */
            
        }
        while((satisfaits[k])->tete != NULL){
            printf("\nsatisfaits :");
            printListeDePersonnes(satisfaits[k]);
            satisfaits[k] = supprimerTeteListe(satisfaits[k]);
            /* => C'est le free(tete) qui fait tout capoter et affiche free(): double free detected in tcache 2 | make: *** [makefile:4: run] Aborted */
        }
    }

    while(immeuble->ascenseur->transportes->longueur != 0){
        ListeDePersonnes *liste = immeuble->ascenseur->transportes;
        printf("\nDans l'ascenseur :");
        printListeDePersonnes(liste);
        printf("\nPersonne :");
        printPersonne(liste->tete);
        printf("\nQueue :");
        printListeDePersonnes(liste->queue);
        /*free(liste->tete);*/ /* => C'est le free(tete) qui fait tout capoter et affiche free(): double free detected in tcache 2 | make: *** [makefile:4: run] Aborted */
        liste = liste->queue;
        /*immeuble->ascenseur->transportes = supprimerTeteListe(immeuble->ascenseur->transportes);*/
        immeuble->ascenseur->transportes = realloc(immeuble->ascenseur->transportes,sizeof(liste));
        immeuble->ascenseur->transportes = liste;
        printf("\n\n Résultat\n");
        printListeDePersonnes(immeuble->ascenseur->transportes);
    }
    free(immeuble->ascenseur);
    free(immeuble);
    free(fin);

    return 0;
}
