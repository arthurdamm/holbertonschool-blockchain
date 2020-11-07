#include "cli.h"

/**
 * cmd_exit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int cmd_exit(info_t *info)
{
	info->err_num = -1;
	return (-2);
}

/**
 * cmd_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int cmd_help(info_t *info)
{
	printf("help call works. Function not yet implemented \n");
	return (0);
	(void)info;
}

/**
 * cmd_wallet_load - loads wallet from file
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int cmd_wallet_load(info_t *info)
{
	EC_KEY *key;

	if (info->argc != 2)
		return (printf(WALLET_SAVE_USAGE), 0);
	key = ec_load(info->argv[1]);
	if (!key)
		return (printf("Wallet failed to load.\n"), 0);
	EC_KEY_free(info->blockchain_data->key);
	info->blockchain_data->key = key;
	printf("Wallet loaded successfully.\n");
	return (0);
}

/**
 * cmd_wallet_save - saves wallet to file
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int cmd_wallet_save(info_t *info)
{
	if (info->argc != 2)
		return (printf(WALLET_SAVE_USAGE), 0);
	if (ec_save(info->blockchain_data->key, info->argv[1]))
		printf("Wallet saved successfully.\n");
	else
		printf("Wallet failed to save.\n");
	return (0);
}

/**
 * cmd_wallet_show - prints public key associated with current wallet
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int cmd_wallet_show(info_t *info)
{
	uint8_t pub[EC_PUB_LEN + 1] = {0};

	ec_to_pub(info->blockchain_data->key, pub);
	print_hex_buffer(pub, EC_PUB_LEN);
	printf("\n");
	return (0);
}
