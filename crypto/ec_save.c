#include "hblk_crypto.h"

/**
 * ec_save - saves public/private keys in PEM format
 * @key: pointer to EC_KEY struct with key pair
 * @folder: the folder to save, created if need be
 * Return: 1 on success else 0
 */
int ec_save(EC_KEY *key, char const *folder)
{
	char path[128] = {0};
	BIO *out;

	if (!key || !folder)
		return (0);
	mkdir(folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	sprintf(path, "%s/key.pem", folder);
	out = BIO_new(BIO_s_file());
	if (!out)
		return (0);
	BIO_set_close(out, BIO_CLOSE);
	if (!BIO_write_filename(out, path))
		return (BIO_free_all(out), 0);
	if (!PEM_write_bio_ECPrivateKey(out, key, NULL, NULL, 0, NULL, NULL))
	{
		BIO_free_all(out);
		return (0);
	}
	BIO_free_all(out);

	sprintf(path, "%s/key_pub.pem", folder);
	out = BIO_new(BIO_s_file());

	if (!out)
		return (0);
	BIO_set_close(out, BIO_CLOSE);
	if (!BIO_write_filename(out, path))
		return (BIO_free_all(out), 0);
	
	if (!PEM_write_bio_EC_PUBKEY(out, key))
	{
		BIO_free_all(out);
		return (0);
	}
	BIO_free_all(out);

	return (1);
}
