#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <stdio.h>
#include "tac.h"



TACList* optimize_tac(TACList* list);

/* File pointer for dual stdout + file output — defined in main.c */
extern FILE* opt_file;

#endif /* OPTIMIZER_H */
