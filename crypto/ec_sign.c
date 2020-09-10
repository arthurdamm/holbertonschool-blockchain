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
	EVP_MD_CTX *ctx = NULL;
	EVP_PKEY *pkey = NULL;
	int ret;

	if (!key || !msg || !sig)
		return (NULL);
	pkey = EVP_PKEY_new();
	if (!pkey)
		return (NULL);
	if (EVP_PKEY_set1_EC_KEY(pkey, (struct ec_key_st *)key) != 1)
		return (NULL);
	ctx = EVP_MD_CTX_create();
	if (!ctx)
		return (NULL);
	if (EVP_DigestSignInit(ctx, NULL, EVP_sha256(), NULL, pkey) != 1)
	{
		EVP_MD_CTX_destroy(ctx);
		return (NULL);
	}
	if (EVP_DigestSignUpdate(ctx, msg, msglen) != 1)
	{
		EVP_MD_CTX_destroy(ctx);
		return (NULL);
	}
	sig->len = SIG_MAX_LEN;
	ret = EVP_DigestSignFinal(ctx, sig->sig, (size_t *)&(sig->len));
	if (ret != 1)
	{
		dprintf(2, "ERR [%s]\n", ERR_error_string(ERR_get_error(), NULL));
		EVP_MD_CTX_destroy(ctx);
		dprintf(2, "FAIL6[ %d]\n", ret);
		return (NULL);
	}

	EVP_PKEY_free(pkey);
	EVP_MD_CTX_destroy(ctx);
	return (sig->sig);
}
