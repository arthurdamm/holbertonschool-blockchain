#include "cli.h"

/**
 * clear_info - initializes info_t struct
 * @info: struct address
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void set_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

	}
}

/**
 * free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_info(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	if (all)
	{
		EC_KEY_free(info->blockchain_data->key);
		blockchain_destroy(info->blockchain_data->blockchain);
		llist_destroy(info->blockchain_data->tx_pool, 1,
			(node_dtor_t)transaction_destroy);
		free(info->arg);
		if (info->readfd > 2)
			close(info->readfd);
	}
}
