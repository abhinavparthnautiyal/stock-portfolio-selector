#ifndef MAXSUB_H
#define MAXSUB_H

#define MAX_DAYS 1000

int load_prices(const char *filename, int p[]);
int max_subarray(const int c[], int lo, int hi, int *start, int *end);

#endif