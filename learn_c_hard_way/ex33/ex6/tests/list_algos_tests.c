#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS", "apple", "applepie", "applecider", "appleseed", "applause"};
#define NUM_VALUES 10

int unsigned str_cnt;
char **str_arr;
int unsigned init_done = 0;

List *create_words()
{
    int i = 0;
    int rand_len = 0;
    int MAX_STR_LEN = 8;
    int MAX_ENTRY_LEN = 7;
    int MIN_ENTRY_LEN = 3;

    List *words = List_create();

    if (init_done == 0) {
        init_done = 1;

        str_cnt = rand() % (MAX_ENTRY_LEN - MIN_ENTRY_LEN + 1) + MIN_ENTRY_LEN;
        str_arr = malloc(sizeof(char *) * str_cnt);
        for (int i = 0; i < str_cnt; i++)
        {
            int cap_en = 0;

            rand_len = rand() % MAX_STR_LEN;
            rand_len += 5; // ensure min length of 5
            str_arr[i] = malloc(sizeof(char) * (rand_len + 1));
            for (int j = 0; j < rand_len; j++)
            {
                cap_en = rand() % 2;
                if (cap_en)
                    str_arr[i][j] = 'A' + (rand() % 26);
                else
                    str_arr[i][j] = 'a' + (rand() % 26);
            }
            str_arr[i][rand_len] = '\0';
            // printf("Generated entry[%d/%d]: %s\n", i, str_cnt, str_arr[i]);
        }
    }

    for(i = 0; i < str_cnt; i++) {
        List_push(words, str_arr[i]);
    }

    return words;
}

int is_sorted(List *words)
{
    LIST_FOREACH(words, first, next, cur) {
        if(cur->next && strcmp(cur->value, cur->next->value) > 0) {
            debug("%s %s", (char *)cur->value, (char *)cur->next->value);
            return 0;
        }
    }

    return 1;
}

char *test_bubble_sort()
{
    List *words = create_words();

    // should work on a list that needs sorting
    int rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort failed.");
    mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

    // should work on an already sorted list
    rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort of already sorted failed.");
    mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");

    List_destroy(words);

    // should work on an empty list
    words = List_create(words);
    rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort failed on empty list.");
    mu_assert(is_sorted(words), "Words should be sorted if empty.");

    List_destroy(words);

    return NULL;
}

char *test_merge_sort()
{
    List *words = create_words();

    // should work on a list that needs sorting
    List *res = List_merge_sort(words, (List_compare)strcmp);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

    List *res2 = List_merge_sort(res, (List_compare)strcmp);
    mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
    List_destroy(res2);
    List_destroy(res);

    List_destroy(words);
    return NULL;
}

#include <time.h>

char * bubble_vs_merge_sort()
{
    struct timespec start, end;
    int unsigned loop = 10000;
    double diff;

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (size_t i = 0; i < loop; i++)
    {
        test_bubble_sort();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    diff = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Bubble sort time for %d sorts: %f seconds\n", loop, diff);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (size_t i = 0; i < loop; i++)
    {
        test_merge_sort();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    diff = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Merge sort time for %d sorts: %f seconds\n", loop, diff);


    return NULL;
}

char * wiki_bottom_up_merge_sort()
{
    int *A;
    int n;
    int *B;
    struct timespec start, end;
    int unsigned loop = 10000;
    double diff;

    n = random() % 128 + 128; //128-256
    A = malloc(sizeof(int) * n);
    B = malloc(sizeof(int) * n);
    for (size_t i = 0; i < n; i++)
    {
        A[i] = random() % 10000;
    }

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &start);
    BottomUpMergeSort(A, B, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    diff = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("BottomUpMergeSort time for %d sorts: %f seconds\n", loop, diff);

    for (int i = 0; i < n; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");

    free(A);
    free(B);

    return NULL;
}


char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);

    mu_run_test(bubble_vs_merge_sort)
    mu_run_test(wiki_bottom_up_merge_sort);
    return NULL;
}

RUN_TESTS(all_tests);