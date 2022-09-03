#include "structures.h"

#include<unistd.h>
#include<ncurses.h>

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>

#define NBREDETAGES 5 /* NBREDETAGES = 5 => 4 étages et 1 rez-de-chaussée */
#define CAPACITE 4 /* Capatié de l'ascenseur */
#define DEPART 0 /* Point dde départ de l'ascenseur */

/** Déclaration des pointeurs **/
ListeDePersonnes *ptr_fin;
Ascenseur *ptr_ascenseur;
/* tableau de pointeurs Liste de personnes pour chaque étage */
ListeDePersonnes **ptr_enAttente;
Immeuble *ptr_immeuble;



int main() {
    /** Création du pointeur signalant la fin d'une liste **/
    ptr_fin = (ListeDePersonnes*)malloc(sizeof(ListeDePersonnes));
    ptr_fin->longueur = 0;
    ptr_fin->tete = NULL;
    ptr_fin->queue = NULL;
    /* Normalement équivalent à fin = insererTetePseronne(NULL,NULL); */


    /*** INITIALISATION ***/

    /** Remplissage du tableau des listes de personnes en attente **/
    ListeDePersonnes* enAttente[NBREDETAGES];
    int etage;
    for(etage=0;etage<NBREDETAGES;etage++){
        enAttente[etage] = (ListeDePersonnes*)malloc(sizeof(ListeDePersonnes)); /* Chaque élément du tableau enAttente est un pointeur vers une liste de personnes*/
        enAttente[etage] = insererPersonneListe(NULL,NULL);

        int nbre_personnes; /* Il y aura 5 personnes en attente à chaque étage */
        for(nbre_personnes = 1; nbre_personnes <= 5; nbre_personnes++){
            int destination = rand() % (NBREDETAGES);
            Personne *ptr_personne = creerPersonne(etage,destination);
            enAttente[etage] = insererPersonneListe(ptr_personne,enAttente[etage]);
        }
    }
    ptr_enAttente = (ListeDePersonnes**)malloc(sizeof(ListeDePersonnes*)*NBREDETAGES);
    ptr_enAttente = enAttente;

    ptr_ascenseur = creerAscenseur(CAPACITE,DEPART,ptr_fin); /* L'ascenseur est initialisé sans personne dedans */
    ptr_immeuble = creerImmeuble(NBREDETAGES,ptr_ascenseur,ptr_enAttente);


    /*** INITIALISATION DE L'AFFICHAGE ***/
    initscr(); /* initialise ncurses */
    noecho();  /* pour ne pas afficher les touches du clavier appyuées */
    sleep(2);

    WINDOW *fenetre = newwin(LINES, COLS, 0, 0);
    wattron(fenetre,A_UNDERLINE);
    mvwprintw(fenetre,1, (COLS / 2) - 13,"SIMULATION D'UN ASCENSEUR");
    wattroff(fenetre,A_UNDERLINE);
    mvwprintw(fenetre,3,10,"Le programme simule le comportement d'un ascenceur");
    mvwprintw(fenetre,4,10,"Une fois le chargement des personnes qui attendent l'ascenseur à chaque étage");
    mvwprintw(fenetre,5,10,"Saisir l'étage à laquelle vous souhaitez que l'ascenseur s'arrête");
    mvwprintw(fenetre,6,10,"Le programme réalise les étapes suivantes:");
    mvwprintw(fenetre,7,10,"- il ammène l ascenceur à l étage sélectionné");
    mvwprintw(fenetre,8,10,"- il fait descendre les personnes concernées");
    mvwprintw(fenetre,9,10,"- il fait rentrer les nouveaux arrivants");
    mvwprintw(fenetre,10,10,"Le programme attend une nouvelle saisie de l'étage.");
    mvwprintw(fenetre,LINES-1,1,"tapez <SPACE> pour lancer la simulation");
    
    wrefresh(fenetre);
    int input = wgetch(fenetre);
    /*getch();*/
    wclear(fenetre);

    wattron(fenetre,A_UNDERLINE);
    mvwprintw(fenetre,1, (COLS / 2) - 13,"SIMULATION D'UN ASCENSEUR");
    wattroff(fenetre,A_UNDERLINE);
    wrefresh(fenetre);
    sleep(1);

    displayImmeuble(fenetre,ptr_immeuble);
    wrefresh(fenetre);
    sleep(1);

    wattron(fenetre,A_ITALIC);
    mvwprintw(fenetre,LINES-2,1,"Entrer un n° d'étage");
    mvwprintw(fenetre,LINES-1,1,"Entrer q pour quitter");
    wattroff(fenetre,A_ITALIC);
    wrefresh(fenetre);


    /*** LANCEMENT DE L'AUTOMATE ***/
    bool run = true;
    while(run){
        int input = wgetch(fenetre);
        if(input == 'q'){
            run = false;
        }
        else{
            int destination = input - '0'; /*Sans le '0' cela ne traduit pas le bon chiffre */
            if((0 <= destination) & (destination < NBREDETAGES)){

                /** Activation de l'ascenseur **/
                
                int taille_dest = (int)strlen("Destination : 0 ");
                mvwprintw(fenetre,LINES-2,COLS-1-taille_dest,"Destination : %d ",destination);
                wrefresh(fenetre);

                ptr_ascenseur->destination = destination;
                while(ptr_ascenseur->etageActuel != ptr_ascenseur->destination){ /* Transport de l'ascenseur jusqu'à la destination (pas d'arrêt aux étages entre temps) */
                    
                    if(ptr_ascenseur->etageActuel < destination){
                        ptr_ascenseur->etageActuel += 1;
                    }
                    else{
                        ptr_ascenseur->etageActuel -= 1;
                    }
                    displayAscenseur(fenetre,ptr_immeuble);
                    wrefresh(fenetre);
                    sleep(1);
                }
                displayAscenseur(fenetre,ptr_immeuble);
                wrefresh(fenetre);
                sleep(1);

                sortirDelAscenseur(ptr_immeuble);
                displayAscenseur(fenetre,ptr_immeuble);
                wrefresh(fenetre);
                sleep(2);

                entrerDanslAscenseur(fenetre,ptr_immeuble);
                displayImmeuble(fenetre,ptr_immeuble);
                wrefresh(fenetre);
            }
        }
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
    free(ptr_immeuble->ascenseur);
    free(ptr_immeuble);
    free(ptr_fin);

    return 0;
}