#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <stdio.h>
#include "tac.h"

/* -----------------------------------------------------------------------
 * TAC Optimizer
 *
 * Runs three classical peephole / local optimizations in sequence:
 *
 *  Pass 1 — Constant Folding
 *      BIN_OP where both arg1 and arg2 are numeric literals is computed
 *      at compile time and replaced with a single ASSIGN of the result.
 *      Example:  t0 = 2 + 3   →   t0 = 5
 *
 *  Pass 2 — Copy Propagation
 *      When we see  x = t0  (a plain copy), every later use of t0 is
 *      replaced with x, and the copy instruction is removed.
 *      Example:  t0 = x + y        t0 = x + y
 *                z  = t0      →    z  = x + y   (copy removed)
 *
 *  Pass 3 — Dead Code Elimination
 *      Any temporary (name starts with 't' followed by digits) that is
 *      assigned but never used anywhere after that point is removed.
 *      Example:  t1 = a * b   ← result never read  →  (removed)
 *
 * The three passes are chained: the output of each feeds the next.
 * optimize_tac() runs all three and returns the final cleaned list.
 * ----------------------------------------------------------------------- */

TACList* optimize_tac(TACList* list);

/* File pointer for dual stdout + file output — defined in main.c */
extern FILE* opt_file;

#endif /* OPTIMIZER_H */
