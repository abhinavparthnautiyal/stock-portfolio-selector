#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asset.h"

static int passed = 0, failed = 0;
static int chosen[MAX_ASSETS];

static void check(const char *name, int got, int expected) {
    if (got == expected) {
        passed++;
        printf("  PASS  %s\n", name);
    } else {
        failed++;
        printf("  FAIL  %s (got %d, expected %d)\n", name, got, expected);
    }
}

static Asset make(const char *name, int cost, int ret, int risk) {
    Asset a;
    strcpy(a.name, name);
    a.cost = cost;
    a.ret = ret;
    a.risk = risk;
    return a;
}

/* returns 1 if the array is sorted by return/cost, best first */
static int is_sorted(const Asset a[], int n) {
    for (int i = 0; i + 1 < n; i++)
        if (a[i].ret * a[i + 1].cost < a[i + 1].ret * a[i].cost) return 0;
    return 1;
}

int main(void) {
    Asset s[5] = {
        make("ITC", 25, 80, 5), make("RELI", 30, 120, 6), make("WIPR", 15, 50, 2),
        make("HDFC", 20, 100, 4), make("INFY", 10, 60, 3)
    };
    merge_sort(s, 0, 4);

    printf("Normal cases\n");
    check("sorted by ratio", is_sorted(s, 5), 1);
    check("greedy, budget 50", greedy(s, 5, 50, 10, chosen), 210);
    check("DP, budget 50", knapsack_dp(s, 5, 50, 10, chosen), 220);
    check("backtracking, budget 50", backtrack(s, 5, 50, 10, chosen), 220);

    printf("Boundary cases\n");
    check("budget 0 (DP)", knapsack_dp(s, 5, 0, 10, chosen), 0);
    check("budget below cheapest asset (greedy)", greedy(s, 5, 9, 10, chosen), 0);
    check("budget equals one asset cost (DP)", knapsack_dp(s, 5, 10, 10, chosen), 60);
    check("budget buys everything (greedy)", greedy(s, 5, 100, 10, chosen), 410);
    check("budget buys everything (DP)", knapsack_dp(s, 5, 100, 10, chosen), 410);
    check("risk limit 1, nothing allowed (DP)", knapsack_dp(s, 5, 50, 1, chosen), 0);
    check("risk limit 2, only WIPR (DP)", knapsack_dp(s, 5, 50, 2, chosen), 50);
    check("empty list (DP)", knapsack_dp(s, 0, 50, 10, chosen), 0);
    check("empty list (backtracking)", backtrack(s, 0, 50, 10, chosen), 0);
    Asset one = make("ONE", 5, 9, 1);
    check("single asset fits (DP)", knapsack_dp(&one, 1, 5, 10, chosen), 9);
    check("single asset too costly (DP)", knapsack_dp(&one, 1, 4, 10, chosen), 0);

    printf("Invalid input\n");
    check("budget above limit (DP)", knapsack_dp(s, 5, MAX_BUDGET + 1, 10, chosen), -1);
    Asset big[26];
    for (int i = 0; i < 26; i++) big[i] = make("X", 1, 1, 1);
    check("too many assets (backtracking)", backtrack(big, 26, 10, 10, chosen), -1);
    Asset loaded[MAX_ASSETS];
    check("missing file", load_assets("tests/no_such_file.txt", loaded), -1);
    check("file with 2 bad rows keeps 2 good rows",
          load_assets("tests/bad_assets.txt", loaded), 2);

    printf("Adversarial case (greedy fails, DP does not)\n");
    Asset trap[2] = { make("SMALL", 1, 2, 1), make("BIG", 100, 190, 1) };
    merge_sort(trap, 0, 1);
    check("greedy takes the best ratio and gets 2", greedy(trap, 2, 100, 10, chosen), 2);
    check("DP finds the better answer 190", knapsack_dp(trap, 2, 100, 10, chosen), 190);
    check("backtracking agrees, 190", backtrack(trap, 2, 100, 10, chosen), 190);

    printf("Stress: 300 random inputs, DP vs backtracking\n");
    srand(7);
    int mismatch = 0, greedy_better = 0;
    for (int t = 0; t < 300; t++) {
        int n = rand() % 12 + 1;
        Asset r[12];
        for (int i = 0; i < n; i++) {
            int cost = rand() % 40 + 1;
            r[i] = make("R", cost, cost * 2 + rand() % (cost * 4 + 1), rand() % 10 + 1);
        }
        int budget = rand() % 150 + 1, risk = rand() % 10 + 1;
        int d = knapsack_dp(r, n, budget, risk, chosen);
        int b = backtrack(r, n, budget, risk, chosen);
        merge_sort(r, 0, n - 1);
        int g = greedy(r, n, budget, risk, chosen);
        if (d != b) mismatch++;
        if (g > d) greedy_better++;
    }
    check("DP equals backtracking on every input", mismatch, 0);
    check("greedy never beats DP", greedy_better, 0);

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed ? 1 : 0;
}