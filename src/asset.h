#ifndef ASSET_H
#define ASSET_H

#define MAX_ASSETS 1000
#define NAME_LEN   20

typedef struct {
    char name[NAME_LEN];
    int  cost;    /* price to buy, in thousands of rupees */
    int  ret;     /* expected return, in thousands of rupees */
    int  risk;    /* risk score from 1 (safe) to 10 (risky) */
} Asset;

int  load_assets(const char *filename, Asset a[]);
void print_assets(const Asset a[], int n);

#endif