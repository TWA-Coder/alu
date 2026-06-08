#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── Internal helpers ───────────────────────────────────────── */

/**
 * history_append - Appends one entry to an account's transaction history.
 */
static void history_append(account_t *acc, const char *sender,
		const char *recipient, uint32_t amount, uint32_t fee,
		uint32_t nonce)
{
	tx_history_t *entry = malloc(sizeof(*entry));
	tx_history_t *cur;

	if (!entry)
		return;
	strncpy(entry->sender,    sender,    STUDENT_ID_LEN - 1);
	strncpy(entry->recipient, recipient, STUDENT_ID_LEN - 1);
	entry->sender[STUDENT_ID_LEN - 1]    = '\0';
	entry->recipient[STUDENT_ID_LEN - 1] = '\0';
	entry->amount = amount;
	entry->fee    = fee;
	entry->nonce  = nonce;
	entry->next   = NULL;

	if (!acc->history)
	{
		acc->history = entry;
		return;
	}
	cur = acc->history;
	while (cur->next)
		cur = cur->next;
	cur->next = entry;
}

/* ─── Public API ─────────────────────────────────────────────── */

/**
 * account_find_or_create - Returns the account for a student ID.
 *   Creates a new account (balance=0, nonce=0) if none exists.
 *
 * @student_id: Student ID string
 * Return: Pointer to account_t, or NULL on allocation failure.
 */
account_t *account_find_or_create(const char *student_id)
{
	account_t *cur = g_accounts;
	account_t *new_acc;

	while (cur)
	{
		if (strcmp(cur->student_id, student_id) == 0)
			return (cur);
		cur = cur->next;
	}

	/* Create new */
	new_acc = malloc(sizeof(*new_acc));
	if (!new_acc)
		return (NULL);
	memset(new_acc, 0, sizeof(*new_acc));
	strncpy(new_acc->student_id, student_id, STUDENT_ID_LEN - 1);

	/* Prepend to list */
	new_acc->next = g_accounts;
	g_accounts    = new_acc;
	return (new_acc);
}

/**
 * account_apply_reward - Called when a block is confirmed.
 *   Credits (token_reward - TX_FEE) to the student's account.
 *   ABSENT blocks (reward == 0) produce no credit.
 *   Records the transaction in the student's history.
 *
 * @block: Confirmed block
 * Return: 1 on success, 0 if no credit applied.
 */
int account_apply_reward(const block_t *block)
{
	account_t *acc;
	uint32_t   net;

	if (block->token_reward == 0)
		return (0);  /* ABSENT — no transaction */

	if (block->token_reward <= TX_FEE)
	{
		printf("  [Account] Reward (%u) <= fee (%d) — no credit for %s\n",
			block->token_reward, TX_FEE, block->student_id);
		return (0);
	}

	acc = account_find_or_create(block->student_id);
	if (!acc)
		return (0);

	net          = block->token_reward - TX_FEE;
	acc->balance += net;

	/* Record in history (using "SYSTEM" as sender for mining rewards) */
	history_append(acc, "SYSTEM", block->student_id, net, TX_FEE, 0);

	printf("  [Account] +%u coins credited to %s (fee %d deducted) | "
		"Balance: %u\n",
		net, block->student_id, TX_FEE, acc->balance);
	return (1);
}

/**
 * account_transfer - Performs a manual token transfer.
 *   Validates: sufficient balance, correct nonce.
 *   Deducts TX_FEE from sender in addition to the amount.
 *   Increments sender's nonce on success.
 *
 * @sender:         Sender student ID
 * @recipient:      Recipient student ID
 * @amount:         Coins to transfer
 * @expected_nonce: Must match sender's current nonce
 *
 * Return: 1 on success, 0 on failure.
 */
int account_transfer(const char *sender, const char *recipient,
		uint32_t amount, uint32_t expected_nonce)
{
	account_t *s_acc;
	account_t *r_acc;
	uint32_t   total;

	s_acc = account_find_or_create(sender);
	if (!s_acc)
		return (0);

	/* Validate nonce */
	if (s_acc->nonce != expected_nonce)
	{
		printf("  [Account] Transfer rejected: nonce mismatch "
			"(expected %u, got %u)\n", s_acc->nonce, expected_nonce);
		return (0);
	}

	total = amount + TX_FEE;

	/* Validate balance */
	if (s_acc->balance < total)
	{
		printf("  [Account] Transfer rejected: insufficient balance "
			"(have %u, need %u incl. %d fee)\n",
			s_acc->balance, total, TX_FEE);
		return (0);
	}

	r_acc = account_find_or_create(recipient);
	if (!r_acc)
		return (0);

	/* Execute transfer */
	s_acc->balance -= total;
	r_acc->balance += amount;
	s_acc->nonce++;

	/* Record in history for both parties */
	history_append(s_acc, sender, recipient, amount, TX_FEE, expected_nonce);
	history_append(r_acc, sender, recipient, amount, TX_FEE, expected_nonce);

	printf("  [Account] Transfer: %s → %s  amount=%u  fee=%d  nonce=%u\n"
		"           %s balance: %u  |  %s balance: %u\n",
		sender, recipient, amount, TX_FEE, expected_nonce,
		sender, s_acc->balance, recipient, r_acc->balance);
	return (1);
}

/**
 * account_print_history - Prints the full transaction history for a student.
 * @student_id: Student ID to look up
 */
void account_print_history(const char *student_id)
{
	account_t    *acc = account_find_or_create(student_id);
	tx_history_t *h;
	int           i   = 1;

	if (!acc)
		return;

	printf("\n══════════════════════ TX HISTORY: %s ═══════════════\n",
		student_id);
	if (!acc->history)
	{
		printf("  (no transactions)\n\n");
		return;
	}
	printf("  %-4s %-15s %-15s %-8s %-6s %s\n",
		"#", "Sender", "Recipient", "Amount", "Fee", "Nonce");
	printf("  %-4s %-15s %-15s %-8s %-6s %s\n",
		"────", "───────────────", "───────────────",
		"────────", "──────", "──────");
	h = acc->history;
	while (h)
	{
		printf("  %-4d %-15s %-15s %-8u %-6u %u\n",
			i, h->sender, h->recipient, h->amount, h->fee, h->nonce);
		h = h->next;
		i++;
	}
	printf("  Current Balance: %u  |  Nonce: %u\n\n",
		acc->balance, acc->nonce);
}

/**
 * account_print_balances - Prints all student account balances.
 */
void account_print_balances(void)
{
	account_t *cur = g_accounts;

	printf("\n══════════════════════ ACCOUNT BALANCES ══════════════════════════\n");
	if (!cur)
	{
		printf("  (no accounts)\n\n");
		return;
	}
	printf("  %-20s %-12s %s\n", "Student ID", "Balance", "Nonce");
	printf("  %-20s %-12s %s\n", "────────────────────", "────────────", "──────");
	while (cur)
	{
		printf("  %-20s %-12u %u\n",
			cur->student_id, cur->balance, cur->nonce);
		cur = cur->next;
	}
	printf("\n");
}

/**
 * accounts_free - Frees all account nodes and their transaction histories.
 */
void accounts_free(void)
{
	account_t    *acc  = g_accounts;
	account_t    *anext;
	tx_history_t *h;
	tx_history_t *hnext;

	while (acc)
	{
		h = acc->history;
		while (h)
		{
			hnext = h->next;
			free(h);
			h = hnext;
		}
		anext = acc->next;
		free(acc);
		acc = anext;
	}
	g_accounts = NULL;
}
