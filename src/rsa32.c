#include "main.h"
#include "rsa32.h"

static huge_t modexp(huge_t a, huge_t b, huge_t n)
{
  huge_t y;
  y = 1;
  /* Compute pow(a, b) % n using the binary square and multiply method.*/
  while (b != 0){
    /*  For each 1 in b, accumulate y. */
    if (b & 1){
      y = (y * a) % n;
    }
    /* Square a for each bit in b. */
    a = (a * a) % n;
    /*  Prepare for the next bit in b. */
    b = b >> 1;
  }

  return y;
}

void rsa_encrypt(huge_t plaintext, huge_t *ciphertext, rsa_keypair_t pubkey)
{
  *ciphertext = modexp(plaintext, pubkey.exp.pub, pubkey.n);

  return;
}

void rsa_decrypt(huge_t ciphertext, huge_t *plaintext, rsa_keypair_t prikey)
{
  *plaintext = modexp(ciphertext, prikey.exp.pri, prikey.n);

  return;
}
