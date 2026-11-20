#include <stdio.h>
#include <limits.h>
#include "maxsub.h"

/* Reads daily prices from a text file. Returns the number of days, or -1 on error. */
int load_prices(const char *filename, int p[]) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: cannot open %s\n", filename);
        return -1;
    }

    int n = 0;
    while (n < MAX_DAYS && fscanf(fp, "%d", &p[n]) == 1) {
        if (p[n] <= 0) {
            printf("Skipping invalid price (must be positive)\n");
            continue;
        }
        n++;
    }

    fclose(fp);
    return n;
}

/* best sum of a subarray that crosses the middle (c[mid] and c[mid+1] both included) */
static int max_crossing(const int c[], int lo, int mid, int hi, int *start, int *end) {
    int sum = 0, left_best = INT_MIN, right_best = INT_MIN;

    for (int i = mid; i >= lo; i--) {
        sum += c[i];
        if (sum > left_best) { left_best = sum; *start = i; }
    }
    sum = 0;
    for (int j = mid + 1; j <= hi; j++) {
        sum += c[j];
        if (sum > right_best) { right_best = sum; *end = j; }
    }
    return left_best + right_best;
}

/* Returns the maximum subarray sum of c[lo..hi]; its position goes in *start and *end. */
int max_subarray(const int c[], int lo, int hi, int *start, int *end) {
    if (lo == hi) {
        *start = lo;
        *end = hi;
        return c[lo];
    }

    int mid = lo + (hi - lo) / 2;
    int ls, le, rs, re, cs, ce;

    int left  = max_subarray(c, lo, mid, &ls, &le);
    int right = max_subarray(c, mid + 1, hi, &rs, &re);
    int cross = max_crossing(c, lo, mid, hi, &cs, &ce);

    if (left >= right && left >= cross)  { *start = ls; *end = le; return left; }
    if (right >= left && right >= cross) { *start = rs; *end = re; return right; }
    *start = cs; *end = ce;
    return cross;
}