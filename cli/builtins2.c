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
		sscanf(&info->argv[2][i * 2], "%2x", (unsigned int *)&pub[i]);
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
 * cmd_mine - mines a new block
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: always 0
 */
int cmd_mine(info_t *info)
{
	int8_t data[64] = {0};
	block_t *prev_block =
		llist_get_tail(info->blockchain_data->blockchain->chain);
	block_t *block;
	transaction_t *coin_tx;

	memcpy(data, BLOCK_DATA, strlen(BLOCK_DATA));
	block = block_create(prev_block,(int8_t *)data, sizeof(data));
	if (!block)
		return (printf("Failed to create block.\n"), 0);
	block->info.difficulty =
		blockchain_difficulty(info->blockchain_data->blockchain);
	printf("DIFFICULTY: [%d]\n", block->info.difficulty);
	coin_tx = coinbase_create(info->blockchain_data->key, block->info.index);
	if (!coin_tx || !coinbase_is_valid(coin_tx, block->info.index))
	{
		printf("Failed to create valid coinbase transaction.\n");
		transaction_destroy(coin_tx);
		block_destroy(block);
		return (0);
	}
	llist_add_node(block->transactions, coin_tx, ADD_NODE_FRONT);
	block_mine(block);
	if (block_is_valid(block, prev_block,
		info->blockchain_data->blockchain->unspent))
	{
		printf("Failed to mine valid block.\n");
		block_destroy(block);
		return (0);
	}
	llist_add_node(info->blockchain_data->blockchain->chain, block, ADD_NODE_REAR);
	printf("Block mined successfully!\n");
	return (0);
	(void)info;
}

/**
 * cmd_info - displays info about blockchain
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: always 0
 */
int cmd_info(info_t *info)
{
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
		printf("Failed to load blockchain.\n");
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
	if(!blockchain_serialize(info->blockchain_data->blockchain, info->argv[1]))
		printf("Blockchain saved successfully.\n");
	else
		printf("Failed to save blockchain.\n");
	return (0);
}
