#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ─── Shared PoW helper ─────────────────────────────────────── */

/**
 * meets_difficulty - Checks whether a block hash has 'difficulty'
 *   leading zero nibbles (half-bytes), which equals difficulty/2 leading
 *   zero bytes + a mask check when difficulty is odd.
 *
 *   This gives a clean decimal difficulty where 1 = "0x" in hex,
 *   2 = "00" and so on, matching the assignment requirement.
 *
 * @hash:       SHA-256 hash buffer
 * @difficulty: Number of leading zero hex characters required (1–4)
 * Return: 1 if target met, 0 otherwise.
 */
static int meets_difficulty(const uint8_t hash[SHA256_DIGEST_LENGTH],
		int difficulty)
{
	int i;

	/* Each byte covers 2 hex chars. Check full zero bytes first. */
	for (i = 0; i < difficulty / 2; i++)
		if (hash[i] != 0x00)
			return (0);

	/* If difficulty is odd, the next nibble (high 4 bits) must be 0. */
	if (difficulty % 2 == 1)
		if ((hash[difficulty / 2] & 0xF0) != 0x00)
			return (0);
	return (1);
}

/**
 * apply_confirmed_block - After mining confirms a block:
 *   1. Appends block to the blockchain.
 *   2. Credits token reward via the active transaction model.
 *
 * @block: Pointer to the freshly mined block
 */
static void apply_confirmed_block(block_t *block)
{
	blockchain_append(block);

	if (g_model == MODEL_UTXO)
		utxo_apply_reward(block);
	else
		account_apply_reward(block);
}

/* ─── 4.1 Solo Mining ────────────────────────────────────────── */

/**
 * mine_solo - Solo miner confirms all pending blocks one by one.
 *   For each pending block:
 *     - Increments nonce and recomputes hash until target difficulty met.
 *     - Prints attempt count.
 *     - Appends block to chain and updates token balances.
 *   Miner receives MINING_REWARD for the work (credited to "MINER").
 *
 * @difficulty: Number of leading zero hex chars required (1–4)
 * Return: 1 if at least one block was mined, 0 if pool was empty.
 */
int mine_solo(int difficulty)
{
	pool_node_t *cur = g_pool;
	unsigned long attempts;
	uint32_t      total_reward = 0;
	int           mined        = 0;

	if (pool_is_empty())
	{
		printf("\n  [Solo Mining] Pending pool is empty — nothing to mine.\n\n");
		return (0);
	}

	printf("\n══════════════════════ SOLO MINING (diff=%d) ════════════════════\n",
		difficulty);

	while (cur)
	{
		attempts      = 0;
		cur->block.nonce = 0;

		printf("\n  Mining block for %s (%s)...\n",
			cur->block.student_name, cur->block.student_id);

		do {
			cur->block.nonce++;
			block_hash_compute(&cur->block);
			attempts++;
		} while (!meets_difficulty(cur->block.hash, difficulty));

		printf("  ✓ Block mined!  nonce=%u  attempts=%lu\n",
			cur->block.nonce, attempts);
		apply_confirmed_block(&cur->block);
		total_reward += MINING_REWARD;
		mined++;
		cur = cur->next;
	}

	pool_free();

	printf("\n  Solo miner earned %u coins (MINING_REWARD × %d blocks).\n",
		total_reward, mined);
	printf("══════════════════════════════════════════════════════════════════\n\n");
	return (1);
}

/* ─── 4.2 Pool Mining ────────────────────────────────────────── */

#define MAX_POOL_MINERS 10

/**
 * mine_pool - Multiple simulated miners share a mining round.
 *   - Each miner gets a random hash rate (50–500 attempts/round).
 *   - Block is considered mined after sum of all miner attempts reaches
 *     a threshold (simulated; actual PoW is done by the pool collectively).
 *   - Reward is split proportionally minus a 2% pool fee.
 *   - Prints a formatted results table.
 *
 * @difficulty:  Difficulty level (affects actual PoW on the first block)
 * @num_miners:  Number of simulated pool miners (2–MAX_POOL_MINERS)
 */
void mine_pool(int difficulty, int num_miners)
{
	pool_node_t *cur = g_pool;
	unsigned long miner_attempts[MAX_POOL_MINERS];
	unsigned long total_attempts;
	double        share_pct;
	double        gross_reward;
	double        pool_fee;
	double        net_reward;
	double        pool_total_reward;
	int           i;
	unsigned long attempts;
	int           mined = 0;

	if (num_miners < 2) num_miners = 2;
	if (num_miners > MAX_POOL_MINERS) num_miners = MAX_POOL_MINERS;

	if (pool_is_empty())
	{
		printf("\n  [Pool Mining] Pending pool is empty — nothing to mine.\n\n");
		return;
	}

	srand((unsigned int)time(NULL));

	printf("\n══════════════════════ POOL MINING (diff=%d, miners=%d) ═════════\n",
		difficulty, num_miners);

	while (cur)
	{
		/* Simulate each miner's hash contribution */
		total_attempts = 0;
		for (i = 0; i < num_miners; i++)
		{
			miner_attempts[i]  = (unsigned long)(rand() % 451 + 50);
			total_attempts    += miner_attempts[i];
		}

		/* Perform actual PoW on block */
		attempts         = 0;
		cur->block.nonce = 0;
		do {
			cur->block.nonce++;
			block_hash_compute(&cur->block);
			attempts++;
		} while (!meets_difficulty(cur->block.hash, difficulty));

		apply_confirmed_block(&cur->block);
		mined++;

		/* Calculate rewards */
		gross_reward    = (double)MINING_REWARD;
		pool_fee        = gross_reward * POOL_FEE_PCT / 100.0;
		pool_total_reward = gross_reward - pool_fee;

		printf("\n  Block for %s mined in %lu PoW attempts\n",
			cur->block.student_name, attempts);
		printf("  Pool fee (%.0f%%): %.2f coins  |  Distributable: %.2f coins\n",
			(double)POOL_FEE_PCT, pool_fee, pool_total_reward);
		printf("\n  %-10s %-12s %-12s %-12s %-12s\n",
			"Miner ID", "Attempts", "Share %", "Gross", "Reward");
		printf("  %-10s %-12s %-12s %-12s %-12s\n",
			"──────────", "────────────", "────────────",
			"────────────", "────────────");

		for (i = 0; i < num_miners; i++)
		{
			share_pct  = (double)miner_attempts[i] / (double)total_attempts * 100.0;
			net_reward = pool_total_reward * ((double)miner_attempts[i]
				/ (double)total_attempts);
			printf("  %-10d %-12lu %-12.2f %-12.2f %.4f\n",
				i + 1, miner_attempts[i], share_pct,
				gross_reward * ((double)miner_attempts[i] / (double)total_attempts),
				net_reward);
		}
		printf("\n");
		cur = cur->next;
	}

	pool_free();
	printf("  [Pool] %d block(s) confirmed.\n", mined);
	printf("══════════════════════════════════════════════════════════════════\n\n");
}

/* ─── 4.3 Cloud Mining ───────────────────────────────────────── */

/**
 * mine_cloud - Simulates rented mining power over multiple rounds.
 *   - Each round: deduct rental_fee, credit mining_reward.
 *   - If cumulative fees exceed cumulative rewards → print warning.
 *   - Mines all pending blocks in round 1 using standard PoW.
 *   - Prints gross earnings, total fees, net profit summary.
 *
 * @difficulty:          PoW difficulty for actually mining pending blocks
 * @rounds:              Rental duration in rounds (1–5)
 * @rental_fee_per_round: Coins paid per round for the rental
 */
void mine_cloud(int difficulty, int rounds, uint32_t rental_fee_per_round)
{
	uint32_t      cumulative_fees    = 0;
	uint32_t      cumulative_rewards = 0;
	int           r;
	pool_node_t  *cur;
	unsigned long attempts;

	if (rounds < 1) rounds = 1;
	if (rounds > 5) rounds = 5;

	printf("\n══════════════════════ CLOUD MINING (diff=%d, rounds=%d) ════════\n",
		difficulty, rounds);
	printf("  Rental fee: %u coins/round\n\n", rental_fee_per_round);
	printf("  %-6s %-14s %-14s %-14s %-12s\n",
		"Round", "Fee Paid", "Reward", "Cumul.Fees", "Cumul.Rew");
	printf("  %-6s %-14s %-14s %-14s %-12s\n",
		"──────", "──────────────", "──────────────",
		"──────────────", "────────────");

	for (r = 1; r <= rounds; r++)
	{
		cumulative_fees    += rental_fee_per_round;
		cumulative_rewards += MINING_REWARD;

		printf("  %-6d %-14u %-14d %-14u %-12u",
			r, rental_fee_per_round, MINING_REWARD,
			cumulative_fees, cumulative_rewards);

		if (cumulative_fees > cumulative_rewards)
			printf("  ⚠ UNPROFITABLE");
		printf("\n");

		/* In round 1 actually mine the pending blocks */
		if (r == 1)
		{
			cur = g_pool;
			while (cur)
			{
				attempts         = 0;
				cur->block.nonce = 0;
				do {
					cur->block.nonce++;
					block_hash_compute(&cur->block);
					attempts++;
				} while (!meets_difficulty(cur->block.hash, difficulty));

				apply_confirmed_block(&cur->block);
				cur = cur->next;
			}
			pool_free();
		}
	}

	printf("\n  ─────────────────────────────────────────────────────────────\n");
	printf("  Gross Earnings : %u coins\n", cumulative_rewards);
	printf("  Total Fees Paid: %u coins\n", cumulative_fees);

	if (cumulative_rewards > cumulative_fees)
		printf("  Net Profit     : \033[32m+%u coins\033[0m\n",
			cumulative_rewards - cumulative_fees);
	else
		printf("  Net Profit     : \033[31m-%u coins (LOSS)\033[0m\n",
			cumulative_fees - cumulative_rewards);

	printf("══════════════════════════════════════════════════════════════════\n\n");
}
