#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "blockchain.h"

// Saves the entire state to the specified file (binary block dump)
int save_chain(const char *filename);

// Loads the entire state, rebuilding mempool and checking blocks
int load_chain(const char *filename);

// Verifies the entire loaded chain for correct hashes, signatures, and PoW
int verify_chain();

#endif
