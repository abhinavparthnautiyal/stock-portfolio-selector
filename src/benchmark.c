#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "asset.h"

#define REPS 50   /* repeat fast methods so the timer can measure them */

static Asset base[MAX_ASSETS], work[MAX_ASSETS];
static int chosen[MAX_ASSETS];

/* the same n always gives the same assets (fixed seed) */
static void make_assets(int n) {
    srand(42);
    for (int i = 0; i < n; i++) {
        sprintf(base[i].name, "S%d", i + 1);
        base[i].cost = rand() % 50 + 1;
        base[i].ret  = base[i].cost * 2 + rand() % (base[i].cost * 4 + 1);
        base[i].risk = rand() % 10 + 1;
    }
}

static double ms_since(clock_t start) {
    return 1000.0 * (clock() - start) / CLOCKS_PER_SEC;
}

static void run_case(const char *label, int n, int budget, int max_risk) {
    make_assets(n);
    printf("\n[%s] n=%d, budget=%d, risk limit=%d\n", label, n, budget, max_risk);

    clock_t t = clock();
    int g = 0;
    for (int r = 0; r < REPS; r++) {
        for (int i = 0; i < n; i++) work[i] = base[i];
        merge_sort(work, 0, n - 1);
        g = greedy(work, n, budget, max_risk, chosen);
    }
    printf("  Greedy        return %6d   time %9.3f ms\n", g, ms_since(t) / REPS);

    t = clock();
    int d = 0;
    for (int r = 0; r < REPS; r++)
        d = knapsack_dp(base, n, budget, max_risk, chosen);
    printf("  DP            return %6d   time %9.3f ms\n", d, ms_since(t) / REPS);

    if (n <= 25) {
        t = clock();
        int b = backtrack(base, n, budget, max_risk, chosen);
        printf("  Backtracking  return %6d   time %9.3f ms   calls %ld\n",
               b, ms_since(t), bt_calls);
        printf("  DP matches backtracking: %s\n", d == b ? "YES" : "NO  <-- BUG");
    } else {
        printf("  Backtracking  skipped (n > 25, it needs O(2^n) time)\n");
    }
}

int main(void) {
    run_case("Normal operation",   20, 300, 10);
    run_case("Constraint shock",   20, 150, 5);
    run_case("Small input",        10, 300, 10);
    run_case("Largest backtrack",  25, 300, 10);
    run_case("Large stress case", 1000, 2000, 10);
    return 0;
}