#include  "utils.h"
#include "cache.h"
#include "armemu.h"

void quadratic_test() {
    struct arm_state state;
    struct dynamic_analysis analyzer;
    unsigned int r;
    r = quadratic_c(0, 1, 1, 1);
    printf("quadratic_c{0,1,1,1} = %d\n", r);
    r = quadratic_s(0, 1, 1, 1);
    printf("quadratic_s{0,1,1,1} = %d\n", r);
    arm_state_init(&state, (unsigned int *) quadratic_s, 0, 1, 1, 1, &analyzer);
    r = armemu(&state, &analyzer);
    printf("quadratic_e{0,1,1,1} = %d\n", r);
    arm_state_print(&state, &analyzer);
    cache_state_stats(&state.cstate);
    printf("\n");

    r = quadratic_c(-1, 1, -1, 1);
    printf("quadratic_c{-1,1,-1,1} = %d\n", r);
    r = quadratic_s(-1, 1, -1, 1);
    printf("quadratic_s{-1,1,-1,1} = %d\n", r);
    arm_state_init(&state, (unsigned int *) quadratic_s, -1, 1, -1, 1, &analyzer);
    r = armemu(&state, &analyzer);
    printf("quadratic_e{-1,1,-1,1} = %d\n", r);
    arm_state_print(&state, &analyzer);
    cache_state_stats(&state.cstate);
    printf("\n");

    r = quadratic_c(0, 0, 0, 0);
    printf("quadratic_c{0,0,0,0} = %d\n", r);
    r = quadratic_s(0, 0, 0, 0);
    printf("quadratic_s{0,0,0,0} = %d\n", r);
    arm_state_init(&state, (unsigned int *) quadratic_s, 0, 0, 0, 0, &analyzer);
    r = armemu(&state, &analyzer);
    printf("quadratic_e{0,0,0,0} = %d\n", r);
    arm_state_print(&state, &analyzer);
    cache_state_stats(&state.cstate);
    printf("\n");
}
