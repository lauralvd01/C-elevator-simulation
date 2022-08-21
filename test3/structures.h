#ifndef STRUCTURES_H
#define STRUCTURES_H

#include<ncurses.h>

typedef struct _Personne {
    /* Une personne est définie par ses étages de départ et de destination */
    int depart;
    int arrivee;
} Personne;

typedef struct _ListeDePersonnes {
    /* Une liste de personnes est une liste chaînée définie par sa longueur, par un pointeur tête pointant sur une personne et par un pointeur queue pointant sur la suite de la liste
       La fin de la liste chaînée est caractérisée par une longueur valant 0 et les pointeurs tête et queue pointant sur NULL */
    int longueur;
    Personne *tete;
    struct _ListeDePersonnes *queue;
} ListeDePersonnes;

typedef struct _Ascenseur {
    /* Un ascenseur est défini par sa capacité, l'étage auquel il se trouve, sa prochaine destination, et un pointeur transportés pointant sur une liste de personnes */
    int capacite;
    int etageActuel;
    int destination;
    ListeDePersonnes *transportes;
} Ascenseur;

typedef struct _Immeuble {
    /* Un immeuble est défini par son nombre d'étages, un ascenseur, et un double pointeur enAttente qui correspond à un tableau de pointeurs pointant chacun sur une liste de personnes (une liste par étage) */
    int nbredEtages; /* NBREDETAGES = 5 => 4 étages et 1 rez-de-chaussée */
    Ascenseur *ascenseur;
    ListeDePersonnes **enAttente;
} Immeuble;


/** Fonctions de création **/
Personne* creerPersonne(int dep, int dest);
ListeDePersonnes* insererPersonneListe(Personne *new_personne, ListeDePersonnes *liste);
ListeDePersonnes* insererPersonneFile(ListeDePersonnes *liste,Personne *personne);
Ascenseur* creerAscenseur(int capacite, int etage_actuel,ListeDePersonnes *personnnes_dedans);
Immeuble* creerImmeuble(int nbre_etages, Ascenseur *ascenseur, ListeDePersonnes **en_attente);

/** Fonctions d'affichage **/
void displayListeDePersonnes(WINDOW *fenetre,int etage,int colomne,ListeDePersonnes* liste);
void displayAscenseur(WINDOW *fenetre,Immeuble *immeuble);
void displayImmeuble(WINDOW *fenetre,Immeuble *immeuble);

/** Fonctionnement de l'ascenseur **/
int tousSatisfaits(ListeDePersonnes **en_attente);
/*int determinateDestination(Immeuble *building);*/
/*Immeuble* moove(Immeuble *building,int destination);*/
void sortirDelAscenseur(Immeuble *ptr_immeuble);
void entrerDanslAscenseur(WINDOW *fenetre,Immeuble *immeuble);


#endif
