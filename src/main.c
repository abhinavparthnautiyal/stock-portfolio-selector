#include <stdio.h>
#include <stdlib.h>
#include "asset.h"
#include "maxsub.h"

/* reads one integer from the keyboard; returns 0 if the input is not a number */
static int read_int(const char *prompt, int *value) {
    printf("%s", prompt);
    int r = scanf("%d", value);
    if (r == EOF) exit(0);
    if (r != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);   /* throw away the bad input */
        return 0;
    }
    return 1;
}

static void run_greedy(const Asset a[], int n, int budget, int max_risk) {
    int chosen[MAX_ASSETS];
    greedy(a, n, budget, max_risk, chosen);
    print_selection(a, n, chosen, "Greedy portfolio:");
}

static void run_dp(const Asset a[], int n, int budget, int max_risk) {
    int chosen[MAX_ASSETS];
    if (knapsack_dp(a, n, budget, max_risk, chosen) >= 0)
        print_selection(a, n, chosen, "DP optimal portfolio:");
}

static void run_backtrack(const Asset a[], int n, int budget, int max_risk) {
    int chosen[MAX_ASSETS];
    if (backtrack(a, n, budget, max_risk, chosen) >= 0) {
        print_selection(a, n, chosen, "Backtracking portfolio:");
        printf("  Recursive calls: %ld\n", bt_calls);
    }
}

static void compare_all(const Asset a[], int n, int budget, int max_risk) {
    int c1[MAX_ASSETS], c2[MAX_ASSETS], c3[MAX_ASSETS];
    int g = greedy(a, n, budget, max_risk, c1);
    int d = knapsack_dp(a, n, budget, max_risk, c2);
    int b = backtrack(a, n, budget, max_risk, c3);

    printf("\n%-14s %s\n", "Method", "Total return");
    printf("%-14s %d\n", "Greedy", g);
    if (d >= 0) printf("%-14s %d\n", "DP", d);
    if (b >= 0) printf("%-14s %d (%ld calls)\n", "Backtracking", b, bt_calls);
    if (d >= 0 && b >= 0)
        printf(d == b ? "DP matches the exact answer.\n"
                      : "MISMATCH between DP and backtracking!\n");
}

static void show_window(void) {
    int prices[MAX_DAYS], change[MAX_DAYS];
    int days = load_prices("data/prices.txt", prices);
    if (days < 2) {
        printf("Need at least 2 prices\n");
        return;
    }
    for (int i = 0; i + 1 < days; i++)
        change[i] = prices[i + 1] - prices[i];

    int s, e;
    int gain = max_subarray(change, 0, days - 2, &s, &e);
    printf("\nBest holding window (max subarray of daily changes):\n");
    if (gain > 0)
        printf("  Buy on day %d at %d, sell on day %d at %d, gain %d\n",
               s + 1, prices[s], e + 2, prices[e + 1], gain);
    else
        printf("  No profitable window in this price history\n");
}

int main(void) {
    Asset assets[MAX_ASSETS];
    int budget = 50, max_risk = 10, choice, value;

    int n = load_assets("data/assets.txt", assets);
    if (n <= 0) return 1;
    merge_sort(assets, 0, n - 1);

    do {
        printf("\n===== Stock Portfolio Selector =====\n");
        printf("Assets: %d | Budget: %d | Risk limit: %d\n", n, budget, max_risk);
        printf("1. Show assets (sorted by return/cost)\n");
        printf("2. Greedy portfolio\n");
        printf("3. DP optimal portfolio\n");
        printf("4. Backtracking portfolio\n");
        printf("5. Compare all three\n");
        printf("6. Best holding window (price history)\n");
        printf("7. Change budget / risk limit\n");
        printf("0. Exit\n");

        if (!read_int("Choose: ", &choice)) choice = -1;

        switch (choice) {
            case 1: print_assets(assets, n); break;
            case 2: run_greedy(assets, n, budget, max_risk); break;
            case 3: run_dp(assets, n, budget, max_risk); break;
            case 4: run_backtrack(assets, n, budget, max_risk); break;
            case 5: compare_all(assets, n, budget, max_risk); break;
            case 6: show_window(); break;
            case 7:
                if (read_int("New budget (1-2000): ", &value) &&
                    value >= 1 && value <= MAX_BUDGET)
                    budget = value;
                else
                    printf("Invalid budget, unchanged\n");
                if (read_int("New risk limit (1-10): ", &value) &&
                    value >= 1 && value <= 10)
                    max_risk = value;
                else
                    printf("Invalid risk limit, unchanged\n");
                break;
            case 0: printf("Goodbye\n"); break;
            default: printf("Invalid choice\n");
        }
    } while (choice != 0);

    return 0;
}