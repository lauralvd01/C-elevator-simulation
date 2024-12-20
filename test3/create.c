#include "structures.h"
#include<stdlib.h>
#include<assert.h>

Personne* creerPersonne(int dep, int dest) {
    /* Crée un pointeur pointant sur un objet de type Personne et par lui attribue ses étages de départ et destination à l'objet */
    Personne *p;
    p =(Personne*)malloc(sizeof(Personne));
    p-> depart = dep;
    p-> arrivee = dest;
    return p;
}

ListeDePersonnes* insererPersonneListe(Personne *new_personne, ListeDePersonnes *liste){
    /* Prend en argument un pointeur pointant vers une personne à ajouter au début de la liste pointée par le pointeur pris en 2nd argument
       Crée un pointeur pointant sur une nouvelle liste de personne et par lui attribue sa longueur, celle de la liste incrémentée de 1, sa tête, le pointeur pointant vers la personne à insérer, et sa queue,le pointeur pointant vers la liste à compléter
       Retourne le pointeur pointant vers la nouvelle liste */
    ListeDePersonnes *new_liste;
    new_liste = (ListeDePersonnes*)malloc(sizeof(ListeDePersonnes));

    /* Si les deux pointeurs en argument sont NULL c'est que l'on crée une nouvelle liste, que l'on initialise à une liste de longueur nulle, avec tête et queue pointant vers NULL */
    if((new_personne == NULL) & (liste == NULL)){
        new_liste->longueur = 0;
    } 
    else{
        new_liste->longueur = liste->longueur +1;
    }
    new_liste -> tete = new_personne;
    new_liste -> queue = liste;
    return new_liste;
}

ListeDePersonnes* insererPersonneFile(ListeDePersonnes *liste,Personne *personne){
    /* Permet d'utiliser la liste chainée comme une file plutôt qu'une pile : insère la personne à ajouter juste avant la fin de la liste chainée,
       au lieu de la mettre en tête */
    ListeDePersonnes *fin = insererPersonneListe(NULL,NULL);

    if(liste->longueur == 0){
        liste = insererPersonneListe(personne,fin);
        return liste;
    }
    else{
        liste->longueur ++;
        liste->queue = insererPersonneFile(liste->queue,personne);
    }
    return liste;
}

Ascenseur* creerAscenseur(int capacite, int etage_actuel,ListeDePersonnes *personnnes_dedans){
    /* Crée un pointeur pointant vers un objet ascenseur et par lui attribue à l'ascenseur sa capacité, son étage, initialise sa destination à l'étage où il se trouve
       vérifie que la liste de personnes pointée par le dernier argument n'est pas trop longue par rappor à la capacité de l'ascenseur, et lui attribue ce pointeur le cas échéant, comme pointeur pointant vers la liste des personnes transportées par l'ascenseur */
    Ascenseur *ascenseur;
    ascenseur = (Ascenseur*)malloc(sizeof(Ascenseur));
    ascenseur -> capacite = capacite;
    ascenseur -> etageActuel = etage_actuel;
    ascenseur -> destination = -1;
    assert(personnnes_dedans->longueur <= capacite);
    ascenseur -> transportes = personnnes_dedans;
    return ascenseur;
}


Immeuble* creerImmeuble(int nbre_etages, Ascenseur *ascenseur, ListeDePersonnes **en_attente){
    /* Crée un pointeur pointant vers un objet de type Immeuble et par lui attribue à l'objet son nombre d'étages, le pointeur pointant vers son ascenseur et le pointeur correspondant au tableau des pointeurs pointants vers les listes des personnes en attente à chaque étage */
    Immeuble *immeuble;
    immeuble = (Immeuble*)malloc(sizeof(Immeuble));
    immeuble -> nbredEtages = nbre_etages;
    immeuble -> ascenseur = ascenseur;
    immeuble -> enAttente = en_attente;
    return immeuble;
}