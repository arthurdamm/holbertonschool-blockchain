#include "hblk_crypto.h"

/**
 * ec_sign - signs a message with private key
 * @key: pointer to struct containing key pair
 * @msg: the message to sign
 * @msglen: length of message
 * @sig: address to store signature
 * Return: pointer to sig buffer or NULL
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
	sig_t *sig)
{
	if (!key || !msg || !sig)
		return (NULL);
	if (ECDSA_sign(0, msg, msglen, sig->sig, (unsigned int *)&sig->len,
		(EC_KEY *)key) != 1)
		return (NULL);
	return (sig->sig);
}
