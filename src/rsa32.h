#ifndef RSA32_H_INCLUDED
#define RSA32_H_INCLUDED

/* Structure for RSA keys. */
typedef uint64_t huge_t;
typedef struct{
    union {
        huge_t pub;
        huge_t pri;
    } exp;
  huge_t n;
} rsa_keypair_t;

void rsa_encrypt(huge_t plaintext, huge_t *ciphertext, rsa_keypair_t pubkey);
void rsa_decrypt(huge_t ciphertext, huge_t *plaintext, rsa_keypair_t prikey);

#endif
