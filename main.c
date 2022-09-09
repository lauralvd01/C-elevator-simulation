#include "structures.h"

#include<unistd.h>
#include<ncurses.h>

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>

#define NBREDETAGES 5 /* NBREDETAGES = 5 => 4 étages et 1 rez-de-chaussée */
#define CAPACITE_MAX 4 /* Capatié de l'ascenseur */
#define DEPART 0 /* Point dde départ de l'ascenseur */
#define NOMBRE_MAX_ARRIVANTS 10
#define HEURE_MAX 100

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
    while(p)
        {
            mvwprintw(fenetre,i,0,">pers[dep=%d,dest=%d,actu=%d][etat=%d][Hdep=%d]\n",p->pers_etagedep,p->pers_etagedest,p->pers_etageactu,p->pers_etat,p->pers_heuredep);
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
    //pile *tmp = *p;
    while(*p)
        {
            if (((*p)->pers_etageactu == etagesorti) && ((*p)->pers_etat==ETAT_DANSASCENSEUR))
            {
                (*p)->pers_etat=ETAT_SORTI;
            }
            *p = (*p)->prec;
        }
    //*p = *tmp; /* Le pointeur pointe sur le dernier élément. */
}
/*************************************************************************/
/* on ne fait entrer que les gens qui sont à l'étage actuel et qui veulent aller à l'étage cible */
void entrerDanslAscenseur(WINDOW *f,pile **p,int etageactuel,int etagecible,int capacite) /* **p car modifié */
{
    /* Fait entrer dans l'ascenseur autant de personnes en attente à l'étage atteint qu'il y a de place */
    pile *ptr;
    ptr = (*p);

wrefresh(f);
    while((*p))
        {
mvwprintw(f,0,80,"tentrerdsascenseur vers cible %d et etat (%d)",etagecible,(*p)->pers_etat);  
wrefresh(f);
            if (((*p)->pers_etagedep == etageactuel && (*p)->pers_etagedest == etagecible) && ((*p)->pers_etat==ETAT_ENATTENTE))
            {
                (*p)->pers_etat=ETAT_DANSASCENSEUR;
                (*p)->pers_etageactu = etageactuel;
            }
            *p = (*p)->prec;
        }
        *p = ptr;
    /* Le pointeur pointe sur le dernier élément. */
}
/*************************************************************************/
void MonterAveclAscenseur(pile **p,int etageascenceur) /* **p car modifié */
{
    /* Fait entrer dans l'ascenseur autant de personnes en attente à l'étage atteint qu'il y a de place */
    //pile *tmp = *p;
    while(*p)
        {
            if ((*p)->pers_etat==ETAT_DANSASCENSEUR)
            {
                (*p)->pers_etageactu = etageascenceur;
            }
            *p = (*p)->prec;
        }
    //*p = *tmp; /* Le pointeur pointe sur le dernier élément. */
}

/******     FONCTIONS DE GESTION DE L'AFFICHAGE    *******/
/*************************************************************************/
void displayEcranIntroduction(WINDOW *fenetre)
{
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
}
/*************************************************************************/
void displayHeureActuelle(WINDOW *fenetre, int heure)
{
    mvwprintw(fenetre,LINES-4,COLS/2-3,"HEURE: %i",heure);
    wrefresh(fenetre);
}
/*************************************************************************/
void displayAfficheLigneCommandes(WINDOW *fenetre)
{
    wattron(fenetre,A_ITALIC);
    mvwprintw(fenetre,LINES-1,1,"Entrer un n° d'étage (q pour quitter):");
    wattroff(fenetre,A_ITALIC);
    wrefresh(fenetre);
}
/*************************************************************************/
void displayCacheLigneCommandes(WINDOW *fenetre)
{
    wattron(fenetre,A_ITALIC);
    mvwprintw(fenetre,LINES-1,1,"                                        ");
    wattroff(fenetre,A_ITALIC);
    wrefresh(fenetre);
}
/*************************************************************************/
void displayListeDePersonnes(WINDOW *fenetre,int etage,int colomne,pile* p,enum etat etatpers)
{
    while(p)
    {
        if (p->pers_etat==etatpers && p->pers_etageactu==etage)
        {
            /* Affiche la première personne de la liste telle que (12) si 1 est son départ et 2 son arrivée */
            mvwprintw(fenetre,etage,colomne,"(%d%d)",p->pers_etagedep,p->pers_etagedest);
            /* On poursuit en affichant la suite de la liste, jusqu'à la fin */
            colomne = colomne + 4;
        }
        p = p->prec;
    }
}
/*************************************************************************/
void displayAscenseur(WINDOW *fenetre,Immeuble *immeuble,pile *p){

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
    displayListeDePersonnes(fenetre,niveau,(COLS/2)-9,p,ETAT_DANSASCENSEUR);
    if(etage_actuel == immeuble->ascenseur->etageDest){
        mvwprintw(fenetre,niveau,(COLS/2)+8,"[");
    }
    else{
        mvwprintw(fenetre,niveau,(COLS/2)+8,"]");
    }
}
/*************************************************************************/
void displayImmeuble(WINDOW *fenetre,Immeuble *immeuble, pile *p)
{
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
        mvwprintw(fenetre,4+i,bord_droit,"|                         ");

        displayListeDePersonnes(fenetre,4+i,bord_droit+2,p,ETAT_ENATTENTE);
    }

    displayAscenseur(fenetre,immeuble,p);
}

/******     FONCTIONS DE CREATION ASCENCEUR ET IMMEUBLE    *******/
/*************************************************************************/
Ascenseur* creerAscenseur(int capacite, int etage_actuel)
{
    /* Crée un pointeur pointant vers un objet ascenseur et par lui attribue à l'ascenseur sa capacité, son étage, initialise sa destination à l'étage où il se trouve
       vérifie que la liste de personnes pointée par le dernier argument n'est pas trop longue par rappor à la capacité de l'ascenseur, et lui attribue ce pointeur le cas échéant, comme pointeur pointant vers la liste des personnes transportées par l'ascenseur */
    Ascenseur *ascenseur;
    ascenseur = (Ascenseur*)malloc(sizeof(Ascenseur));
    ascenseur -> capacite = capacite;
    ascenseur -> etageActuel = etage_actuel;
    ascenseur -> etageDest = -1;
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
    int tabPersArrivants[NOMBRE_MAX_ARRIVANTS][3] = { {0,0,2}, {0,4,0}, {1,2,0}, {2,3,0}, {3,3,2}, {-1,-1,-1} };
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

    displayImmeuble(fenetre,ptr_immeuble,PilePers);
    wrefresh(fenetre);
    sleep(1);   

    int heureActuelle=0; 

    /*** LANCEMENT DE L'AUTOMATE ***/
    while(heureActuelle<HEURE_MAX)
    {
        displayHeureActuelle(fenetre,heureActuelle);
        displayAfficheLigneCommandes(fenetre);
int input = wgetch(fenetre);
//int input = 2;
        displayCacheLigneCommandes(fenetre);
        if(input == 'q') break;
    
        /* gestion des arrivants en fonction de l'heure*/
        /* on ajoute dans la pile que les gens si l'heure actuelle est leur heure d'arrivée */
        mvwprintw(fenetre,LINES-3,COLS/2-5,"                          ");  
        wrefresh(fenetre);
        for (int i=0;i<nbreTotArrivants;i++) /* tableau limité au nombre exact d'arrivants */
        {
            int heureArrivant = tabPersArrivants[i][0];
            // 
            if (heureArrivant==heureActuelle) 
            {
                /* on ajoute à la pile le nouvel arrivant son état devient : EN ATTENTE */
                Push(&PilePers,tabPersArrivants[i][1],tabPersArrivants[i][2],heureArrivant,ETAT_ENATTENTE);
mvwprintw(fenetre,LINES-3,0,"Nouvel arrivant : %d -> %d ",tabPersArrivants[i][1],tabPersArrivants[i][2]);
wrefresh(fenetre);
            }
        }
        displayImmeuble(fenetre,ptr_immeuble,PilePers);
        wrefresh(fenetre);
mvwprintw(fenetre,0,0,"taille pile= %d     ",Length(PilePers)); 
wrefresh(fenetre);

        int etageCible = input - '0'; /*input envoi le code du caractère ascii moins le code caractère ascii'0' cela se traduit par le bon chiffre */  
        /* on vérifie que la saisie clavier correspond bien à un étage */
        if((0 <= etageCible) & (etageCible < NBREDETAGES)){
            /* ALGO ACTUEL : on ne fait rentrer dans l'ascenceur que les personnes qui vont à l'étage cible */
            entrerDanslAscenseur(fenetre,&PilePers,ptr_ascenseur->etageActuel,etageCible,ptr_ascenseur->capacite);
ViewW(fenetre,3,PilePers);
wrefresh(fenetre);
            /** Activation de l'ascenseur **/
            int taille_dest = (int)strlen("Destination : 0 ");
mvwprintw(fenetre,LINES-2,COLS-1-taille_dest,"Destination : %d ",etageCible);
wrefresh(fenetre);

            ptr_ascenseur->etageDest = etageCible;
            int sens = (ptr_ascenseur->etageActuel < etageCible) ? +1 : -1;
            while(ptr_ascenseur->etageActuel != ptr_ascenseur->etageDest)
            { /* Transport de l'ascenseur jusqu'à la destination (pas d'arrêt aux étages entre temps) */
                ptr_ascenseur->etageActuel = ptr_ascenseur->etageActuel + sens;
mvwprintw(fenetre,1,1,"etage actu asc=%d sens=%d",ptr_ascenseur->etageActuel,sens);
wrefresh(fenetre);
                
                //MonterAveclAscenseur(&PilePers,ptr_ascenseur->etageActuel);
ViewW(fenetre,10,PilePers);
                displayAscenseur(fenetre,ptr_immeuble,PilePers);
                wrefresh(fenetre);
                sleep(1);
            }
mvwprintw(fenetre,1,1,"etage actu asc=%d arrivé       ",ptr_ascenseur->etageActuel);
wrefresh(fenetre);

            displayAscenseur(fenetre,ptr_immeuble,PilePers);
            wrefresh(fenetre);
            sleep(1);

            //sortirDelAscenseur(&PilePers,etageCible);
            displayAscenseur(fenetre,ptr_immeuble,PilePers);
            wrefresh(fenetre);
            sleep(1);

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