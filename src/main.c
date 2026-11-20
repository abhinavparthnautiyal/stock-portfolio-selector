#include <stdio.h>
#include "asset.h"

int main(void) {
    Asset assets[MAX_ASSETS];
    int n = load_assets("data/assets.txt", assets);
    if (n <= 0) return 1;

    printf("Before sorting:\n");
    print_assets(assets, n);

    merge_sort(assets, 0, n - 1);

    printf("\nAfter sorting by return/cost (best first):\n");
    print_assets(assets, n);
    return 0;
}