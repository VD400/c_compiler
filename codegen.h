#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "tac.h"

/* -----------------------------------------------------------------------
 * x86-style Assembly Code Generator
 *
 * Translates the optimized TAC instruction list into x86 assembly.
 * The output follows a readable AT&T-style syntax and targets a simple
 * flat register model with a software stack for temporaries.
 *
 * Register allocation strategy (simple, no spilling needed at this scale):
 *   eax  — accumulator: holds the result of every computation
 *   ebx  — secondary operand (right-hand side of binary ops)
 *   esp  — stack pointer (managed by PUSH/POP for temporaries)
 *
 * Memory model:
 *   Variables and temporaries are stored as labelled memory locations
 *   in the .data / .bss section.  Every declared variable gets a DWORD
 *   (4-byte) slot.  Temporaries are tracked in a simple name→label map.
 *
 * Output file:  asm_output.asm
 *   .section .data    — string constants for print format
 *   .section .bss     — uninitialized variable storage
 *   .section .text    — the actual instructions
 * ----------------------------------------------------------------------- */

void generate_code(TACList* list);

/* File pointer for dual stdout + file output — defined in main.c */
extern FILE* asm_file;

#endif /* CODEGEN_H */
