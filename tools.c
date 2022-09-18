#ifndef TOOLS_LIB
#define TOOLS_LIB

#include "structures.h"

#include<unistd.h>
#include<ncurses.h>

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<assert.h>



/**************     FONCTION DE GESTION DU TEMPS    *******/
int msleep(unsigned int tms) {
  return usleep(tms * 1000);
}

#endif