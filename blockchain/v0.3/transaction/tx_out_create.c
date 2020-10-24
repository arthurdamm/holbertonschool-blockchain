#include "transaction.h"

/**
 * tx_out_create - creates a new transaction output structure
 * @amount: the transaction amount
 * @pub: public key of receiver
 * Return: pointer to new structure or NULL
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *t = calloc(1, sizeof(*t));
	unsigned char buf[sizeof(t->amount) + sizeof(t->pub)];

	if (!t)
		return (NULL);
	t->amount = amount;
	memcpy(t->pub, pub, sizeof(t->pub));
	memcpy(buf, &amount, sizeof(amount));
	memcpy(buf + sizeof(amount), pub, EC_PUB_LEN);
	if (!sha256((int8_t const *)buf, sizeof(buf), t->hash))
		return (free(t), NULL);

	return (t);
}
