#include <stdio.h>
#include "asset.h"
#include "maxsub.h"

int main(void) {
    Asset assets[MAX_ASSETS];
    int chosen_g[MAX_ASSETS], chosen_d[MAX_ASSETS], chosen_b[MAX_ASSETS];
    int budget = 50, max_risk = 10;

    int n = load_assets("data/assets.txt", assets);
    if (n <= 0) return 1;

    merge_sort(assets, 0, n - 1);

    int g = greedy(assets, n, budget, max_risk, chosen_g);
    int d = knapsack_dp(assets, n, budget, max_risk, chosen_d);
    int b = backtrack(assets, n, budget, max_risk, chosen_b);
    if (d < 0 || b < 0) return 1;

    print_selection(assets, n, chosen_g, "Greedy portfolio (budget 50):");
    print_selection(assets, n, chosen_d, "DP optimal portfolio (budget 50):");
    print_selection(assets, n, chosen_b, "Backtracking portfolio (budget 50):");

    printf("\nGreedy = %d, DP = %d, Backtracking = %d\n", g, d, b);
    printf("Backtracking made %ld recursive calls\n", bt_calls);
    printf(d == b ? "DP matches the exact answer.\n"
                  : "MISMATCH between DP and backtracking!\n");

    /* best holding window from the price history */
    int prices[MAX_DAYS], change[MAX_DAYS];
    int days = load_prices("data/prices.txt", prices);
    if (days < 2) {
        printf("Need at least 2 prices\n");
        return 1;
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
    return 0;
}