#include <stdio.h>
#include "asset.h"

static int dp[MAX_ASSETS + 1][MAX_BUDGET + 1];

/* Optimal 0/1 selection. Marks chosen[i] = 1 and returns the best total return,
   or -1 if the budget is larger than MAX_BUDGET. */
int knapsack_dp(const Asset a[], int n, int budget, int max_risk, int chosen[]) {
    if (budget > MAX_BUDGET) {
        printf("Error: budget %d is above the limit of %d\n", budget, MAX_BUDGET);
        return -1;
    }

    for (int w = 0; w <= budget; w++) dp[0][w] = 0;

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= budget; w++) {
            dp[i][w] = dp[i - 1][w];                       /* skip asset i */
            if (a[i - 1].risk <= max_risk && a[i - 1].cost <= w) {
                int take = dp[i - 1][w - a[i - 1].cost] + a[i - 1].ret;
                if (take > dp[i][w]) dp[i][w] = take;      /* buy asset i */
            }
        }
    }

    /* walk back through the table to find which assets were bought */
    int w = budget;
    for (int i = n; i >= 1; i--) {
        chosen[i - 1] = 0;
        if (dp[i][w] != dp[i - 1][w]) {
            chosen[i - 1] = 1;
            w -= a[i - 1].cost;
        }
    }
    return dp[n][budget];
}