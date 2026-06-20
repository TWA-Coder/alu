
# ALU Health Insurance Blockchain Management System: Technical Report

## 1. Blockchain Architecture and Data Structures

The **ALU Health Insurance Blockchain Management System** is built on a custom blockchain implementation written in C. The blockchain consists of a sequence of **Blocks**, each containing a header and a collection of validated transactions.

Each block header stores essential metadata, including:

- Block timestamp
- Mining difficulty
- Miner identifier
- Transaction payload
- Merkle root
- Previous block hash
- Nonce

The `previous_hash` field cryptographically links each block to its predecessor, ensuring chain integrity. Any modification to a historical block changes its hash, invalidating all subsequent blocks and making unauthorized tampering immediately detectable.

---

## 2. Transaction Hashing and Merkle Tree Generation

Transactions are treated as immutable payloads and do not permanently store their individual SHA-256 hashes within the transaction structure.

Instead, transaction hashes are generated dynamically whenever they are required for:

- Digital signature generation
- Signature verification
- Blockchain validation

This functionality is implemented through the `compute_sha256()` function.

To efficiently verify the integrity of all transactions contained within a block, the blockchain constructs a **Merkle Tree**. The implementation, located in `src/blockchain.c`, iteratively hashes pairs of transaction hashes until a single hash remains.

The final hash is stored as the **Merkle Root** in the block header and serves as a cryptographic fingerprint representing every transaction included in the block.

---

## 3. Account Nonces vs. Block Nonces

### Account Nonces

Account nonces are maintained by the ledger to prevent replay attacks.

Every account keeps an incrementing sequence number. Before accepting a new transaction, the network verifies that the submitted nonce matches the expected value (`N + 1`).

Because the nonce is included in the transaction signature, attackers cannot modify or replay previously submitted transactions.

### Block Nonces

Block nonces are independent of account nonces and are used exclusively during the mining process.

Miners repeatedly increment the block nonce while recomputing the block hash until a hash satisfying the network's Proof-of-Work (PoW) difficulty requirement is produced.

This mining process is implemented within the `mine_solo()` function.

---

## 4. Automated Reinsurance Distribution

The system incorporates an automated reinsurance mechanism during premium settlement.

Whenever a premium payment is processed, two outputs are automatically generated:

- **95%** of the premium is transferred to the primary insurance provider.
- **5%** is automatically allocated to the reinsurance fund.

This distribution occurs programmatically without requiring manual intervention, ensuring consistent, transparent, and auditable allocation of funds.

---

## 5. Security and Fraud Detection

The blockchain integrates a fraud detection module through the `check_fraud_heuristics()` function.

Each submitted insurance claim is evaluated against predefined fraud detection rules. Transactions exhibiting abnormal characteristics, such as unusually large claim amounts or suspicious behavioral patterns, are flagged as suspicious.

When a transaction is identified as potentially fraudulent, the function returns `STATUS_SUSPICIOUS`, allowing the system to suspend normal processing and preserve the transaction for further investigation.

This mechanism enhances the security of the blockchain while reducing the likelihood of fraudulent insurance claims being processed automatically.

