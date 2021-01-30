/* armemu.h - Public data structures and functions from armemu.c */
#ifndef ARMEMU_H
#define ARMEMU_H

/* cache.h is needed for struct arm_state */
#include <stdio.h>
#include "cache.h"

#define NREGS 16
#define STACK_SIZE 1024
#define SP 13
#define LR 14
#define PC 15

/* The complete machine state */
struct arm_state {
    unsigned int regs[NREGS];
    unsigned int cpsr;
    unsigned char stack[STACK_SIZE];
    struct cache_state cstate;
};

struct dynamic_analysis {
    int instruction_count;
    int dp_count;
    int mem_count;
    int branch_count;
    int branch_taken;
    int branch_not_taken;
};

/* Public functions prototypes */
void arm_state_init(struct arm_state *as, unsigned int *func, unsigned int arg0, unsigned int arg1, unsigned int arg2,
                    unsigned int arg3, struct dynamic_analysis *pAnalysis) {
    int i;
    /* Zero out all registers */
    for (i = 0; i < NREGS; i++) {
        as->regs[i] = 0;
    }
    /* Zero out CPSR */
    as->cpsr = 0;
    /* Zero out the stack */
    for (i = 0; i < STACK_SIZE; i++) {
        as->stack[i] = 0;
    }
    /* Set the PC to point to the address of the function to emulate */
    as->regs[PC] = (unsigned int) func;
    /* Set the SP to the top of the stack (the stack grows down) */
    as->regs[SP] = (unsigned int) &as->stack[STACK_SIZE];
    /* Initialize LR to 0, this will be used to determine when the function has called bx lr */
    as->regs[LR] = 0;
    /* Initialize the first 4 arguments */
    as->regs[0] = arg0;
    as->regs[1] = arg1;
    as->regs[2] = arg2;
    as->regs[3] = arg3;

    cache_state_init(&as->cstate);

    pAnalysis->branch_count = 0;
    pAnalysis->branch_not_taken = 0;
    pAnalysis->branch_taken = 0;
    pAnalysis->dp_count = 0;
    pAnalysis->instruction_count = 0;
    pAnalysis->mem_count = 0;
}

void arm_state_free(struct arm_state *as) {
    cache_state_free(&as->cstate);
}

void arm_state_print(struct arm_state *as, struct dynamic_analysis *pAnalysis) {
    int i;
    for (i = 0; i < NREGS; i++) {
        printf("reg[%d] = %d\n", i, as->regs[i]);
    }
    printf("cpsr = %X\n", as->cpsr);
    printf("Total Instruction count = %d\n", pAnalysis->instruction_count);
    printf("Data processing count = %d\n", pAnalysis->dp_count);
    printf("Memory count = %d\n", pAnalysis->mem_count);
    printf("Total Branch count = %d\n", pAnalysis->branch_count);
    printf("Branch taken = %d\n", pAnalysis->branch_taken);
    printf("Branch not taken = %d\n", pAnalysis->branch_not_taken);
}


#endif /* ARMEMU_H */