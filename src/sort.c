#include "asset.h"

static Asset tmp[MAX_ASSETS];   /* helper array used while merging */

/* returns 1 if x has a higher return-per-cost ratio than y */
static int higher_ratio(Asset x, Asset y) {
    return x.ret * y.cost > y.ret * x.cost;
}

/* merges the sorted halves a[lo..mid] and a[mid+1..hi] */
static void merge(Asset a[], int lo, int mid, int hi) {
    int i = lo, j = mid + 1, k = lo;

    while (i <= mid && j <= hi) {
        if (higher_ratio(a[j], a[i]))
            tmp[k++] = a[j++];
        else
            tmp[k++] = a[i++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi)  tmp[k++] = a[j++];

    for (k = lo; k <= hi; k++) a[k] = tmp[k];
}

/* sorts a[lo..hi] by ratio, highest first */
void merge_sort(Asset a[], int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort(a, lo, mid);
    merge_sort(a, mid + 1, hi);
    merge(a, lo, mid, hi);
}