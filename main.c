#include "structures.h"

#include<unistd.h>
#include<ncurses.h>

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>

#define NBREDETAGES 8 /* NBREDETAGES = 8 => 7 étages et 1 rez-de-chaussée */
#define CAPACITE_MAX 4 /* Capatié de l'ascenseur */
#define DEPART 0 /* Point dde départ de l'ascenseur */
#define NOMBRE_MAX_ARRIVANTS 30
#define HEURE_MAX 100

/**************     FONCTION DE GESTION DU TEMPS    *******/
int msleep(unsigned int tms) {
  return usleep(tms * 1000);
}


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
            mvwprintw(fenetre,i,0,">pers[8h:%d][%d => %d][et.:%d]",p->pers_heuredep,p->pers_etagedep,p->pers_etagedest,p->pers_etageactu);
            i++;
            p = p->prec;
        }
    wrefresh(fenetre);
}
/*************************************************************************/


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

/******     FONCTIONS DE GESTION DE L'AFFICHAGE    *******/
/*************************************************************************/
void displayEcranIntroduction(WINDOW *fenetre)
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
    mvwprintw(fenetre,LINES-1,1,"tapez <SPACE> pour lancer la simulation");   
    wrefresh(fenetre);
    int input = wgetch(fenetre);
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

    displayEcranIntroduction(fenetre);
    
    /* nouvelle fenetre pour afficher la sequence */
    wclear(fenetre);
    wattron(fenetre,A_UNDERLINE);
    mvwprintw(fenetre,1, (COLS / 2) - 13,"SIMULATION D'UN ASCENSEUR");
    wattroff(fenetre,A_UNDERLINE);
    wrefresh(fenetre);
    sleep(1);

    /*displayImmeuble(fenetre,ptr_immeuble,&PilePers);*/
    

    int heureActuelle=0;
    bool showinfo = FALSE;

    /*** LANCEMENT DE L'AUTOMATE ***/
    while(heureActuelle<HEURE_MAX)
    {
        int nb=0;
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
        displayAfficheLigneCommandes(fenetre);
        int input = wgetch(fenetre);
        displayCacheLigneCommandes(fenetre);
        if(input=='i') showinfo = !showinfo; 
        if(input == 'q') break;
        int etageCible = input - '0'; /*input envoi le code du caractère ascii moins le code caractère ascii'0' cela se traduit par le bon chiffre */  
        /* on vérifie que la saisie clavier correspond bien à un étage */
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