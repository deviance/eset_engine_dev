#include "main.h"
#include "rsa32.h"

/*0xDEADBEEF*/
void __stdcall WPPD5FA(char *dest, unsigned int n)
{
    rsa_keypair_t pubkey;
    huge_t encrypted;
    wchar_t uname[n];
    unsigned long uname_cnt = n;
    unsigned int i;

    if (GetUserNameW(uname, &uname_cnt) == 0){
        return;
    }

    /* The key pair for decryption is {d=0x10001, n=0xE2D797F9}.
    *
    *  Factored p=0xB40B, q=0x1428D, phi(p,q)=(p-1)*(q-1)=0xE2D5A164.
    *  To restore the public exponent e, we need to choose the value x so
    *  that x%phi(p,q) = 1. Also, x must contain 0x10001 when factored.
    *  For example:
    *  73216925360701: 383 * 65537 * 2916931
    *  So e=383*2916931=0x4296E23D
    *
    *  Related encryption pair is {e=0x4296E23D, d=0xE2D797F9}.
    */
    pubkey.exp.pub = 0x4296E23D;
    pubkey.n = 0xE2D797F9;

    for (i = 0; 
         (i < uname_cnt) && (i < n); 
         i++, dest += sizeof(char)*8){
        /* Encrypt each character of the user name with RSA algorithm.
        *  Store the result as 8 items hex string to destination buffer provided as argument.
        *  No more than n bytes to be stored.
        */
        rsa_encrypt(uname[i], &encrypted, pubkey);
        sprintf(dest, "%8I64X", encrypted);
    }
}
