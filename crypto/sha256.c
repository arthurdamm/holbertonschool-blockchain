#include "hblk_crypto.h"

/**
 * sha256 - gets SHA256 hash for given string
 * @s: the string to encode
 * @len: the length of given string
 * @digest: buffer for hash
 * Return: pointer to digest else NULL
 */
uint8_t *sha256(int8_t const *s, size_t len,
	uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!s || !digest)
		return (NULL);
	return (SHA256((const unsigned char *)s, len, digest));
}
