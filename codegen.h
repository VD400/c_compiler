#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "tac.h"

void generate_code(TACList* list);

extern FILE* asm_file;

#endif
