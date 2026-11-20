#include <string.h>
#include "asset.h"

/* Builds the ticker index: one entry per stock, sorted by name (insertion sort). */
void build_index(const Asset a[], int n, IndexEntry idx[]) {
    for (int i = 0; i < n; i++) {
        IndexEntry e;
        strcpy(e.name, a[i].name);
        e.pos = i;

        int j = i - 1;
        while (j >= 0 && strcmp(idx[j].name, e.name) > 0) {
            idx[j + 1] = idx[j];     /* shift larger names one place right */
            j--;
        }
        idx[j + 1] = e;
    }
}

/* Checks every stock in turn. Returns its position, or -1. */
int linear_search(const Asset a[], int n, const char *key, int *cmps) {
    *cmps = 0;
    for (int i = 0; i < n; i++) {
        (*cmps)++;
        if (strcmp(a[i].name, key) == 0) return i;
    }
    return -1;
}

/* Iterative binary search on the sorted index. Returns the stock's position, or -1. */
int binary_search_iter(const IndexEntry idx[], int n, const char *key, int *cmps) {
    int lo = 0, hi = n - 1;
    *cmps = 0;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int c = strcmp(key, idx[mid].name);
        (*cmps)++;
        if (c == 0) return idx[mid].pos;
        if (c < 0) hi = mid - 1;     /* key is in the left half */
        else       lo = mid + 1;     /* key is in the right half */
    }
    return -1;
}

static int search_range(const IndexEntry idx[], int lo, int hi, const char *key, int *cmps) {
    if (lo > hi) return -1;

    int mid = lo + (hi - lo) / 2;
    int c = strcmp(key, idx[mid].name);
    (*cmps)++;
    if (c == 0) return idx[mid].pos;
    if (c < 0) return search_range(idx, lo, mid - 1, key, cmps);
    return search_range(idx, mid + 1, hi, key, cmps);
}

/* Recursive binary search: the same idea, each step calls itself on one half. */
int binary_search_rec(const IndexEntry idx[], int n, const char *key, int *cmps) {
    *cmps = 0;
    return search_range(idx, 0, n - 1, key, cmps);
}