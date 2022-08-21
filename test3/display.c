#include "structures.h"

#include<ncurses.h>
#include<string.h>

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

void displayAscenseur(WINDOW *fenetre,Immeuble *immeuble){

    /** NETTOYAGE DE L ESPACE DEDIE A L ASCENSEUR **/
    for(int i=0;i<immeuble->nbredEtages;i++){
        mvwprintw(fenetre,4+i,(COLS/2)-12,"                      ");
    }
    wrefresh(fenetre);

    /** AFFICHAGE DE L ASCENSEUR ACTUALISE **/
    int etage_sup = immeuble->nbredEtages -1;
    int etage_actuel = immeuble->ascenseur->etageActuel;
    int niveau = 4+etage_sup-etage_actuel;

    mvwprintw(fenetre,niveau,(COLS/2)-11,"[");
    displayListeDePersonnes(fenetre,niveau,(COLS/2)-9,immeuble->ascenseur->transportes);
    if(etage_actuel == immeuble->ascenseur->destination){
        mvwprintw(fenetre,niveau,(COLS/2)+8,"[");
    }
    else{
        mvwprintw(fenetre,niveau,(COLS/2)+8,"]");
    }

    return;
}

void displayImmeuble(WINDOW *fenetre,Immeuble *immeuble){

    int etage_sup = immeuble->nbredEtages -1;
    int demi_largeurAscenseur = (immeuble->ascenseur->capacite)*3;
    int bord_gauche = (COLS / 2) -2 -demi_largeurAscenseur;
    int bord_droit = (COLS / 2) + demi_largeurAscenseur;


    char *msg = "N° d'étage  |";
    int taille = (int)strlen(msg)-2;
    wattron(fenetre,A_BOLD);
    mvwprintw(fenetre, 3, bord_gauche - taille+1,msg);
    mvwprintw(fenetre, 3, bord_droit,"|  En attente\n");
    wattroff(fenetre,A_BOLD);
    
    for(int i=0;i<=etage_sup;i++){
        mvwprintw(fenetre,4+i,bord_gauche-(taille-3)/2,"%d",etage_sup-i);

        mvwprintw(fenetre,4+i,bord_gauche,"|");
        mvwprintw(fenetre,4+i,bord_droit,"|");

        displayListeDePersonnes(fenetre,4+i,bord_droit+2,immeuble->enAttente[etage_sup-i]);
    }

    displayAscenseur(fenetre,immeuble);

    return;
}