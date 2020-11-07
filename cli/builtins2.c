#include "cli.h"

/**
 * cmd_send - sends coins from one wallet to another
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: always 0
 */
int cmd_send(info_t *info)
{
	EC_KEY *receiver;
	transaction_t *tx = NULL;
	uint8_t pub[EC_PUB_LEN] = {0};
	uint32_t amount, i;

	if (info->argc != 3)
		return (printf(SEND_USAGE), 0);
	amount = atoi(info->argv[1]);
	for (i = 0; i < EC_PUB_LEN; i++)
		if (sscanf(&info->argv[2][i * 2], "%2x", (unsigned int *)&pub[i]) < 1)
			break;
	receiver = ec_from_pub(pub);
	if (!receiver)
		return (printf("Invalid receiver public key.\n"), 0);
	tx = transaction_create(info->blockchain_data->key, receiver, amount,
		info->blockchain_data->blockchain->unspent);
	if (!tx)
		printf("Failed to create transaction.\n");
	else
	{
		if (transaction_is_valid(tx,
			info->blockchain_data->blockchain->unspent))
		{
			if (llist_add_node(info->blockchain_data->tx_pool, tx,
				ADD_NODE_REAR) != -1)
				printf("Transaction successfully added to local pool.\n");
			else
			{
				printf("Error adding transaction to local pool.\n");
				transaction_destroy(tx);
			}
		}
		else
		{
			printf("Transaction not validated.\n");
			transaction_destroy(tx);
		}
	}
	EC_KEY_free(receiver);
	return (0);
}

/**
 * cmd_info - displays info about blockchain
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: always 0
 */
int cmd_info(info_t *info)
{
	if (isarg(info, "-u"))
	{
		print_all_unspent(info->blockchain_data->blockchain->unspent);
		return (0);
	}
	if (isarg(info, "-p"))
	{
		printf("----------- TX Pool:\n");
		llist_for_each(info->blockchain_data->tx_pool,
		(node_func_t)_transaction_print_loop, "\t\t");
		printf("----------- END TX Pool:\n");
		return (0);
	}
	if (isarg(info, "-b"))
		_blockchain_print_brief(info->blockchain_data->blockchain);
	else
		_blockchain_print(info->blockchain_data->blockchain);
	printf("%s\n", "------------------------------------------------------");
	printf("Blocks: %d\tUTXOs: %d\tTX Pool: %d\n",
		llist_size(info->blockchain_data->blockchain->chain),
		llist_size(info->blockchain_data->blockchain->unspent),
		llist_size(info->blockchain_data->tx_pool));
	return (0);
}

/**
 * cmd_load - loads a blockchain from file
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: always 0
 */
int cmd_load(info_t *info)
{
	blockchain_t *blockchain = NULL;

	if (info->argc != 2)
		return (printf(LOAD_USAGE), 0);
	blockchain = blockchain_deserialize(info->argv[1]);
	if (!blockchain)
		return (printf("Failed to load blockchain.\n"), 0);
	blockchain_destroy(info->blockchain_data->blockchain);
	info->blockchain_data->blockchain = blockchain;
	printf("Blockchain loaded successfully.\n");
	return (0);
}

/**
 * cmd_save - saves a blockchain to file
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: always 0
 */
int cmd_save(info_t *info)
{
	if (info->argc != 2)
		return (printf(SAVE_USAGE), 0);
	if (!blockchain_serialize(info->blockchain_data->blockchain, info->argv[1]))
		printf("Blockchain saved successfully.\n");
	else
		printf("Failed to save blockchain.\n");
	return (0);
}
