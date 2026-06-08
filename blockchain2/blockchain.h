#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include "../crypto/hblk_crypto.h"

/* ─── Constants ─────────────────────────────────────────────── */
#define MAX_STUDENTS    64
#define STUDENT_ID_LEN  32
#define NAME_LEN        64
#define COURSE_LEN      64
#define TX_FEE          1      /* coins deducted per confirmed reward tx */
#define POOL_FEE_PCT    2      /* % deducted before pool reward share    */
#define MINING_REWARD   25     /* coins awarded to the miner   (Student)          */
#define MIN_DIFFICULTY  1
#define MAX_DIFFICULTY  4
#define DEFAULT_DIFF    2

/* ─── Attendance status ──────────────────────────────────────── */
typedef enum
{
	PRESENT = 0,
	LATE    = 1,
	ABSENT  = 2
} attendance_t;

/* ─── Block ──────────────────────────────────────────────────── */
/**
 * struct block_s - Blockchain block structure
 * @index:        Block index in the chain
 * @nonce:        Proof-of-work nonce (incremented during mining)
 * @timestamp:    Unix timestamp of attendance event
 * @student_id:   Student ID string
 * @student_name: Student full name
 * @course:       Course code / name
 * @status:       Attendance status (PRESENT/LATE/ABSENT)
 * @token_reward: Coins earned (10/5/0)
 * @tx_id:        SHA-256 of the reward transaction string
 * @hash:         SHA-256 hash of this block (after PoW)
 * @prev_hash:    SHA-256 hash of the previous block
 * @sig:          ECDSA signature of this block's hash
 */
typedef struct block_s
{
	uint32_t     index;
	uint32_t     nonce;
	time_t       timestamp;
	char         student_id[STUDENT_ID_LEN];
	char         student_name[NAME_LEN];
	char         course[COURSE_LEN];
	attendance_t status;
	uint32_t     token_reward;
	uint8_t      tx_id[SHA256_DIGEST_LENGTH];
	uint8_t      hash[SHA256_DIGEST_LENGTH];
	uint8_t      prev_hash[SHA256_DIGEST_LENGTH];
	sig_t        sig;
} block_t;

/* ─── Confirmed chain ────────────────────────────────────────── */
/**
 * struct chain_node_s - Node in the confirmed blockchain
 */
typedef struct chain_node_s
{
	block_t              block;
	struct chain_node_s *next;
} chain_node_t;

/* ─── Pending pool ───────────────────────────────────────────── */
/**
 * struct pool_node_s - Node in the pending (unconfirmed) pool
 */
typedef struct pool_node_s
{
	block_t             block;
	struct pool_node_s *next;
} pool_node_t;

/* ─── UTXO model ─────────────────────────────────────────────── */
/**
 * struct utxo_s - Unspent Transaction Output
 * @tx_id:        ID of the transaction that created this UTXO
 * @output_index: Index within that transaction's outputs
 * @owner:        Student ID that owns this UTXO
 * @amount:       Coin amount
 * @spent:        1 = already spent, 0 = unspent
 */
typedef struct utxo_s
{
	uint8_t       tx_id[SHA256_DIGEST_LENGTH];
	uint32_t      output_index;
	char          owner[STUDENT_ID_LEN];
	uint32_t      amount;
	int           spent;
	struct utxo_s *next;
} utxo_t;

/* ─── Account / Balance model ────────────────────────────────── */
/**
 * struct tx_history_s - One entry in a student's transaction log
 */
typedef struct tx_history_s
{
	char                 sender[STUDENT_ID_LEN];
	char                 recipient[STUDENT_ID_LEN];
	uint32_t             amount;
	uint32_t             fee;
	uint32_t             nonce;
	struct tx_history_s *next;
} tx_history_t;

/**
 * struct account_s - Student token account
 * @student_id: Matches the student registry ID
 * @balance:    Current token balance
 * @nonce:      Increments with every outgoing transaction
 * @history:    Linked list of past transactions
 */
typedef struct account_s
{
	char              student_id[STUDENT_ID_LEN];
	uint32_t          balance;
	uint32_t          nonce;
	tx_history_t     *history;
	struct account_s *next;
} account_t;

/* ─── Student registry ───────────────────────────────────────── */
/**
 * struct student_s - Registered student
 */
typedef struct student_s
{
	char    id[STUDENT_ID_LEN];
	char    name[NAME_LEN];
	EC_KEY *key;  /* EC key pair for signing attendance blocks */
} student_t;

/* ─── Transaction model selector ─────────────────────────────── */
typedef enum
{
	MODEL_UTXO    = 0,
	MODEL_ACCOUNT = 1
} tx_model_t;

/* ─── Global state (declared in main.c) ─────────────────────── */
extern student_t    g_students[MAX_STUDENTS];
extern int          g_student_count;
extern chain_node_t *g_chain;
extern pool_node_t  *g_pool;
extern utxo_t       *g_utxo_set;
extern account_t    *g_accounts;
extern tx_model_t    g_model;
extern int           g_difficulty;
extern uint32_t      g_chain_length;

/* ─── Function prototypes ────────────────────────────────────── */

/* block.c */
void     block_hash_compute(block_t *block);
block_t  block_create(student_t *student, const char *course,
		attendance_t status, const uint8_t prev_hash[SHA256_DIGEST_LENGTH],
		uint32_t index);

/* blockchain.c */
void          blockchain_init(void);
void          blockchain_append(block_t *block);
void          blockchain_print(void);
void          blockchain_free(void);
chain_node_t *blockchain_last(void);

/* pending_pool.c */
void pool_add(block_t *block);
void pool_print(void);
void pool_free(void);
int  pool_is_empty(void);

/* utxo.c */
utxo_t  *utxo_add(const uint8_t tx_id[SHA256_DIGEST_LENGTH],
		uint32_t output_index, const char *owner, uint32_t amount);
uint32_t utxo_balance(const char *owner);
int      utxo_apply_reward(const block_t *block);
int      utxo_transfer(const char *sender, const char *recipient,
		uint32_t amount);
void     utxo_print_all(void);
void     utxo_free(void);

/* account.c */
account_t *account_find_or_create(const char *student_id);
int        account_apply_reward(const block_t *block);
int        account_transfer(const char *sender, const char *recipient,
		uint32_t amount, uint32_t expected_nonce);
void       account_print_history(const char *student_id);
void       account_print_balances(void);
void       accounts_free(void);

/* mining.c */
int  mine_solo(int difficulty);
void mine_pool(int difficulty, int num_miners);
void mine_cloud(int difficulty, int rounds, uint32_t rental_fee_per_round);

#endif /* BLOCKCHAIN_H */
