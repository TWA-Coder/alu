#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <stddef.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/sha.h>

#define SHA256_HEX_LENGTH (SHA256_DIGEST_LENGTH * 2 + 1)
#define MAX_SIGNATURE_LENGTH 128

// Generates a 256-bit SHA-256 hash.
// output must be at least SHA256_DIGEST_LENGTH bytes (32).
void compute_sha256(const unsigned char *data, size_t len, unsigned char *output);

// Hex string output of SHA-256
void compute_sha256_hex(const unsigned char *data, size_t len, char *hex_output);

// Generates a new secp256k1 EC key pair
EC_KEY* generate_ec_key();

// Signs data with the given EC private key.
// Produces the signature and the signature length.
int ec_sign_data(const unsigned char *data, size_t len, EC_KEY *priv_key, unsigned char *sig, unsigned int *sig_len);

// Verifies a signed data payload against the provided EC public key
int ec_verify_data(const unsigned char *data, size_t len, const unsigned char *sig, unsigned int sig_len, EC_KEY *pub_key);

// Exports public key to a hex string representation (address)
char* extract_public_key_hex(EC_KEY *key);

void print_hex(const unsigned char *data, size_t len);

#endif // CRYPTO_UTILS_H
