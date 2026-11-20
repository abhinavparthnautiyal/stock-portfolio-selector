#ifndef ASSET_H
#define ASSET_H

#define MAX_ASSETS 1000
#define NAME_LEN   20

#define MAX_BUDGET  2000

typedef struct {
    char name[NAME_LEN];
    int  cost;    /* price to buy, in thousands of rupees */
    int  ret;     /* expected return, in thousands of rupees */
    int  risk;    /* risk score from 1 (safe) to 10 (risky) */
} Asset;

typedef struct {
    char name[NAME_LEN];
    int  pos;     /* position of this stock in the assets array */
} IndexEntry;

int  load_assets(const char *filename, Asset a[]);
void print_assets(const Asset a[], int n);

void merge_sort(Asset a[], int lo, int hi);

int  greedy(const Asset a[], int n, int budget, int max_risk, int chosen[]);
void print_selection(const Asset a[], int n, const int chosen[], const char *title);

int  knapsack_dp(const Asset a[], int n, int budget, int max_risk, int chosen[]);

extern long bt_calls;   /* number of recursive calls made by backtracking */
int backtrack(const Asset a[], int n, int budget, int max_risk, int chosen[]);

extern long sort_cmps;
void quick_sort(Asset a[], int lo, int hi);

void build_index(const Asset a[], int n, IndexEntry idx[]);
int  linear_search(const Asset a[], int n, const char *key, int *cmps);
int  binary_search_iter(const IndexEntry idx[], int n, const char *key, int *cmps);
int  binary_search_rec(const IndexEntry idx[], int n, const char *key, int *cmps);

#endif