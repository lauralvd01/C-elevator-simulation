#ifndef DISPLAY_LIB
#define DISPLAY_LIB

#include "structures.h"

#include<unistd.h>
#include<ncurses.h>

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>

/******     FONCTIONS DE GESTION DE L'AFFICHAGE    *******/
/*************************************************************************/
enum mode displayEcranIntroduction(WINDOW *fenetre)
{
    wattron(fenetre,A_UNDERLINE);
    int  i =(COLS / 2) - 50;
    mvwprintw(fenetre,1, i+5,"SIMULATION D'UN ASCENSEUR AVEC ANIMATION TEMPORELLE");
    wattroff(fenetre,A_UNDERLINE);
    mvwprintw(fenetre,3,i,"Le programme simule le comportement d'un ascenceur");
    mvwprintw(fenetre,4,i,"Toutes les minutes, de nouvelles personnes arrivent.");
    mvwprintw(fenetre,5,i,"Elles attendent l'ascenseur depuis leur étage.");
    mvwprintw(fenetre,6,i,"Saisir l'étage à laquelle vous souhaitez que l'ascenseur s'arrête");
    mvwprintw(fenetre,7,i,"Le programme réalise les étapes suivantes:");
    mvwprintw(fenetre,8,i,"- il ammène l ascenceur à l étage sélectionné");
    mvwprintw(fenetre,9,i,"- il fait descendre les personnes concernées");
    mvwprintw(fenetre,10,i,"- il fait rentrer les nouveaux arrivants");
    mvwprintw(fenetre,11,i,"Le programme attend une nouvelle saisie de l'étage.");
    mvwprintw(fenetre,LINES-2,1,"tapez <a> pour lancer la simulation en mode automatique* ou <m> pour le mode de saisie manuelle de l'étage"); 
    mvwprintw(fenetre,LINES-1,1,"(*) l'automate déplace l'ascenceur par ordre d'arrivée des occupants.");   
    wrefresh(fenetre);
    int input = wgetch(fenetre);
    return ((input=='a') ? MODE_AUTO : MODE_MANUEL);

}
/*************************************************************************/
void displayHeureActuelle(WINDOW *fenetre, int heure,int nb)
{
    char *st = nb>1 ? "s":"";
    mvwprintw(fenetre,LINES-3,COLS/2-20,"Il est 8 h%d mn, arrivée de %d nouvelle%s personne%s.          ",heure,nb,st,st);
    wrefresh(fenetre);
}
/*************************************************************************/
void displayAfficheLigneCommandes(WINDOW *fenetre)
{
    wattron(fenetre,A_ITALIC);
    mvwprintw(fenetre,LINES-1,1,"Entrer un n° d'étage (i pour infos, q pour quitter):");
    wattroff(fenetre,A_ITALIC);
    wrefresh(fenetre);
}
/*************************************************************************/
void displayCacheLigneCommandes(WINDOW *fenetre)
{
    wattron(fenetre,A_ITALIC);
    /* on efface la ligne */
    wmove(fenetre,LINES-1,1); wclrtoeol(fenetre);
    wattroff(fenetre,A_ITALIC);
    wrefresh(fenetre);
}

/*************************************************************************/
void displayScene(WINDOW *fenetre,Immeuble *immeuble, pile *pinit)
{
    pile *p;
    int etage_sup = immeuble->nbredEtages -1;
    int demi_largeurAscenseur = (immeuble->ascenseur->capacite)*3;
    int bord_gauche = (COLS / 2) -2 -demi_largeurAscenseur;
    int bord_droit = (COLS / 2) + demi_largeurAscenseur;

    char *msg = "En attente   N° étage |";
    int taille = (int)strlen(msg)-2;
    wattron(fenetre,A_BOLD);
    mvwprintw(fenetre, 3, bord_gauche - taille+1,msg);
    mvwprintw(fenetre, 3, bord_droit,"|  Sortants");
    wattroff(fenetre,A_BOLD);
    
    /* on parcourt tous les étages */
    for(int i=0;i<=etage_sup;i++) 
    {
        int dxEnAttente = 0,dxInAscenseur=0,dxSorti=0;
        int di=4+etage_sup-i; /* calcul ligne à l'écran */
        
        /* on efface la ligne */
        wmove(fenetre,di,30); wclrtoeol(fenetre);
        
        /* on affiche les bords, le nun étage et l'ascenseur si il est à cet étage */
        mvwprintw(fenetre,di,bord_gauche-5,"%d",i);
        mvwprintw(fenetre,di,bord_gauche,"|");
        mvwprintw(fenetre,di,bord_droit,"|");
        if (immeuble->ascenseur->etageActuel==i)
        {
            mvwprintw(fenetre,di,(COLS/2)-11,"[");
            mvwprintw(fenetre,di,(COLS/2)+10,"]");
        }

        /* on reparcours (du début) la liste des personnes pour afficher celles qui sont à cette étage */
        p = pinit;
        while(p)
        {
            /* la personne de la pile est elle à cet étage ? */
            if (p->pers_etageactu==i)
            {
                switch (p->pers_etat)
                {
                    case ETAT_ENATTENTE: 
                        mvwprintw(fenetre,di,bord_gauche-16+dxEnAttente,"(%d%d)",p->pers_etagedep,p->pers_etagedest);
                        dxEnAttente-=4; /* justification par la droite */
                        break;
                    case ETAT_DANSASCENSEUR:                      
                        mvwprintw(fenetre,di,bord_gauche+4+dxInAscenseur,"(%d%d)",p->pers_etagedep,p->pers_etagedest);
                        dxInAscenseur+=4; /* justification cenrtrée */
                        break;
                    case ETAT_SORTI:
                        mvwprintw(fenetre,di,bord_droit+3+dxSorti,"(%d%d)",p->pers_etagedep,p->pers_etagedest);
                        dxSorti+=4; /* justification par la gauche */
                        break;
                    default:
                        // on fait rien pour l'instant */
                        break;
                }
            }      
            p = p->prec;
        }
    }
    wrefresh(fenetre);
}

/*************************************************************************/
void animAscenceur(WINDOW *fenetre, Immeuble *immeuble, enum etatporte action)
{
    int etage_sup = immeuble->nbredEtages -1;
    int demi_largeurAscenseur = (immeuble->ascenseur->capacite)*3;
    int bord_gauche = (COLS / 2) -2 -demi_largeurAscenseur;
    int bord_droit = (COLS / 2) + demi_largeurAscenseur;
    int y=3+4+etage_sup; /* calcul ligne à l'écran */
    mvwprintw(fenetre,y-1,(COLS/2)-4,"[  %d  ]  ",immeuble->ascenseur->etageActuel);
    switch (action)
    {
        case PORTE_SOUVRE :
        case PORTE_OUVERTE :
            for (int i=0;i<demi_largeurAscenseur;i++) 
            {
                mvwprintw(fenetre,y,(COLS/2)-i," ");
                mvwprintw(fenetre,y,(COLS/2)+i-1," ");
                if (action==PORTE_SOUVRE)
                {
                    wrefresh(fenetre);
                    msleep(100);
                }
            }
            break;
        case PORTE_SEFERME :
        case PORTE_FERMEE :
            for (int i=demi_largeurAscenseur-1;i>=0;i--) 
            {
                mvwprintw(fenetre,y,(COLS/2)-i,"H");
                mvwprintw(fenetre,y,(COLS/2)+i-1,"H");
                if (action==PORTE_SEFERME)
                {
                    wrefresh(fenetre);
                    msleep(100);
                }
            }
            wrefresh(fenetre);
            break;
        
    }
}
/*************************************************************************/

#endif