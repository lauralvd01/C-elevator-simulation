#include "structures.h"
#include<unistd.h>

#include<stddef.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
#include<ncurses.h>
#include<string.h>

#define NBREDETAGES 5 /* NBREDETAGES = 5 => 4 étages et 1 rez-de-chaussée */
#define CAPACITE 3 /* Capatié de l'ascenseur */
#define DEPART 0 /* Point dde départ de l'ascenseur */

/** Déclaration des pointeurs **/
ListeDePersonnes *ptr_fin;
Ascenseur *ptr_ascenseur;
ListeDePersonnes **ptr_enAttente;
ListeDePersonnes **ptr_satisfaits;
Immeuble *ptr_immeuble;



int main() {
    /** Déclaration du pointeur signalant la fin d'une liste **/
    ptr_fin = (ListeDePersonnes*)malloc(sizeof(ListeDePersonnes));
    ptr_fin->longueur = 0;
    ptr_fin->tete = NULL;
    ptr_fin->queue = NULL;
    /* Normalement équivalent à fin = insererTetePseronne(NULL,NULL); */


    /*** INITIALISATION ***/

    ListeDePersonnes* enAttente[NBREDETAGES];
    
    /** Remplissage du tableau des listes de personnes en attente **/
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

    ptr_ascenseur = creerAscenseur(CAPACITE,DEPART,ptr_fin);
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
    int stop = 1;
    while(stop == 0){
    }

    
    endwin(); /* Termine correctement l'utilisation de ncurses */

    /** NETTOYAGE DE LA MEMOIRE **/
    free(ptr_immeuble->ascenseur);
    free(ptr_immeuble);
    free(ptr_fin);

    return 0;
}
