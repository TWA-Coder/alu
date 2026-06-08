#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── Internal helpers ───────────────────────────────────────── */

/**
 * utxo_alloc - Allocates and initialises a new UTXO node.
 */
static utxo_t *utxo_alloc(const uint8_t tx_id[SHA256_DIGEST_LENGTH],
		uint32_t output_index, const char *owner, uint32_t amount)
{
	utxo_t *u = malloc(sizeof(*u));

	if (!u)
		return (NULL);
	memcpy(u->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	u->output_index = output_index;
	strncpy(u->owner, owner, STUDENT_ID_LEN - 1);
	u->owner[STUDENT_ID_LEN - 1] = '\0';
	u->amount = amount;
	u->spent  = 0;
	u->next   = NULL;
	return (u);
}

/* ─── Public API ─────────────────────────────────────────────── */

/**
 * utxo_add - Appends a new UTXO to the global UTXO set.
 * @tx_id:        Transaction ID that created this output
 * @output_index: Index within that transaction
 * @owner:        Student ID of the UTXO owner
 * @amount:       Coin amount
 *
 * Return: Pointer to the new UTXO, or NULL on failure.
 */
utxo_t *utxo_add(const uint8_t tx_id[SHA256_DIGEST_LENGTH],
		uint32_t output_index, const char *owner, uint32_t amount)
{
	utxo_t *node = utxo_alloc(tx_id, output_index, owner, amount);
	utxo_t *cur;

	if (!node)
		return (NULL);

	if (!g_utxo_set)
	{
		g_utxo_set = node;
		return (node);
	}
	cur = g_utxo_set;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
	return (node);
}

/**
 * utxo_balance - Sums all unspent UTXOs owned by a student.
 * @owner: Student ID string
 *
 * Return: Total unspent coin balance.
 */
uint32_t utxo_balance(const char *owner)
{
	utxo_t   *cur = g_utxo_set;
	uint32_t  bal = 0;

	while (cur)
	{
		if (!cur->spent && strcmp(cur->owner, owner) == 0)
			bal += cur->amount;
		cur = cur->next;
	}
	return (bal);
}

/**
 * utxo_apply_reward - Called when a block is confirmed (after mining).
 *   - If token_reward > 0 and reward > TX_FEE: creates a UTXO for
 *     (token_reward - TX_FEE) coins credited to the student.
 *   - ABSENT blocks (reward == 0) produce no UTXO.
 *
 * @block: Confirmed block
 * Return: 1 on success, 0 if no UTXO created (ABSENT or fee >= reward).
 */
int utxo_apply_reward(const block_t *block)
{
	uint32_t net;

	if (block->token_reward == 0)
		return (0);   /* ABSENT — no transaction */

	if (block->token_reward <= TX_FEE)
	{
		printf("  [UTXO] Reward (%u) <= fee (%d) — no UTXO created for %s\n",
			block->token_reward, TX_FEE, block->student_id);
		return (0);
	}

	net = block->token_reward - TX_FEE;
	utxo_add(block->tx_id, 0, block->student_id, net);
	printf("  [UTXO] +%u coins credited to %s (fee %d deducted)\n",
		net, block->student_id, TX_FEE);
	return (1);
}

/**
 * utxo_transfer - Performs a manual token transfer between students.
 *   Validates: sender has sufficient UTXOs, fee covers, no double-spend.
 *   Creates output UTXO for recipient and change UTXO for sender if needed.
 *
 * @sender:    Student ID of the sender
 * @recipient: Student ID of the recipient
 * @amount:    Coins to send (fee is added on top)
 *
 * Return: 1 on success, 0 on failure.
 */
int utxo_transfer(const char *sender, const char *recipient, uint32_t amount)
{
	uint32_t total_needed = amount + TX_FEE;
	uint32_t collected    = 0;
	utxo_t  *cur;
	uint8_t  new_tx_id[SHA256_DIGEST_LENGTH];
	char     tx_desc[256];

	if (utxo_balance(sender) < total_needed)
	{
		printf("  [UTXO] Transfer rejected: insufficient balance "
			"(have %u, need %u incl. %d fee)\n",
			utxo_balance(sender), total_needed, TX_FEE);
		return (0);
	}

	/* Generate a new tx_id for this transfer */
	snprintf(tx_desc, sizeof(tx_desc), "transfer:%s:%s:%u:%ld",
		sender, recipient, amount, (long)time(NULL));
	sha256((int8_t *)tx_desc, strlen(tx_desc), new_tx_id);

	/* Mark UTXOs as spent until we have enough */
	cur = g_utxo_set;
	while (cur && collected < total_needed)
	{
		if (!cur->spent && strcmp(cur->owner, sender) == 0)
		{
			collected   += cur->amount;
			cur->spent   = 1;
		}
		cur = cur->next;
	}

	/* Create output UTXO for recipient */
	utxo_add(new_tx_id, 0, recipient, amount);

	/* Return change to sender if any */
	if (collected > total_needed)
		utxo_add(new_tx_id, 1, sender, collected - total_needed);

	printf("  [UTXO] Transfer: %s → %s  amount=%u  fee=%d  change=%u\n",
		sender, recipient, amount, TX_FEE,
		(collected > total_needed) ? collected - total_needed : 0);
	return (1);
}

/**
 * utxo_print_all - Prints the full UTXO set showing all outputs and status.
 */
void utxo_print_all(void)
{
	utxo_t *cur = g_utxo_set;
	int     i   = 1;
	int     j;

	printf("\n══════════════════════ UTXO SET ══════════════════════════════════\n");
	if (!cur)
	{
		printf("  (empty)\n\n");
		return;
	}
	printf("  %-4s %-20s %-10s %-10s %s\n",
		"#", "Owner", "Amount", "Status", "Tx-ID (first 8 bytes)");
	printf("  %-4s %-20s %-10s %-10s %s\n",
		"────", "────────────────────", "──────────",
		"──────────", "────────────────");
	while (cur)
	{
		printf("  %-4d %-20s %-10u %-10s ",
			i, cur->owner, cur->amount,
			cur->spent ? "SPENT" : "UNSPENT");
		for (j = 0; j < 8; j++)
			printf("%02x", cur->tx_id[j]);
		printf("...\n");
		i++;
		cur = cur->next;
	}
	printf("\n");
}


/**
 * utxo_free - Frees all UTXO nodes.
 */
void utxo_free(void)
{
	utxo_t *cur  = g_utxo_set;
	utxo_t *next;

	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
	g_utxo_set = NULL;
}
