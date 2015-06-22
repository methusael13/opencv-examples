#include <stdio.h>
#include <stdlib.h>

/**
 * Basic implementation for Heapsort
 * @author: Methusael Murmu
**/ 

inline void swap_data(void **d1, void **d2) {
    void *t = *d1;
    *d1 = *d2;
    *d2 = t;
}

void max_heapify(void **arr, int idx, int hsz, int (* compare)(void *, void *)) {
    int lt = 2 * idx + 1;
    int rt = 2 * (idx + 1);
    int max;

    if (lt < hsz && compare(arr[idx], arr[lt]) < 0)
        max = lt;
    else
        max = idx;
    if (rt < hsz && compare(arr[max], arr[rt]) < 0)
        max = rt;

    if (idx != max) {
        swap_data(&arr[idx], &arr[max]);
        max_heapify(arr, max, hsz, compare);
    }
}

void build_max_heap(void **arr, int asz, int (* compare)(void *, void *)) {
    int i;
    for (i = asz/2 - 1; i >= 0; i--)
        max_heapify(arr, i, asz, compare);
}

/*
 * Sorts an array of integers
 * arr: The integer array to sort
 * asz: Size of the array
 * compare: Function to be used for comparison
 */
void heap_sort(void **arr, int asz, int (* compare)(void *, void *)) {
    if (asz < 2) return;

    int i;
    build_max_heap(arr, asz, compare);
    for (i = asz - 1; i > 0; i--) {
        swap_data(&arr[0], &arr[i]);
        max_heapify(arr, 0, --asz, compare);
    }
}
