#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * pool_add - Adds a block to the pending pool (linked list FIFO).
 * @block: Pointer to the block to add the struck
 */
void pool_add(block_t *block)
{
	pool_node_t *node;
	pool_node_t *cur;

	node = malloc(sizeof(*node));
	if (!node)
	{
		fprintf(stderr, "pool_add: memory allocation failed\n");
		return;
	}
	node->block = *block;
	node->next  = NULL;

	if (!g_pool)
	{
		g_pool = node;
		return;
	}
	/* Append to end */
	cur = g_pool;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

/**
 * pool_print - Displays all unconfirmed blocks in the pending pool.
 */
void pool_print(void)
{
	pool_node_t *cur = g_pool;
	int          i   = 1;

	if (!cur)
	{
		printf("\n  [Pending Pool] Empty — no blocks waiting to be mined.\n\n");
		return;
	}

	printf("\n══════════════════════ PENDING POOL ══════════════════════════════\n");
	while (cur)
	{
		printf("\n  Pending #%d\n", i);
		printf("  %-14s: %s\n", "Student ID",   cur->block.student_id);
		printf("  %-14s: %s\n", "Name",         cur->block.student_name);
		printf("  %-14s: %s\n", "Course",       cur->block.course);
		printf("  %-14s: %s\n", "Status",
			(cur->block.status == PRESENT) ? "PRESENT" :
			(cur->block.status == LATE)    ? "LATE"    : "ABSENT");
		printf("  %-14s: %u coins\n", "Reward", cur->block.token_reward);
		printf("  ──────────────────────────────────────────────────────────────\n");
		cur = cur->next;
		i++;
	}
}

/**
 * pool_free - Frees all nodes in the pending pool.
 */
void pool_free(void)
{
	pool_node_t *cur  = g_pool;
	pool_node_t *next;

	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
	g_pool = NULL;
}

/**
 * pool_is_empty - Returns 1 if the pool has no pending blocks, 0 otherwise.
 */
int pool_is_empty(void)
{
	return (g_pool == NULL);
}
