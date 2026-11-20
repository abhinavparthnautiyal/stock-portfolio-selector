#include <stdio.h>
#include "asset.h"

int main(void) {
    Asset assets[MAX_ASSETS];
    int chosen[MAX_ASSETS];
    int budget = 50, max_risk = 10;

    int n = load_assets("data/assets.txt", assets);
    if (n <= 0) return 1;

    merge_sort(assets, 0, n - 1);
    printf("Assets sorted by return/cost:\n");
    print_assets(assets, n);

    greedy(assets, n, budget, max_risk, chosen);
    print_selection(assets, n, chosen, "Greedy portfolio (budget 50):");
    return 0;
}