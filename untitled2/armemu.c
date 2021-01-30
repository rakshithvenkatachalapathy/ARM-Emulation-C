#include <stdbool.h>
#include <stdio.h>
#include "armemu.h"
#include "cache.c"


/* Assembly functions to emulate */

int quadratic_c(int x, int a, int b, int c);

int quadratic_s(int x, int a, int b, int c);

int mov_s(int a, int b);

int sum_array_s(int *ar, int len);

int fib_iter_s(int n);

int fib_rec_s(int n);

int find_max_s(int *ar, int len);

int strlen_s(char *s);

int strlen_c(char *s);
int sort_s(int *a,int len);
int find_max_index_s(int *array, int len);

/* Initialize an arm_state struct with a function pointer and arguments */
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
bool is_mul_inst(unsigned int iw) {
    unsigned int bx_code, a;
    bx_code = (iw >> 22) & 0b111111;
    a = (iw >> 21) & 0b1;
    return ((bx_code == 000000) && (a == 0));
}

void armemu_mul(struct arm_state *state, unsigned int iw, struct dynamic_analysis *pAnalysis) {
    unsigned int rd, rs, rm, a;
    rd = (iw >> 16) & 0xF;
    rs = (iw >> 8) & 0xF;
    rm = iw & 0xF;
    a = (iw >> 21) & 0b1;
    if (a == 0) {
        state->regs[rd] = state->regs[rs] * state->regs[rm];
    }
    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
    }
    pAnalysis->dp_count += 1;
    pAnalysis->instruction_count += 1;
}

bool is_dp_inst(unsigned int iw) {
    unsigned int op;
    op = (iw >> 26) & 0b11;
    return (op == 0);
}

void cpsr_update_cmp(unsigned int *cpsr, unsigned int a, unsigned int b) {
    int as, bs, r, N, Z, C, V;
    as = (int) a;
    bs = (int) b;
    r = as - bs;
    if (r < 0)
        N = 1;
    else
        N = 0;
    if (r == 0)
        Z = 1;
    else
        Z = 0;
    if (b > a)
        C = 1;
    else
        C = 0;
    V=0;
    if ((as > 0) && (bs < 0)) {
        if (r < 0) {
            V = 1;
        }
    } else if ((as < 0) && (bs > 0)) {
        if (r > 0)
            V = 1;
    }
    *cpsr = (unsigned int) N * 8 + Z * 4 + C * 2 + V * 1;
}

void armemu_dp(struct arm_state *state, unsigned int iw, struct dynamic_analysis *pAnalysis) {
    unsigned int rd, rn, rm, i, imm;
    unsigned int opcode, op2;
    rd = (iw >> 12) & 0xF;
    rn = (iw >> 16) & 0xF;
    rm = iw & 0xF;
    i = (iw >> 25) & 0b1;
    imm = iw & 0xFF;
    opcode = (iw >> 21) & 0b1111;
    if (i == 0) {
        op2 = state->regs[rm];
    } else {
        op2 = imm;
    }
    if (opcode == 0b0100) {
        /* add */
        state->regs[rd] = state->regs[rn] + op2;
    } else if (opcode == 0b0010) {
        /* sub */
        state->regs[rd] = state->regs[rn] - op2;
    } else if (opcode == 0b1010) {
        /* cmp */
        cpsr_update_cmp(&state->cpsr, state->regs[rn], op2);
    } else if (opcode == 0b1101) {
        /* mov */
        state->regs[rd] = op2;
    }
    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
    }
    pAnalysis->dp_count += 1;
    pAnalysis->instruction_count += 1;
}

bool is_bx_inst(unsigned int iw) {
    unsigned int bx_code;
    bx_code = (iw >> 4) & 0x00FFFFFF;
    return (bx_code == 0b000100101111111111110001);
}

void armemu_bx(struct arm_state *state, unsigned int iw, struct dynamic_analysis *pAnalysis) {
    unsigned int rn;
    rn = iw & 0b1111;
    state->regs[PC] = state->regs[rn];
    pAnalysis->branch_count += 1;
    pAnalysis->branch_taken += 1;
    pAnalysis->instruction_count += 1;
}

bool is_b_inst(unsigned int iw) {
    unsigned int bcode;
    bcode = (iw >> 25) & 0b111;
    return (bcode == 0b101);
}

bool cpsr_check_cond(unsigned int cpsr_val, unsigned int cond) {
    int N, Z, C, V;
    int cpsr = (int) cpsr_val;
    bool flag = false;
    V = cpsr & 0b1;
    C = (cpsr >> 1) & 0b1;
    Z = (cpsr >> 2) & 0b1;
    N = (cpsr >> 3) & 0b1;
    if ((cond == 0b0000) && Z == 0b1) {
        flag = true;
    } else if ((cond == 0b0001) && Z == 0b0)
        flag = true; // bne
    else if ((cond == 0b1100) && Z == 0b0 && N == V)
        flag = true; //bgt
    else if ((cond == 0b1110)) // all
        flag = true;
    return flag;
}

void armemu_b(struct arm_state *as, unsigned int iw, struct dynamic_analysis *pAnalysis) {
    unsigned int link, cond, offset;
    int n_int;
    bool execute;
    link = (iw >> 24) & 0b1;
    cond = (iw >> 28) & 0b1111;
    offset = iw & 0xFFFFFF;
    execute = cpsr_check_cond(as->cpsr, cond);
    if (execute) {
        if ((offset >> 23) == 0b1) {
            //sign extend
            const int negative = (offset & (1 << 23)) != 0;
            if (negative)
                n_int = offset | ~((1 << 24) - 1);
            else
                n_int = offset;
            offset = n_int;
        }
        offset = offset * 4;
        offset = offset + 8;
        if (link) {
            as->regs[LR] = as->regs[PC] + 4;
        }
        as->regs[PC] = as->regs[PC] + offset;
    } else {
        as->regs[PC] = as->regs[PC] + 4;
    }
    if (execute)
        pAnalysis->branch_taken += 1;
    else
        pAnalysis->branch_not_taken += 1;
    pAnalysis->branch_count += 1;
    pAnalysis->instruction_count += 1;
}

bool is_mem_inst(unsigned int iw) {
    unsigned int op;
    op = (iw >> 26) & 0b11;
    return (op == 0b01);
}

void armemu_mem(struct arm_state *state, unsigned int iw, struct dynamic_analysis *pAnalysis) {
    unsigned int L, rn, rd, B, i, imm, rm, up_down;
    unsigned char *addr;
    unsigned int *temp;
    L = (iw >> 20) & 0b1;
    rn = (iw >> 16) & 0b1111;
    rd = (iw >> 12) & 0b1111;
    B = (iw >> 22) & 0b1;
    i = (iw >> 25) & 0b1;
    up_down = (iw >> 23) & 0b1;
    imm = iw & 0xFFF;
    rm = iw & 0xF;
    if (up_down == 1) {
        imm = imm * 1;
    }
    if (i == 0) {
        // offset is an immediate value
        addr = (unsigned char *) state->regs[rn] + imm;
    } else {
        //offset is a register
        addr = (unsigned char *) state->regs[rn] + state->regs[rm];
    }
    if (L == 0b0) {
        if (B == 0) {
            temp = (unsigned int *) addr;
            *temp = state->regs[rd];
        } else
            *addr = state->regs[rd];
    } else {
        if (B == 0) {
            temp = (unsigned int *) addr;
            state->regs[rd] = *temp;
        } else
            state->regs[rd] = *addr;
    }
    if (rd != PC) {
        state->regs[PC] = state->regs[PC] + 4;
    }
    pAnalysis->instruction_count += 1;
    pAnalysis->mem_count += 1;
}

void armemu_one(struct arm_state *state, struct dynamic_analysis *pAnalysis) {
    unsigned int iw;
    //iw = *((unsigned int *) state->regs[PC]);
    iw = cache_lookup(&state->cstate, state->regs[PC]);
    if (is_bx_inst(iw)) {
        armemu_bx(state, iw, pAnalysis);
    } else if (is_mul_inst(iw)) {
        armemu_mul(state, iw, pAnalysis);
    } else if (is_dp_inst(iw)) {
        armemu_dp(state, iw, pAnalysis);
    } else if (is_b_inst(iw)) {
        armemu_b(state, iw, pAnalysis);
    } else if (is_mem_inst(iw)) {
        armemu_mem(state, iw, pAnalysis);
    }
}

unsigned int armemu(struct arm_state *state, struct dynamic_analysis *pAnalysis) {
    /* Execute instructions until PC = 0 */
    /* This happens when bx lr is issued and lr is 0 */
    while (state->regs[PC] != 0) {
        armemu_one(state, pAnalysis);
    }
    return state->regs[0];
}
