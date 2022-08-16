/** Tutoriels pour utiliser Ncurses **/

#include<ncurses.h>
/* #include<stdio.h> */ /* stdio.h est inclus dans ncurses.h */

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
    mvprintw(LINES + 1, COLS + 1, ".");
    getch();
    endwin();

    return 0;
}
