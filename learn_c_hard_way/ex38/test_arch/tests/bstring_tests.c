#include "minunit.h"
#include <lcthw/bstrlib.h>


//test copy flow
//bstrcpy
//bassign

char * test_bstring_proc0()
{
    bstring b = bfromcstr("Hello, World!");
    bstring b_new;
    char cstr[100] = {0};
    int len_tmp = 0;

    mu_assert(strcmp(bdata(b), "Hello, World!") == 0, "bfromcstr failed.");

    b_new = bstrcpy(b);
    mu_assert(strcmp(bdata(b_new), "Hello, World!") == 0, "bstrcpy failed.");
    mu_assert(biseq(b, b_new) == 1, "bstrcpy failed.");
    mu_assert(bstrcmp(b, b_new) == 0, "bstrings not equal after copy.");
    mu_assert(blength(b) == 13, "Wrong length for bstring.");
    mu_assert(blength(b_new) == 13, "Wrong length for bstring.");

    b_new = bfromcstr("Hello, World!2");
    mu_assert(biseq(b, b_new) == 0, "bstrcpy failed.");
    mu_assert(blength(b_new) == 14, "Wrong length for bstring.");

    bassign(b_new, b);
    mu_assert(biseq(b, b_new) == 1, "bassign failed.");
    mu_assert(blength(b_new) == 13, "Wrong length for bstring after bassign.");

    bdestroy(b_new);
    b_new = bfromcstr("12345");
    bassignblk(b_new, "Hello", 5);
    mu_assert(blength(b_new) == 5, "bassignblk failed.");
    mu_assert(strcmp(bdata(b_new), "Hello") == 0, "bassignblk failed.");

    strcpy(cstr, bdata(b));
    mu_assert(strcmp(cstr, "Hello, World!") == 0, "bdata failed.");
    b_new = bformat("%s Goodbye!", "Tom");
    mu_assert(strcmp(bdata(b_new), "Tom Goodbye!") == 0, "bformat failed.");

    bcatcstr(b, " Goodbye!");
    mu_assert(blength(b) == 22, "Wrong length after concatenation.");
    mu_assert(strcmp(bdata(b), "Hello, World! Goodbye!") == 0, "Wrong data after concatenation.");

    bdestroy(b);

    return NULL;
}


char * all_tests()
{
    mu_suite_start();

    mu_run_test(test_bstring_proc0);

    return NULL;
}

RUN_TESTS(all_tests);