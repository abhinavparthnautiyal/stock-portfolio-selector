#include <stdio.h>
#include "asset.h"

int main(void) {
    Asset assets[MAX_ASSETS];
    int n = load_assets("data/assets.txt", assets);
    if (n <= 0) return 1;
    print_assets(assets, n);
    return 0;
}