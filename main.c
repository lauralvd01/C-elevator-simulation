#include "structures.h"

#include<unistd.h>
#include<ncurses.h>

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>

/*************************************************************************/
/*                                 MAIN                                  */
/*************************************************************************/

/** Déclaration des pointeurs **/
Ascenseur *ptr_ascenseur;
/* tableau de pointeurs Liste de personnes pour chaque étage */
Immeuble *ptr_immeuble;

int main() 
{
    /** Création du pointeur signalant la fin d'une liste **/
    pile *PilePers = NULL;
    enum mode modeanim;

    /*** INITIALISATION ***/
    /* tableau des personnes arrivants pour prendre l'ascenseur pour chaque personne [1 ligne][heurearrivee,etagedepart,etagearrivee] */
    /* si fin du tableau => alors {-1,-1,-1} */
    /* L'utilité dans le tableau du paramètre heure est que l'on peut pour la même heure ajouter plusieurs personnes */
    int tabPersArrivants[NOMBRE_MAX_ARRIVANTS][3] = { {0,0,2}, {0,4,0}, {1,2,0}, {2,3,0}, {3,0,7}, {4,6,0},
     {5,0,5}, {5,0,5}, {6,8,0}, {7,2,0}, {7,0,4}, {8,5,0},
     {8,0,3}, {9,4,0}, {10,2,0}, {10,6,0}, {11,3,0}, {-1,-1,-1}};
    int nbreTotArrivants=0;
    for (int i=0;i<NOMBRE_MAX_ARRIVANTS && tabPersArrivants[i][0]>-1;i++)
        nbreTotArrivants++;
    
    ptr_ascenseur = creerAscenseur(CAPACITE_MAX,DEPART); /* L'ascenseur est initialisé sans personne dedans */
    ptr_immeuble = creerImmeuble(NBREDETAGES,ptr_ascenseur);

    /*** INITIALISATION DE L'AFFICHAGE ***/
    initscr(); /* initialise ncurses avec ses autres parametres dont LINES et COLS*/
    noecho();  /* pour ne pas afficher les touches du clavier appyuées */
    

    WINDOW *fenetre = newwin(LINES, COLS, 0, 0);

    modeanim = displayEcranIntroduction(fenetre);
    
    /* nouvelle fenetre pour afficher la sequence */
    wclear(fenetre);
    wattron(fenetre,A_UNDERLINE);
    mvwprintw(fenetre,1, (COLS / 2) - 13,"SIMULATION D'UN ASCENSEUR");
    wattroff(fenetre,A_UNDERLINE);
    wrefresh(fenetre);
    sleep(1);

    int heureActuelle=0;
    bool showinfo = TRUE;

    /*** LANCEMENT DE L'AUTOMATE ***/
    while(heureActuelle<HEURE_MAX)
    {
        int nb=0;
        int etageCible;
        /* gestion des arrivants en fonction de l'heure*/
        /* on ajoute dans la pile que les gens si l'heure actuelle est leur heure d'arrivée */
        mvwprintw(fenetre,LINES-3,COLS/2-5,"                          ");  
        wrefresh(fenetre);
        nb=0;
        for (int i=0;i<nbreTotArrivants;i++) /* tableau limité au nombre exact d'arrivants */
        {
            int heureArrivant = tabPersArrivants[i][0];
            // 
            if (heureArrivant==heureActuelle) 
            {
                /* on ajoute à la pile le nouvel arrivant son état devient : EN ATTENTE */
                Push(&PilePers,tabPersArrivants[i][1],tabPersArrivants[i][2],heureArrivant,ETAT_ENATTENTE);
                nb++;
            }
        }
        if (showinfo) ViewW(fenetre,3,PilePers);
        displayHeureActuelle(fenetre,heureActuelle,nb);
        animAscenceur(fenetre, ptr_immeuble, PORTE_FERMEE);
        displayScene(fenetre,ptr_immeuble,PilePers);
        wrefresh(fenetre);
        sleep(1);
        

        if (modeanim==MODE_MANUEL)
        {
            displayAfficheLigneCommandes(fenetre);
            int input = wgetch(fenetre);
            displayCacheLigneCommandes(fenetre);

            if(input=='i') showinfo = !showinfo; 
            if(input == 'q') break;
        
            int etageSaisi = input - '0'; /*input envoi le code du caractère ascii moins le code caractère ascii'0' cela se traduit par le bon chiffre */  
            /* on vérifie que la saisie clavier correspond bien à un étage */
            if((0 <= etageSaisi) & (etageSaisi < NBREDETAGES))
                etageCible = etageSaisi;
        }
        else /* MODE_AUTO */
        {
            /* On recherche dans la pile la plus ancienne demande premier arrivé de la liste en attente */
            pile *p;
            p = PilePers;
            int n=-1;
int tmpcpt=0,tmpi=0;
            while(p)
            {
                /* on identifie le nouvel étage de la 1ere demande */
                if ((p->pers_etat==ETAT_ENATTENTE || p->pers_etat==ETAT_DANSASCENSEUR) && p->pers_etagedest!=ptr_ascenseur->etageActuel)
                {
                    n=p->pers_etagedest;
tmpcpt++;
mvwprintw(fenetre,LINES-1,0,"DEBUG[%d]:",tmpcpt);
mvwprintw(fenetre,LINES-1,tmpi*8+10,"i:%d,n:%d",tmpi,n);
wrefresh(fenetre);
                }
tmpi++;
                p = p->prec;
            }
wrefresh(fenetre);
sleep(5);
            etageCible = n;
            /* si etageCible=-1 c'est qu'il n'y a plus de personne en attente */
        }   

        if((0 <= etageCible) & (etageCible < NBREDETAGES))
        {
            
            animAscenceur(fenetre, ptr_immeuble, PORTE_SOUVRE);
            ptr_ascenseur->porte = PORTE_OUVERTE;
            /* ALGO ACTUEL : on ne fait rentrer dans l'ascenceur que les personnes qui vont à l'étage cible */
            entrerDanslAscenseur(&PilePers,ptr_ascenseur->etageActuel,etageCible,ptr_ascenseur->capacite);
            displayScene(fenetre,ptr_immeuble,PilePers);
            sleep(1);
            animAscenceur(fenetre, ptr_immeuble, PORTE_SEFERME);
            ptr_ascenseur->porte = PORTE_FERMEE;

            /** Démarrage de l'ascenseur **/
            ptr_ascenseur->etageDest = etageCible;
            int sens = (ptr_ascenseur->etageActuel < etageCible) ? +1 : -1;
            while(ptr_ascenseur->etageActuel != ptr_ascenseur->etageDest)
            { /* Transport de l'ascenseur jusqu'à la destination (pas d'arrêt aux étages entre temps) */
                ptr_ascenseur->etageActuel = ptr_ascenseur->etageActuel + sens;
                MonterAveclAscenseur(&PilePers,ptr_ascenseur->etageActuel);
                animAscenceur(fenetre, ptr_immeuble, PORTE_FERMEE);
                displayScene(fenetre,ptr_immeuble,PilePers);
                sleep(1);
            }
            animAscenceur(fenetre, ptr_immeuble, PORTE_SOUVRE);
            ptr_ascenseur->porte = PORTE_OUVERTE;
            /* ALGO ACTUEL : on ne fait rentrer dans l'ascenceur que les personnes qui vont à l'étage cible */
            sortirDelAscenseur(&PilePers,etageCible);
            displayScene(fenetre,ptr_immeuble,PilePers);
            sleep(1);
            entrerDanslAscenseur(&PilePers,ptr_ascenseur->etageActuel,etageCible,ptr_ascenseur->capacite);
            displayScene(fenetre,ptr_immeuble,PilePers);
            sleep(1);
            animAscenceur(fenetre, ptr_immeuble, PORTE_SEFERME);
            ptr_ascenseur->porte = PORTE_FERMEE;
        }

        /* increment de l'heure */
        heureActuelle++;
        sleep(2);
    }

    wclear(fenetre);
    wattron(fenetre,A_UNDERLINE);
    mvwprintw(fenetre,2, (COLS / 2) - 11,"SIMULATION TERMINEE");
    wattroff(fenetre,A_UNDERLINE);
    wrefresh(fenetre);
    sleep(2);
    wclear(fenetre);
    clear();

    endwin(); /* Termine correctement l'utilisation de ncurses */

    /** NETTOYAGE DE LA MEMOIRE **/

     Clear(&PilePers);

    free(ptr_immeuble->ascenseur);
    free(ptr_immeuble);
}