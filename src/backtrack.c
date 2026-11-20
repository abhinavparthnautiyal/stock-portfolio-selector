#include <stdio.h>
#include "asset.h"

#define MAX_BT 25            /* backtracking is O(2^n), so limit the input size */

long bt_calls = 0;

static const Asset *items;
static int count, limit_risk, best_ret;
static int best_set[MAX_ASSETS], cur_set[MAX_ASSETS];

/* decides asset i: first try buying it, then try skipping it */
static void explore(int i, int budget_left, int ret) {
    bt_calls++;

    if (i == count) {                       /* all assets decided */
        if (ret > best_ret) {
            best_ret = ret;
            for (int k = 0; k < count; k++) best_set[k] = cur_set[k];
        }
        return;
    }

    /* choice 1: buy (pruned if it breaks the budget or the risk limit) */
    if (items[i].risk <= limit_risk && items[i].cost <= budget_left) {
        cur_set[i] = 1;
        explore(i + 1, budget_left - items[i].cost, ret + items[i].ret);
    }

    /* choice 2: skip */
    cur_set[i] = 0;
    explore(i + 1, budget_left, ret);
}

/* Optimal selection by trying all combinations. Returns the best total return,
   or -1 if n is too large. */
int backtrack(const Asset a[], int n, int budget, int max_risk, int chosen[]) {
    if (n > MAX_BT) {
        printf("Backtracking skipped: n = %d is above %d\n", n, MAX_BT);
        return -1;
    }

    items = a;
    count = n;
    limit_risk = max_risk;
    best_ret = 0;
    bt_calls = 0;
    for (int k = 0; k < n; k++) { best_set[k] = 0; cur_set[k] = 0; }

    explore(0, budget, 0);

    for (int k = 0; k < n; k++) chosen[k] = best_set[k];
    return best_ret;
}