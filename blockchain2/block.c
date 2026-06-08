#include "blockchain.h"
#include <string.h>
#include <stdio.h>

/**
 * block_hash_compute - Computes the SHA-256 hash of a block's data fields necesary.
 *   Hashes: index, nonce, timestamp, student_id, student_name, course,
 *           status, token_reward, tx_id, prev_hash.
 *   The resulting hash is stored in block->hash.
 * @block: Pointer to the block to hash
 */
void block_hash_compute(block_t *block)
{
	uint8_t buf[4096];
	size_t  offset = 0;

	memcpy(buf + offset, &block->index,        sizeof(block->index));
	offset += sizeof(block->index);

	memcpy(buf + offset, &block->nonce,        sizeof(block->nonce));
	offset += sizeof(block->nonce);

	memcpy(buf + offset, &block->timestamp,    sizeof(block->timestamp));
	offset += sizeof(block->timestamp);

	memcpy(buf + offset, block->student_id,    STUDENT_ID_LEN);
	offset += STUDENT_ID_LEN;

	memcpy(buf + offset, block->student_name,  NAME_LEN);
	offset += NAME_LEN;

	memcpy(buf + offset, block->course,        COURSE_LEN);
	offset += COURSE_LEN;

	memcpy(buf + offset, &block->status,       sizeof(block->status));
	offset += sizeof(block->status);

	memcpy(buf + offset, &block->token_reward, sizeof(block->token_reward));
	offset += sizeof(block->token_reward);

	memcpy(buf + offset, block->tx_id,         SHA256_DIGEST_LENGTH);
	offset += SHA256_DIGEST_LENGTH;

	memcpy(buf + offset, block->prev_hash,     SHA256_DIGEST_LENGTH);
	offset += SHA256_DIGEST_LENGTH;

	sha256((int8_t *)buf, offset, block->hash);
}

/**
 * block_create - Creates and partially initialises a new attendance block.
 *   - Sets all metadata fields.
 *   - Computes token_reward (10/5/0).
 *   - Computes tx_id as SHA-256 of a reward-transaction descriptor string.
 *   - Computes the initial block hash (nonce=0).
 *   - Signs the block hash with the student's EC private key.
 *
 * @student:   Pointer to the registered student
 * @course:    Course name/code string
 * @status:    Attendance status (PRESENT/LATE/ABSENT)
 * @prev_hash: SHA-256 hash of the previous block in the chain/pool
 * @index:     Block index
 *
 * Return: Populated block_t value (not yet mined / added to pool)
 */
block_t block_create(student_t *student, const char *course,
		attendance_t status,
		const uint8_t prev_hash[SHA256_DIGEST_LENGTH],
		uint32_t index)
{
	block_t block;
	char    tx_desc[256];

	memset(&block, 0, sizeof(block));

	block.index     = index;
	block.nonce     = 0;
	block.timestamp = time(NULL);
	block.status    = status;

	strncpy(block.student_id,   student->id,   STUDENT_ID_LEN - 1);
	strncpy(block.student_name, student->name, NAME_LEN - 1);
	strncpy(block.course,       course,        COURSE_LEN - 1);
	memcpy(block.prev_hash, prev_hash, SHA256_DIGEST_LENGTH);

	/* Assign token reward */
	if (status == PRESENT)
		block.token_reward = 10;
	else if (status == LATE)
		block.token_reward = 5;
	else
		block.token_reward = 0;

	/* Compute tx_id: SHA-256 of "student_id:reward:timestamp" */
	snprintf(tx_desc, sizeof(tx_desc), "%s:%u:%ld",
		block.student_id, block.token_reward, (long)block.timestamp);
	sha256((int8_t *)tx_desc, strlen(tx_desc), block.tx_id);

	/* Compute initial block hash */
	block_hash_compute(&block);

	/* Sign block hash with student's EC private key (if key exists) */
	if (student->key)
		ec_sign(student->key, block.hash, SHA256_DIGEST_LENGTH, &block.sig);

	return (block);
}
