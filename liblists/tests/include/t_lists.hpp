#ifndef T_LISTS_HPP
#define T_LISTS_HPP
extern "C" {
#include "SList.h"
#include "DList.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static char str[100];

static void
affInt (const void *data)
{
    char tmp[100];
    intptr_t d = (intptr_t)data;
    snprintf (tmp, 100, "%ld", d);
    if (strlen (str) > 0)
        strncat (str, " ", 1);
    size_t size;
    size = 100 - strlen(str) - 1;
    strncat (str, tmp, size);
}

static void
del (void *data)
{
    printf ("rm %ld\n", (intptr_t)data);
}


static void
constPrint (const void *data)
{
    printf ("%ld\n", (intptr_t)data);
}

static void
myFunc (void **data)
{
    *data = (void *)((intptr_t)*data + 1);
}

static char *
printSL (SList *l)
{
    memset (str, 0, 100);
    SList_foreach_safe (l, affInt);
    return str;
}

static char *
printDL (DList *l)
{
    memset (str, 0, 100);
    DList_foreach_safe (l, affInt);
    return str;
}

static int q[] = { 65, 78, 98, 12, 78, 312, 6 };
}

#include <gtest/gtest.h>

#endif // T_LISTS_HPP
