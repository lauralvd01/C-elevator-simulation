#ifndef IMMEUBLE_LIB
#define IMMEUBLE_LIB

#include "structures.h"

#include<unistd.h>
#include<ncurses.h>

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>



/******     FONCTIONS DE GESTION DES ENTREES & SORTIES DES PERSONNES DE L'ASCENCEUR    *******/
/*************************************************************************/
void sortirDelAscenseur(pile **p,int etagesorti) /* **p car modifié */
{
    /* Observe une à une les personnes transportées par l'ascenseur de l'immeuble et les efface de la liste si elles souhaitent sortir à l'étage atteint*/
    pile *ptr;
    ptr = (*p);
    while(*p)
        {
            if (((*p)->pers_etageactu == etagesorti && (*p)->pers_etagedest == etagesorti) && ((*p)->pers_etat==ETAT_DANSASCENSEUR))
            {
                (*p)->pers_etat=ETAT_SORTI;
            }
            *p = (*p)->prec;
        }
    *p = ptr; /* Le pointeur pointe sur le dernier élément. */
}
/*************************************************************************/
/* on ne fait entrer que les gens qui sont à l'étage actuel et qui veulent aller à l'étage cible */
void entrerDanslAscenseur(pile **p,int etageactuel,int etagecible,int capacite) /* **p car modifié */
{
    /* Fait entrer dans l'ascenseur autant de personnes en attente à l'étage atteint qu'il y a de place */
    pile *ptr;
    ptr = (*p);
    while((*p))
        {
            if (((*p)->pers_etagedep == etageactuel) && ((*p)->pers_etat==ETAT_ENATTENTE))
            {
                (*p)->pers_etat=ETAT_DANSASCENSEUR;
                (*p)->pers_etageactu = etageactuel;
            }
            *p = (*p)->prec;
        }
    *p = ptr; /* Le pointeur pointe tjrs sur le dernier élément. */
}
/*************************************************************************/
void MonterAveclAscenseur(pile **p,int etageascenceur) /* **p car modifié */
{
    /* Fait entrer dans l'ascenseur autant de personnes en attente à l'étage atteint qu'il y a de place */
    pile *ptr;
    ptr = (*p);
    while(*p)
        {
            if ((*p)->pers_etat==ETAT_DANSASCENSEUR)
            {
                (*p)->pers_etageactu = etageascenceur;
            }
            *p = (*p)->prec;
        }
    *p = ptr; /* Le pointeur pointe sur le dernier élément. */
}




/******     FONCTIONS DE CREATION ASCENCEUR ET IMMEUBLE    *******/
/*************************************************************************/
Ascenseur* creerAscenseur(int capacite, int etage_actuel)
{
    /* Crée un pointeur pointant vers un objet ascenseur et par lui attribue à l'ascenseur sa capacité, son étage, initialise sa destination à l'étage où il se trouve
       vérifie que la liste de personnes pointée par le dernier argument n'est pas trop longue par rappor à la capacité de l'ascenseur, et lui attribue ce pointeur le cas échéant, comme pointeur pointant vers la liste des personnes transportées par l'ascenseur */
    Ascenseur *ascenseur;
    ascenseur = (Ascenseur*)malloc(sizeof(Ascenseur));
    ascenseur->capacite = capacite;
    ascenseur->etageActuel = etage_actuel;
    ascenseur->etageDest = -1;
    ascenseur->porte = PORTE_FERMEE;  
    return ascenseur;
}

/*************************************************************************/
Immeuble* creerImmeuble(int nbre_etages, Ascenseur *ascenseur){
    /* Crée un pointeur pointant vers un objet de type Immeuble et par lui attribue à l'objet son nombre d'étages, le pointeur pointant vers son ascenseur et le pointeur correspondant au tableau des pointeurs pointants vers les listes des personnes en attente à chaque étage */
    Immeuble *immeuble;
    immeuble = (Immeuble*)malloc(sizeof(Immeuble));
    immeuble -> nbredEtages = nbre_etages;
    immeuble -> ascenseur = ascenseur;
    return immeuble;
}

#endif