#include "main.h"

static int comp(const void *a, const void *b)
{
    return (*(unsigned short*)a) - (*(unsigned short*)b);
}

/*0xDEADCODE*/
void __stdcall Z1IBF(unsigned short *dest, unsigned int n)
{
    /* Sort the array of 2-byte unsigned values.
    */
    qsort(dest, n, sizeof(unsigned short), comp);
}
