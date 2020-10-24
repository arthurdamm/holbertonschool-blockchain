#include "hblk_crypto.h"

/**
 * ec_save - saves public/private keys in PEM format
 * @key: pointer to EC_KEY struct with key pair
 * @folder: the folder to save, created if need be
 * Return: 1 on success else 0
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *fp;
	char path[128] = {0};

	if (!key || !folder)
		return (0);
	mkdir(folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	sprintf(path, "%s/" PRI_FILENAME, folder);
	fp = fopen(path, "w");
	if (!fp)
		return (0);
	if (!PEM_write_ECPrivateKey(fp, key, NULL, NULL, 0, NULL, NULL))
	{
		/*fclose(fp);*/
		/*return (0);*/
	}
	fclose(fp);

	sprintf(path, "%s/" PUB_FILENAME, folder);
	fp = fopen(path, "w");
	if (!fp)
		return (0);
	if (!PEM_write_EC_PUBKEY(fp, key))
	{
		fclose(fp);
		return (0);
	}
	fclose(fp);

	return (1);
}
