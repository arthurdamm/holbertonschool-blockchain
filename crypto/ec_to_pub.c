#include "hblk_crypto.h"

/**
 * ec_to_pub - extracts public key from EC_KEY struct
 * @key: pointer to EC_KEY struct
 * @pub: address of buffer to populate with pub key
 * Return: address of populated buffer or NULL
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	EC_GROUP const *group;
	EC_POINT const *point;
	size_t len;

	if (!key)
		return (NULL);
	group = EC_KEY_get0_group(key);
	point = EC_KEY_get0_public_key(key);
	if (!group || !point)
		return (NULL);
	len = EC_POINT_point2oct(
		group,
		point,
		POINT_CONVERSION_UNCOMPRESSED,
		pub, EC_PUB_LEN, NULL);
	if (len != EC_PUB_LEN)
		return (NULL);
	return (pub);
}
