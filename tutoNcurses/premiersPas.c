/** Tutoriels pour utiliser Ncurses **/

#include<ncurses.h>
/* #include<stdio.h> */ /* stdio.h est inclus dans ncurses.h */
#include <stdlib.h> /* au moins pour utiliser free() */
#include <string.h> /* ôur utiliser strlen */

int main(void){
    /** PREMIERE FENETRE HELLO WORLD **/
    initscr();              // Initialise la structure WINDOW et autres paramètres 
    printw("Hello World");  // Écrit Hello World à l'endroit où le curseur logique est positionné
    refresh();              // Rafraîchit la fenêtre courante afin de voir le message apparaître
    getch();                // On attend que l'utilisateur appui sur une touche pour quitter
    endwin();               // Restaure les paramètres par défaut du terminal
    
    /** SE POSITIONNER DANS LA FENETRE **/
    initscr();
    move(LINES - 1, COLS - 1);  // Déplace le curseur tout en bas à droite de l'écran
    addch('.');                 // Écrit le caractère . au nouvel emplacement
    /** est équivalent à : **/
    //mvaddch(LINES - 1, COLS - 1, '.');
    /** ou encore à : **/
    //mvprintw(LINES - 1, COLS - 1, ".");
    getch();
    mvprintw(1,1, ".."); 
    getch();
    endwin();
    /** LINES et COLS correspondent, lorsque pas définies, aux nombres de
     *  lignes et de colomnes, donc à des ENTIERS **/


    /** ECRIRE EN UTILISANT LES MISES EN FORME **/
    /* Mises en formes utilisables :
    
    Normal (A_NORMAL)
    Meilleur surlignement (A_STANDOUT)
    Inversion vidéo (A_REVERSE)
    À moitié brillant (A_DIM)
    Très brillant ou en gras (A_BOLD)
    Souligné (A_UNDERLINE)
    Mode invisible ou blanc (A_INVIS)

    --> appliquées dès la balise attron(§mise_en_forme§) et enlevée dès attrof(§mise_en_forme§)
    */
   initscr();

    attron(A_NORMAL);
    printw("\nTexte sans mise en forme\n");

    attron(A_STANDOUT);
    printw("Texte en meilleur surlignement\n");
    attroff(A_STANDOUT);
    
    attron(A_REVERSE);
    printw("Texte en inversion video\n"); /* Pas encore compris commenet ça marchait celui-là */
    attroff(A_REVERSE);

    attron(A_DIM);
    printw("Texte a moitie brillant\n");
    attroff(A_DIM);

    attron(A_BOLD);
    printw("Texte en gras\n");
    attroff(A_BOLD);

    attron(A_UNDERLINE);
    printw("Texte en souligne\n");
    attroff(A_UNDERLINE);

    attron(A_INVIS);
    printw("Texte invisible\n");
    attroff(A_INVIS);

    attron(A_UNDERLINE | A_BOLD); // Pour appliquer plusieurs type de mises en forme, on utilise l'opérateur unaire |
    printw("Texte en gras souligne\n");
    
    attron(A_NORMAL); /* Permet aussi d'annuler toutes les mises en forme appliquées */
    refresh();
    getch();
    endwin();
    /*
    Si vous désirez utiliser une mise en forme commune pour chaque 
    fenêtre et sous-fenêtre que vous créer (par exemple écrire en gras), 
    plutôt que d'utiliser attron à chaque fois que vous voulez écrire, 
    vous pouvez utiliser la fonction attrset avec votre liste d'attribut.
    Ainsi, n'importe qu'elle fenêtre que vous créerez héritera de cet 
    attribut et donc chaque fois que vous écrirez dedans, ça sera 
    automatiquement mis en gras. 
    (Reste encore à savoir ce qu'est un attribut x)
    */
   clear();

    /** LIRE UNE CHAINE DE CARACTERES A PARTIR DU CLAVIER **/
    /* Méthode utilisée jusqu'à maintenant : getch -> ? 
    Nouvelle méthode : getstr pour avoir des chaines de caractères entières directement
    */

   /** CREER PLUSIEURS FENETRES ou subdiviser une fenêtre **/
    WINDOW *haut, *bas;
    
    initscr();
    haut= subwin(stdscr, LINES / 2, COLS, 0, 0);        // Créé une fenêtre de 'LINES / 2' lignes et de COLS colonnes en 0, 0
    bas= subwin(stdscr, LINES / 2, COLS, LINES / 2, 0); // Créé la même fenêtre que ci-dessus sauf que les coordonnées changent
    
    box(haut, ACS_VLINE, ACS_HLINE);
    box(bas, ACS_VLINE, ACS_HLINE);
    
    mvwprintw(haut, 1, 1, "Ceci est la fenetre du haut");
    mvwprintw(bas, 1, 1, "Ceci est la fenetre du bas");
    
    wrefresh(haut);
    wrefresh(bas);
    
    getch();
    endwin();
    
    /*free(haut); Fait aborter le makefile et affiche double free or corruption(out) -> ?
    free(bas);*/
    

    /** CREER UNE BORDURE AUTOUR DE LA FENETRE **/
    /** Méthode box **/
    clear();
    WINDOW *boite;
    
    initscr();
    boite= subwin(stdscr, LINES / 2, COLS /2, 0, 0);
   
    box(boite, ACS_VLINE, ACS_HLINE); // ACS_VLINE et ACS_HLINE sont des constantes qui génèrent des bordures par défaut
    mvwprintw(boite, 1, 1, "Ceci est une boite");
    refresh();
   
    getch();
    clear();
    endwin();
    
    /*free(boite);*/

    /** Méthode wborder **/
    WINDOW *boite2;
    
    initscr();
    boite2= subwin(stdscr, 0, 0, LINES / 2, COLS /2);
    wborder(boite2, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(boite2);
    
    getch();
    endwin();
    
    /*free(boite2);*/

    /* ==> Fonctionnent mais attention aux nombres de colonnes et lignes signifiés ! Parfois ça a marché mais juste ça sort de l'écran ! */


    /** CENTRER LE TEXTE SUR LE TERMINAL **/
    WINDOW *boite3;

    initscr();
    printw("Le terminal actuel comporte %d lignes et %d colonnes\n", LINES, COLS);
    refresh();
    getch();
    endwin(); 
    
    /*free(boite3); */

    /* Attention ! 
    lorsque vous redimensionner le terminal, c'est comme si une touche 
    du clavier était appuyé. 
    ==> Pour palier ceci, on utilise une boucle : */
    WINDOW *boite4;
    
    initscr();
    while(1) {
        printw("Le terminal actuel comporte %d lignes et %d colonnes\n", LINES, COLS);
        refresh();  // Rafraîchit la fenêtre par défaut (stdscr) afin d'afficher le message
        if(getch() != 410)  // 410 est le code de la touche générée lorsqu'on redimensionne le terminal
            break;
    }
    
    endwin();
    
    /*free(boite4); */

    /** Maintenant venons au fait, pour centrer un texte, il suffit 
     * de partir du centre de la fenêtre, auquel on enlève la moitié de 
     * la longueur de notre texte. **/
    WINDOW *boite5;
    char *msg= "Texte au centre";
    int taille= strlen(msg);
    
    initscr();
    while(1) {
        clear();    // Efface la fenêtre (donc l'ancien message)
        mvprintw(LINES/2, (COLS / 2) - (taille / 2), msg);
        refresh();
        if(getch() != 410)
            break;
    }
    
    endwin();
    
    /*free(boite5); */
    return 0;
}
