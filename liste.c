#ifndef LISTE_LIB
#define LISTE_LIB

#include "structures.h"

#include<unistd.h>
#include<ncurses.h>

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>


/**************     FONCTION DE GESTION DE LA PILE DE PERSONNES    *******/
/*************************************************************************/
void Push(pile **p, int etagedep,int etagedest,int heuredep,enum etat etatinit)
{
    pile *element = malloc(sizeof(pile));
    if(!element) exit(EXIT_FAILURE);     /* Si l'allocation a échoué. */
    element->pers_etagedep = etagedep;
    element->pers_etageactu = etagedep; // idem dessus */
    element->pers_etagedest = etagedest;
    element->pers_heuredep = heuredep;
    element->pers_etat = etatinit;
    element->prec = *p;
    *p = element;       /* Le pointeur pointe sur le dernier élément. */
}
/*************************************************************************/
void Pop(pile **p)
{
    pile *tmp;
    if(!*p) exit;     /* Retourne -1 si la pile est vide. */
    tmp = (*p)->prec;
    free(*p);
    *p = tmp;       /* Le pointeur pointe sur le dernier élément. */
}
/*************************************************************************/
void Clear(pile **p)
{
    pile *tmp;
    while(*p)
        {
            tmp = (*p)->prec;
            free(*p);
            *p = tmp;
        }
}
/*************************************************************************/
int Length(pile *p)
{
    int n=0;
    while(p) {
            n++; 
            p = p->prec;
        }
    return n;
}
/*************************************************************************/
void View(pile *p)
{
        while(p)
          {
             printf("pile:pers[dep=%d,dest=%d,actu=%d][etat=%d][Hdep=%d]\n",p->pers_etagedep,p->pers_etagedest,p->pers_etageactu,p->pers_etat,p->pers_heuredep);
             
             p = p->prec;
          }
}
/*************************************************************************/
/*************************************************************************/
void ViewW(WINDOW *fenetre, int offset,pile *p)
{
    int i=offset;
    wclear(fenetre);
    wattron(fenetre,A_UNDERLINE);
    mvwprintw(fenetre,1, (COLS / 2) - 13,"SIMULATION D'UN ASCENSEUR");
    mvwprintw(fenetre,0, 1,"Nombre total de personnes: %d",Length(p));
    wattroff(fenetre,A_UNDERLINE);
    while(p)
        {
            mvwprintw(fenetre,i,0,">pers[8h:%d][%d => %d][P:%d][E:%d]",p->pers_heuredep,p->pers_etagedep,p->pers_etagedest,p->pers_etageactu,p->pers_etat);
            i++;
            p = p->prec;
        }
    wrefresh(fenetre);
}
/*************************************************************************/

#endif