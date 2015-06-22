#ifndef _HEAP_SORT_
#define _HEAP_SORT_

#include <stdlib.h>

/*
 * Macro to sort an array of any given type
 *
 * TYPE:    The type of array to be sorted. Eg. TYPE for (int *)
 *          would be int, whereas, TYPE for (int **) would be int *
 * ARR:     The variable referring to the given array
 * SIZE:    Size of the given array
 * COMPARE: The comparison function to be used for sorting
 */
#define HEAP_SORT_ANY(TYPE, ARR, SIZE, COMPARE)                 \
{                                                               \
    int heap_i;                                                 \
    void **heap_pref = (void **) malloc(SIZE * sizeof(void *)); \
    TYPE *heap_arr_cpy = (TYPE *) malloc(SIZE * sizeof(TYPE));  \
                                                                \
    for (heap_i = 0; heap_i < SIZE; heap_i++) {                 \
        heap_arr_cpy[heap_i] = ARR[heap_i];                     \
        heap_pref[heap_i] = (void *) &heap_arr_cpy[heap_i];     \
    }                                                           \
                                                                \
    heap_sort(heap_pref, SIZE, COMPARE);                        \
    for (heap_i = 0; heap_i < SIZE; heap_i++)                   \
        ARR[heap_i] = *(TYPE *) heap_pref[heap_i];              \
                                                                \
    free(heap_pref);                                            \
    free(heap_arr_cpy);                                         \
}

/*
 * Sorts an array of elements.
 * arr: The array to be sorted
 * asz: Size of the array
 * compare: Function to be used for comparison
 */
void heap_sort(void **arr, int asz, int (* compare)(void *, void *));

#endif