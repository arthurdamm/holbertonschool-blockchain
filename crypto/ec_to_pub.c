#include "hblk_crypto.h"

/**
 * ec_to_pub - extracts public key from EC_KEY struct
 * @key: pointer to EC_KEY struct
 * @pub: address of buffer to populate with pub key
 * Return: address of populated buffer or NULL
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	EC_GROUP const *group = NULL;
	EC_POINT const *point = NULL;

	if (!key || !pub)
		return (NULL);
	point = EC_KEY_get0_public_key(key);
	if (!point)
		return (NULL);
	group = EC_KEY_get0_group(key);
	if (!group)
		return (NULL);
	if (!EC_POINT_point2oct(
		group,
		point,
		POINT_CONVERSION_UNCOMPRESSED,
		pub, EC_PUB_LEN, NULL))
		return (NULL);
	return (pub);
}
