#include "blockchain.h"

/**
 * block_destroy - frees a block
 * @block: pointer to block to free
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;
	transaction_destroy(block->transactions);
	free(block);
}
