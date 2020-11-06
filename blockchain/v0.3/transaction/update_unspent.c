#include "transaction.h"

/**
 * update_unspent - updates list of UTXOs
 * @transactions: list of validate txs
 * @block_hash: hash of block containing txs
 * @all_unspent: list of all UTXOs
 * Return: new UTXO list
 */
llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	(void)transactions;
	(void)block_hash;
	(void)all_unspent;
	return (NULL);
}
