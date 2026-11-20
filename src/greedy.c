#include "asset.h"

/* a[] must already be sorted by ratio (best first).
   Marks chosen[i] = 1 for bought assets. Returns the total return. */
int greedy(const Asset a[], int n, int budget, int max_risk, int chosen[]) {
    int total_return = 0;

    for (int i = 0; i < n; i++) {
        chosen[i] = 0;
        if (a[i].risk <= max_risk && a[i].cost <= budget) {
            chosen[i] = 1;
            budget -= a[i].cost;
            total_return += a[i].ret;
        }
    }
    return total_return;
}