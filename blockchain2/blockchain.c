#include "blockchain.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * blockchain_init - Initialises the blockchain with a genesis block.
 *   Genesis block: index=0, all hashes zero, no student/reward data.
 *   Appended directly to g_chain without mining (special case).
 */
void blockchain_init(void)
{
	chain_node_t *genesis;
	block_t       gb;

	memset(&gb, 0, sizeof(gb));
	strncpy(gb.student_name, "GENESIS", NAME_LEN - 1);
	strncpy(gb.course,       "N/A",     COURSE_LEN - 1);
	/* Hash genesis block */
	block_hash_compute(&gb);

	genesis = malloc(sizeof(*genesis));
	if (!genesis)
	{
		fprintf(stderr, "blockchain_init: memory allocation failed\n");
		return;
	}
	genesis->block = gb;
	genesis->next  = NULL;
	g_chain        = genesis;
	g_chain_length = 1;
}

/**
 * blockchain_last - Returns the last confirmed block node.
 * Return: Pointer to last chain_node_t, or NULL if chain is empty.
 */
chain_node_t *blockchain_last(void)
{
	chain_node_t *cur = g_chain;

	if (!cur)
		return (NULL);
	while (cur->next)
		cur = cur->next;
	return (cur);
}

/**
 * blockchain_append - Appends a confirmed (mined) block to the chain.
 * @block: Pointer to the block to append
 */
void blockchain_append(block_t *block)
{
	chain_node_t *node;
	chain_node_t *last;

	node = malloc(sizeof(*node));
	if (!node)
	{
		fprintf(stderr, "blockchain_append: memory allocation failed\n");
		return;
	}
	node->block = *block;
	node->next  = NULL;

	last = blockchain_last();
	if (last)
		last->next = node;
	else
		g_chain = node;

	g_chain_length++;
}

/**
 * print_hash - Prints a SHA-256 hash as a 64-char hex string.
 * @hash: Array of SHA256_DIGEST_LENGTH bytes
 */
static void print_hash(const uint8_t hash[SHA256_DIGEST_LENGTH])
{
	int i;

	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", hash[i]);
}

/**
 * attendance_str - Returns a string label for an attendance status.
 */
static const char *attendance_str(attendance_t s)
{
	if (s == PRESENT) return "PRESENT";
	if (s == LATE)    return "LATE";
	return "ABSENT";
}

/**
 * blockchain_print - Prints every confirmed block in the chain.
 */
void blockchain_print(void)
{
	chain_node_t *cur = g_chain;
	int           i   = 0;
	int           sig_ok;

	printf("\n══════════════════════ CONFIRMED BLOCKCHAIN ══════════════════════\n");
	while (cur)
	{
		printf("\n  Block #%u\n", cur->block.index);
		printf("  %-14s: %s\n", "Student ID",   cur->block.student_id[0]
			? cur->block.student_id : "N/A");
		printf("  %-14s: %s\n", "Name",         cur->block.student_name);
		printf("  %-14s: %s\n", "Course",       cur->block.course);
		printf("  %-14s: %s\n", "Status",       attendance_str(cur->block.status));
		printf("  %-14s: %u coins\n", "Reward",  cur->block.token_reward);
		printf("  %-14s: %ld\n",      "Timestamp", (long)cur->block.timestamp);
		printf("  %-14s: ", "Hash");   print_hash(cur->block.hash); printf("\n");
		printf("  %-14s: ", "Prev Hash"); print_hash(cur->block.prev_hash);
		printf("\n");
		printf("  %-14s: ", "Tx ID"); print_hash(cur->block.tx_id); printf("\n");
		printf("  %-14s: %u\n", "Nonce (PoW)", cur->block.nonce);

		/* Verify signature if block has one */
		if (cur->block.index > 0 && cur->block.sig.len > 0)
		{
			/* Find student key */
			sig_ok = 0;
			for (i = 0; i < g_student_count; i++)
			{
				if (strcmp(g_students[i].id, cur->block.student_id) == 0
					&& g_students[i].key)
				{
					sig_ok = ec_verify(g_students[i].key,
						cur->block.hash,
						SHA256_DIGEST_LENGTH,
						&cur->block.sig);
					break;
				}
			}
			printf("  %-14s: %s\n", "Signature",
				sig_ok ? "\033[32mVALID\033[0m"
				       : "\033[31mINVALID / UNVERIFIED\033[0m");
		}

		printf("  ──────────────────────────────────────────────────────────────\n");
		cur = cur->next;
	}
}

/**
 * blockchain_free - Frees all nodes in the confirmed chain.
 */
void blockchain_free(void)
{
	chain_node_t *cur  = g_chain;
	chain_node_t *next;

	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
	g_chain = NULL;
	g_chain_length = 0;
}
