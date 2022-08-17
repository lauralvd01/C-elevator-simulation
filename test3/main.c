#include "structures.h"

#include<time.h>
#include<unistd.h>
#include<ncurses.h>

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>

#define NBREDETAGES 5 /* NBREDETAGES = 5 => 4 étages et 1 rez-de-chaussée */
#define CAPACITE 3 /* Capatié de l'ascenseur */
#define DEPART 0 /* Point dde départ de l'ascenseur */

/** Déclaration des pointeurs **/
ListeDePersonnes *ptr_fin;
Ascenseur *ptr_ascenseur;
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

        int nbre_personnes; /* Il y aura 6 personnes en attente à chaque étage */
        for(nbre_personnes = 1; nbre_personnes <= 6; nbre_personnes++){
            int destination = rand() % (NBREDETAGES-1);
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
    
    wrefresh(fenetre);
    getch();

    char *msg = "SIMULATION D'UN ASCENSEUR";
    int taille = (int)strlen(msg);
    attron(A_BOLD);
    mvwprintw(fenetre,1, (COLS / 2) - (taille / 2),msg);
    attroff(A_BOLD);
    wrefresh(fenetre);
    getch();
    sleep(2);
    /*displayImmeuble(ptr_immeuble);*/


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
                wclear(fenetre);
                char desti[20];
                sprintf(desti,"Destination : %d ",destination);
                int taille_dest = (int)strlen(desti);
                mvwprintw(fenetre,LINES -1,COLS -1-taille_dest,desti);
                /*displayImmeuble(ptr_immeuble);
                wrefresh(fenetre);*/

                ptr_ascenseur->destination = destination;
                /*while(ptr_ascenseur->etageActuel != ptr_ascenseur->destination){
                    transporter(ptr_ascenseur);
                }

                sortirDelAscenseur(ptr_ascenseur);

                ptr_immeuble->ascenseur = ptr_ascenseur;
                wclear(fenetre);
                displayImmeuble(ptr_immeuble);
                wrefresh(fenetre);

                entrerDanslAscenseur(ptr_immeuble,ptr_ascenseur);

                ptr_immeuble->ascenseur = ptr_ascenseur;
                wclear(fenetre);
                displayImmeuble(ptr_immeuble);
                wrefresh(fenetre);
                */
            }
        }
    }

    wclear(fenetre);
    char *msg_fin = "SIMULATION TERMINEE";
    int taille_fin = (int)strlen(msg_fin);
    attron(A_BOLD);
    mvwprintw(fenetre,1, (COLS / 2) - (taille_fin / 2),msg_fin);
    attroff(A_BOLD);
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
