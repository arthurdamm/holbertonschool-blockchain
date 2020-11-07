#include "cli.h"

#define UPDATE_UTXOS { \
	info->blockchain_data->blockchain->unspent = update_unspent( \
		info->blockchain_data->tx_pool, \
		block->hash, \
		info->blockchain_data->blockchain->unspent); \
	llist_destroy(info->blockchain_data->tx_pool, 1, \
			(node_dtor_t)transaction_destroy); \
	info->blockchain_data->tx_pool = llist_create(MT_SUPPORT_TRUE); \
	utx = unspent_tx_out_create(block->hash, coin_tx->id, \
		llist_get_head(coin_tx->outputs)); }
/**
 * delete_invalid_tx - finds unspent matching txo
 * @node: current node, unspent_txo_t
 * @unspent: unspents
 * Return: 0 if continue else 1
 */
int delete_invalid_tx(llist_node_t node, void *unspent)
{
	transaction_t *tx = node;

	printf("delete_identifier:\n");
	_print_hex_buffer(tx->id, SHA256_DIGEST_LENGTH);
	printf("\n");
	return (!transaction_is_valid(tx, unspent));
}

/**
 * add_tx - adds tx to block
 * @node: current node, tx
 * @idx: index of node
 * @_block: arguments
 * Return: 0 if continue else 1
 */
int add_tx(llist_node_t node, unsigned int idx, void *_block)
{
	transaction_t *tx = node;
	block_t *block = _block;


	if (llist_add_node(block->transactions, tx, ADD_NODE_REAR))
	{
		dprintf(2, "add_tx: error llist_add_node!\n");
	}
	return (0);
	(void)idx;
}

/**
 * add_transaction_pool - adds tx_pool to block transactions
 * @info: info struct
 * @block: block to add
 */
void add_transaction_pool(info_t *info, block_t *block)
{
	printf("TXO Pool: %d\n", llist_size(info->blockchain_data->tx_pool));
	while (!llist_remove_node(info->blockchain_data->tx_pool,
		delete_invalid_tx, info->blockchain_data->blockchain->unspent,
		1, (node_dtor_t)transaction_destroy))
		dprintf(2, "add_transaction_pool: invalid tx destroyed.\n");
	llist_for_each(info->blockchain_data->tx_pool, add_tx, block);
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
	unspent_tx_out_t *utx;

	memcpy(data, BLOCK_DATA, strlen(BLOCK_DATA));
	block = block_create(prev_block, (int8_t *)data, sizeof(data));
	if (!block)
		return (printf("Failed to create block.\n"), 0);
	add_transaction_pool(info, block);
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
	block_mine(block), printf("MINED BLOCK:\n"), _block_print(block, 0, "\t\t");
	if (block_is_valid(block, prev_block,
		info->blockchain_data->blockchain->unspent))
	{
		printf("Failed to mine valid block.\n");
		block_destroy(block);
		return (0);
	}
	UPDATE_UTXOS;
	if (!utx || llist_add_node(info->blockchain_data->blockchain->unspent,
		utx, ADD_NODE_REAR))
		return (block_destroy(block), printf("Failed to created UTXO.\n"), 0);
	llist_add_node(info->blockchain_data->blockchain->chain,
		block, ADD_NODE_REAR);
	printf("Block mined successfully!\n");
	return (0);
}
