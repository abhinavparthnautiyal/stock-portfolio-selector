#include <stdio.h>
#include "asset.h"

/* Reads assets from a text file. Returns the number loaded, or -1 on error. */
int load_assets(const char *filename, Asset a[]) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: cannot open %s\n", filename);
        return -1;
    }

    int n = 0;
    while (n < MAX_ASSETS &&
           fscanf(fp, "%19s %d %d %d",
                  a[n].name, &a[n].cost, &a[n].ret, &a[n].risk) == 4) {
        if (a[n].cost <= 0 || a[n].ret < 0 ||
            a[n].risk < 1 || a[n].risk > 10) {
            printf("Skipping invalid row: %s\n", a[n].name);
            continue;   /* n is not increased, so the next row overwrites this slot */
        }
        n++;
    }

    fclose(fp);
    return n;
}

void print_assets(const Asset a[], int n) {
    printf("%-4s %-10s %6s %8s %5s\n", "No.", "Name", "Cost", "Return", "Risk");
    for (int i = 0; i < n; i++) {
        printf("%-4d %-10s %6d %8d %5d\n",
               i + 1, a[i].name, a[i].cost, a[i].ret, a[i].risk);
    }
}

void print_selection(const Asset a[], int n, const int chosen[], const char *title) {
    int cost = 0, ret = 0, risk_sum = 0, count = 0;

    printf("\n%s\n", title);
    for (int i = 0; i < n; i++) {
        if (chosen[i]) {
            printf("  %-10s cost %3d  return %3d  risk %d\n",
                   a[i].name, a[i].cost, a[i].ret, a[i].risk);
            cost += a[i].cost;
            ret += a[i].ret;
            risk_sum += a[i].risk;
            count++;
        }
    }
    printf("  Total cost: %d | Total return: %d", cost, ret);
    if (count > 0)
        printf(" | Average risk: %.2f", (double)risk_sum / count);
    printf("\n");
}