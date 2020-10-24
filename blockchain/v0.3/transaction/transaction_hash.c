#include "transaction.h"

/**
 * transaction_hash - computes hash of given transaction
 * @transaction: pointer to tx to hash
 * @hash_buf: buffer to save hash
 * Return: ptr to buffer or NULL
 */
uint8_t *transaction_hash(transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	ssize_t len, i;
	uint8_t *_buf, *buf;

	if (!transaction)
		return (NULL);
	len = SHA256_DIGEST_LENGTH * 3 * llist_size(transaction->inputs)
		+ SHA256_DIGEST_LENGTH * llist_size(transaction->outputs);
	_buf = buf = calloc(1, len);
	if (!_buf)
		return (NULL);
	for (i = 0; i < llist_size(transaction->inputs); i++)
	{
		tx_in_t *txi = llist_get_node_at(transaction->inputs, i);

		memcpy(buf, txi, SHA256_DIGEST_LENGTH * 3);
		buf += SHA256_DIGEST_LENGTH * 3;
	}
	for (i = 0; i < llist_size(transaction->outputs); i++)
	{
		tx_out_t *txo = llist_get_node_at(transaction->outputs, i);

		memcpy(buf, txo->hash, SHA256_DIGEST_LENGTH);
		buf += SHA256_DIGEST_LENGTH;
	}
	return (sha256((const int8_t *)_buf, len, hash_buf));
}
