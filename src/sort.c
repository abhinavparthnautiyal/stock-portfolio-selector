#include "asset.h"

static Asset tmp[MAX_ASSETS];   /* helper array used while merging */
long sort_cmps = 0;             /* counts comparisons, used by the benchmark */

/* returns 1 if x has a higher return-per-cost ratio than y */
static int higher_ratio(Asset x, Asset y) {
    sort_cmps++;
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

static void swap(Asset *x, Asset *y) {
    Asset t = *x;
    *x = *y;
    *y = t;
}

/* sorts a[lo..hi] by ratio, highest first; the pivot is the last element */
void quick_sort(Asset a[], int lo, int hi) {
    if (lo >= hi) return;

    Asset pivot = a[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (higher_ratio(a[j], pivot)) {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[hi]);

    quick_sort(a, lo, i);        /* better-ratio side */
    quick_sort(a, i + 2, hi);    /* worse-ratio side */
}