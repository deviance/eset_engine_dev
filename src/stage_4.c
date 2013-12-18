#include "main.h"
#include "crc32.h"

static int compute_roots(unsigned char roots[5],
                         const size_t uname_max,
                         unsigned int serial);

static void compute_coeffs(unsigned char roots[5],
                           uint64_t coeffs[5]);

static void print_out(char *dest,
                      uint64_t coeffs[5]);

/*0xDEADCAFE*/
void __stdcall IH5PME0(char *dest, unsigned int uname_max, unsigned int serial)
{
    unsigned char roots[5];
    uint64_t coeffs[5];

    if (compute_roots(&roots[0], uname_max, serial) != 0){
        return;
    }
    compute_coeffs(&roots[0], &coeffs[0]);
    print_out(dest, coeffs);
}

static int compute_roots(unsigned char roots[5], const size_t uname_max, unsigned int serial)
{
    wchar_t uname[1000];
    unsigned long uname_len = uname_max;
    uint64_t product;
    unsigned int i;

    if (GetUserNameW(uname, &uname_len) == 0){
        return -1;
    };

    /* The roots are computed as a product of crc32(uname) * volume_serial */
    product = (uint64_t)crc32(uname, uname_len, sizeof(wchar_t)) * serial;

    /* Treat the product as an array of unsigned chars and store the roots */
    for (i = 0U; i < 5U; i++){
        roots[i] = ((unsigned char *)&product)[i];
    }

    return 0;
}

static void compute_coeffs(unsigned char roots[5], uint64_t coeffs[5])
{
    /* The equation is
    *  x^5 - Ax^4 + Bx^3 - Cx^2 + Dx - E = 0
    */
    unsigned char x5, x4, x3, x2, x1;
    uint64_t      a, b, c, d, e;

    x5 = roots[0];
    x4 = roots[1];
    x3 = roots[2];
    x2 = roots[3];
    x1 = roots[4];

    /* Compute coefficients with Vieta's formulas */
    a = (uint64_t)x1+x2+x3+x4+x5;

    b = (uint64_t)x1*x2 + x1*x3 + x1*x4 + x1*x5 + x2*x3 +
                  x2*x4 + x2*x5 + x3*x4 + x3*x5 + x4*x5;

    c = (uint64_t)x1*x2*x3 + x1*x2*x4 + x1*x2*x5 + x1*x3*x4 + x1*x3*x5 +
                  x1*x4*x5 + x2*x3*x4 + x2*x3*x5 + x2*x4*x5 + x3*x4*x5;

    d = (uint64_t)x1*x2*x3*x4 + x1*x2*x3*x5 + x1*x3*x4*x5 +
                  x2*x3*x4*x5 + x1*x2*x4*x5;

    e = (uint64_t)x1*x2*x3*x4*x5;

    coeffs[0] = a;
    coeffs[1] = b;
    coeffs[2] = c;
    coeffs[3] = d;
    coeffs[4] = e;
}

static void print_out(char *dest, uint64_t coeffs[5])
{
    int i;

    for (i = 0; 
         i < 5; 
         i++, dest += sizeof(char)*10){
         
        sprintf(dest, "%10I64X", coeffs[i]);
    }
}
