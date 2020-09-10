#include "hblk_crypto.h"

/**
 * ec_verify - verifies a message with public key
 * @key: pointer to struct containing key pair
 * @msg: the message to sign
 * @msglen: length of message
 * @sig: address to store signature
 * Return: 1 if verified else 0
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
	sig_t const *sig)
{
	EVP_MD_CTX *ctx = NULL;
	EVP_PKEY *pkey = NULL;
	int ret;

	if (!key || !msg || !sig)
		return (0);
	pkey = EVP_PKEY_new();
	if (!pkey)
		return (0);
	if (EVP_PKEY_set1_EC_KEY(pkey, (struct ec_key_st *)key) != 1)
		return (EVP_PKEY_free(pkey), 0);
	ctx = EVP_MD_CTX_create();
	if (!ctx)
		return (EVP_PKEY_free(pkey), 0);
	if (EVP_DigestVerifyInit(ctx, 0, EVP_sha256(), 0, pkey) != 1)
	{
		EVP_PKEY_free(pkey), EVP_MD_CTX_destroy(ctx);
		return (0);
	}
	if (EVP_DigestVerifyUpdate(ctx, msg, msglen) != 1)
	{
		EVP_PKEY_free(pkey), EVP_MD_CTX_destroy(ctx);
		return (0);
	}
	ret = EVP_DigestVerifyFinal(ctx, (unsigned char *)sig->sig, sig->len);
	if (ret != 1)
	{
		/* dprintf(2, "ERR [%s]\n", ERR_error_string(ERR_get_error(), 0)); */
		EVP_PKEY_free(pkey), EVP_MD_CTX_destroy(ctx);
		return (0);
	}

	EVP_PKEY_free(pkey);
	EVP_MD_CTX_destroy(ctx);
	return (1);
}
