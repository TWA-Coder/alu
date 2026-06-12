#include "crypto_utils.h"
#include <stdio.h>
#include <string.h>

void compute_sha256(const unsigned char *data, size_t len, unsigned char *output) {
    SHA256(data, len, output);
}

void compute_sha256_hex(const unsigned char *data, size_t len, char *hex_output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    compute_sha256(data, len, hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hex_output + (i * 2), "%02x", hash[i]);
    }
    hex_output[SHA256_HEX_LENGTH - 1] = '\0';
}

EC_KEY* generate_ec_key() {
    EC_KEY *key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!key) return NULL;
    
    if (!EC_KEY_generate_key(key)) {
        EC_KEY_free(key);
        return NULL;
    }
    return key;
}

int ec_sign_data(const unsigned char *data, size_t len, EC_KEY *priv_key, unsigned char *sig, unsigned int *sig_len) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    compute_sha256(data, len, hash); // Sign the SHA256 digest of the data
    
    return ECDSA_sign(0, hash, SHA256_DIGEST_LENGTH, sig, sig_len, priv_key);
}

int ec_verify_data(const unsigned char *data, size_t len, const unsigned char *sig, unsigned int sig_len, EC_KEY *pub_key) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    compute_sha256(data, len, hash);
    
    int ret = ECDSA_verify(0, hash, SHA256_DIGEST_LENGTH, sig, sig_len, pub_key);
    return ret == 1; // 1 for valid signature
}

char* extract_public_key_hex(EC_KEY *key) {
    if (!key) return NULL;
    const EC_POINT *pub_point = EC_KEY_get0_public_key(key);
    const EC_GROUP *group = EC_KEY_get0_group(key);
    
    if (!pub_point || !group) return NULL;
    return EC_POINT_point2hex(group, pub_point, POINT_CONVERSION_COMPRESSED, NULL);
}

void print_hex(const unsigned char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}
