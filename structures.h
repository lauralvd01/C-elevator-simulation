#ifndef STRUCTURES_H
#define STRUCTURES_H

#include<ncurses.h>

enum etat { ETAT_OUTSIDE, ETAT_ENATTENTE, ETAT_DANSASCENSEUR, ETAT_SORTI };

enum etatporte { PORTE_SOUVRE, PORTE_OUVERTE, PORTE_SEFERME, PORTE_FERMEE };

 /*  Structure représentant un élément de la pile. */
typedef struct pile
{
        int pers_etagedep;
        int pers_etagedest;
        int pers_etageactu;
        int pers_heuredep;
        enum etat pers_etat;
        struct pile *prec;
} pile ;

typedef struct _Ascenseur {
    int capacite;
    int etageActuel;
    int etageDest;
    enum etatporte porte;
} Ascenseur;

typedef struct _Immeuble {
    /* Un immeuble est défini par son nombre d'étages, un ascenseur, et un double pointeur enAttente qui correspond à un tableau de pointeurs pointant chacun sur une liste de personnes (une liste par étage) */
    int nbredEtages; /* NBREDETAGES = 5 => 4 étages et 1 rez-de-chaussée */
    Ascenseur *ascenseur;
} Immeuble;

/** Fonctions de gestion de la pile de personnes **/
void Push(pile **, int,int,int,enum etat);    /*  Push empile une valeur sur la pile. */
void Pop(pile **);                  /*  Pop retire la dernière valeur empilée sur la pile. */
void Clear(pile **);                /*  Clear vide la pile. */
int Length(pile *p);                /*  Length retourne le nombre d'éléments de la pile. */
void View(pile *);                  /*  Affiche la totalité de la pile en commençant par le sommet. */
void ViewW(WINDOW *fenetre,int offset, pile *p);

/** Fonctions de création **/
Ascenseur* creerAscenseur(int capacite, int etage_actuel);
Immeuble* creerImmeuble(int nbre_etages, Ascenseur *ascenseur);

/** Fonctions d'affichage **/
void displayHeureActuelle(WINDOW *fenetre,int heure, int nb);
void displayAfficheLigneCommandes(WINDOW *fenetre);
void displayCacheLigneCommandes(WINDOW *fenetre);
void displayScene(WINDOW *fenetre,Immeuble *immeuble, pile *p);
void animAscenceur(WINDOW *fenetre, Immeuble *immeuble, enum etatporte action);

/** Fonctionnement de l'ascenseur **/
void sortirDelAscenseur(pile **p,int etagesorti);
void entrerDanslAscenseur(pile **p,int etageactuel,int etagecible, int capacite);
void MonterAveclAscenseur(pile **p,int etageascenceur);

#endif