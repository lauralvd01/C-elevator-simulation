#include <time.h>
#include <stdlib.h>
#include <ncurses.h>

#include "test1elevator.h"
#include "test1person.h"

#define HEIGHT 30
#define WIDTH 40
#define PERSON_WIDTH 3

void DisplayPersonList(WINDOW *win, PersonList *list, int level, int offset) {
  while(list != NULL) {
    // display 25 for a person going from floor 2 to floor 5
    mvwaddch(win, level, offset, '0' + list->person->src);
    mvwaddch(win, level, offset+1, '0' + list->person->dest);
    list = list->next;
    offset+= PERSON_WIDTH;
  }
}

void DisplayElevator(WINDOW *win, int nbFloor, Elevator *e, int offset) {
  //Display elevator
  // [23 24 31 30 42]
 
  int level = 3*(nbFloor - e->currentFloor); // 3 lines per level
  mvwaddch(win, level, offset+1, '[');
  DisplayPersonList(win, e->persons, level, offset+2);
  mvwaddch(win, level, offset+2+ (PERSON_WIDTH*e->capacity), ']');
}

void DisplayBuilding(WINDOW *win, Building *b) {
  int offset = 1;

  // display wall
  // |                |
  // |[23 24 31 30 42]| 31 32
  // |                |
  int right_wall = offset + 3 + (PERSON_WIDTH*b->elevator->capacity);
  for(int i=0; i < b->nbFloor; ++i) {
    int level = 3*i+1;
    mvwaddch(win,level,  offset,'|');
    mvwaddch(win,level+1,offset,'|');
    mvwaddch(win,level,  right_wall,'|');
    mvwaddch(win,level+1,right_wall,'|');
  }
  for(int i=offset+1; i < right_wall; i++) {
    mvwaddch(win,3*(b->nbFloor)+1,i,'_');
  }

  DisplayElevator(win, b->nbFloor, b->elevator, offset);

  for(int i=0; i < b->nbFloor; i++) {
    int level = 3*(b->nbFloor - i);
    DisplayPersonList(win,b->waitingLists[i], level, right_wall + 2);
  }
}

void stepElevator(Building *building) {
    // Simule le déplacement de l'ascenseur :
    // Si l'étage courant correspond à sa destination, faire en sorte que les personnes entrent et sortent de la chaîne : appeler
    exitElevator(building->elevator);
    enterElevator(building->elevator);
    // Sinon, l'étage courant est incrémenté ou décrémenté de 1 pour se rapprocher de sa destination
}

PersonList* exitElevator(Elevator *e){
    // Lorsque l'ascenseur arrive à un étage, cette fonction est appelée par stepElevator
    // Elle renvoie la liste des personnes qui sortent de l'ascenseur
    // (Celles qui ne sont pas sorties restent dans l'ascenseur)
    PersonList *sortants;
    
    PersonList *observed_in_elevator = e->persons;
    while( observed_in_elevator->person != NULL ){
        if(observed_in_elevator->person->dest == e->currentFloor){
            // La destination de la personne observée (encore dans l'ascenseur) correspond à l'étage atteint
            // On l'enregistre dans la liste des sortants :
            sortants = insert(observed_in_elevator->person,sortants);
        }
        // Puis on observe la personne suivante :
        observed_in_elevator->person = observed_in_elevator->next->person;
        observed_in_elevator->next = observed_in_elevator->next->next;
    }
    
    return sortants;

}


int main() {
  srand(time(NULL));   // should only be called once

  // generate list of waiting persons
  int nbFloor = 5;
PersonList **waitingLists = malloc(nbFloor*sizeof(PersonList*));
  for(int currentFloor=0; currentFloor < nbFloor; currentFloor++) {
    waitingLists[currentFloor] = NULL;
    int nbPerson = 5; // 5 persons in the waiting list
    for(int j=0 ; j<nbPerson ; j++) {
      int dest = rand() % (nbFloor);
      Person *p = createPerson(currentFloor, dest);
      waitingLists[currentFloor] = insert(p,waitingLists[currentFloor]);
    }
  }

  // Initialize building and elevator
  int capacity = 3;
  int currentFloor = 0;
  Elevator *elevator = create_elevator(capacity, currentFloor , NULL);
  Building *building = create_building(nbFloor, elevator, waitingLists);

  // Initialize ncurse display
  initscr(); // initialize ncurses
  noecho();  // do not display in window the pressed keys
  halfdelay(2);

  WINDOW *win = newwin(HEIGHT, WIDTH, 0, 0);

  // Animation loop
  bool run=true;
  while(run) {
    // Generate people in function of input (or quit if 'q')
    int input = wgetch(win);
    if(input == 'q') {
      run = false;
    } else {
      int level = input - '0';
      if(0 <= level && level < nbFloor) {
    building->elevator->targetFloor = level;
      }
    }

    // Update state machine of elevator !!!!

    stepElevator(building);

    wclear(win);   // clear display area
    box(win, 0,0); // display border of window

    DisplayBuilding(win, building);

    wrefresh(win); // actual display function

  }

  endwin(); // correct ending of ncurses

  return 0;
}

