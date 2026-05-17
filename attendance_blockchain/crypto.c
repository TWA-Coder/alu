#include "attendance_system.h"

EVP_PKEY *keypair = NULL;

void generate_keypair() {
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    EVP_PKEY_keygen_init(pctx);
    EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_secp256k1);
    EVP_PKEY_keygen(pctx, &keypair);
    EVP_PKEY_CTX_free(pctx);
}

void free_keypair() {
    if (keypair) EVP_PKEY_free(keypair);
}

// Helper to convert binary hash to hex string
void bin2hex(const unsigned char* bin, size_t len, char* hex) {
    for (size_t i = 0; i < len; i++) {
        sprintf(hex + (i * 2), "%02x", bin[i]);
    }
    hex[len * 2] = '\0';
}

void compute_hash(Block* block, char* output_hash) {
    char data_to_hash[2048];
    // We combine all fields except the hash itself
    // To ensure exact replication, we use snprintf
    int len = snprintf(data_to_hash, sizeof(data_to_hash), 
             "%d%lld%s%s%s%s%s", 
             block->index, 
             (long long)block->timestamp, 
             block->student_id, 
             block->full_name, 
             block->course_code, 
             block->status, 
             block->previous_hash);
             
    // Note: in a strict blockchain, you'd also include the signature in the hash,
    // but the assignment says "hash of all the above fields combined" and lists signature BEFORE hash.
    // So we include signature too.
    char sig_hex[150] = {0};
    if (block->sig_len > 0) {
        bin2hex(block->signature, block->sig_len, sig_hex);
    }
    
    snprintf(data_to_hash + len, sizeof(data_to_hash) - len, "%s", sig_hex);

    unsigned char hash_out[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)data_to_hash, strlen(data_to_hash), hash_out);
    
    bin2hex(hash_out, SHA256_DIGEST_LENGTH, output_hash);
}

int sign_block(Block* block) {
    // We sign the same data that we hash, excluding the signature and hash fields.
    char data_to_sign[2048];
    snprintf(data_to_sign, sizeof(data_to_sign), 
             "%d%lld%s%s%s%s%s", 
             block->index, 
             (long long)block->timestamp, 
             block->student_id, 
             block->full_name, 
             block->course_code, 
             block->status, 
             block->previous_hash);

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestSignInit(mdctx, NULL, EVP_sha256(), NULL, keypair);
    EVP_DigestSignUpdate(mdctx, data_to_sign, strlen(data_to_sign));
    
    size_t siglen;
    EVP_DigestSignFinal(mdctx, NULL, &siglen);
    
    // Ensure signature buffer is large enough
    if(siglen > sizeof(block->signature)) {
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    
    EVP_DigestSignFinal(mdctx, block->signature, &siglen);
    block->sig_len = siglen;
    
    EVP_MD_CTX_free(mdctx);
    return 1;
}

int verify_signature(Block* block) {
    if (block->sig_len == 0 && block->index == 0) return 1; // Genesis block might not be signed

    char data_to_sign[2048];
    snprintf(data_to_sign, sizeof(data_to_sign), 
             "%d%lld%s%s%s%s%s", 
             block->index, 
             (long long)block->timestamp, 
             block->student_id, 
             block->full_name, 
             block->course_code, 
             block->status, 
             block->previous_hash);

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, keypair);
    EVP_DigestVerifyUpdate(mdctx, data_to_sign, strlen(data_to_sign));
    
    int ret = EVP_DigestVerifyFinal(mdctx, block->signature, block->sig_len);
    EVP_MD_CTX_free(mdctx);
    
    return (ret == 1);
}
