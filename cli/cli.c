#include "cli.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	info_t info[] = { INFO_INIT };
	blockchain_data_t blockchain_data[] = { BLOCKCHAIN_DATA_INIT };

	blockchain_data->key = ec_create();
	blockchain_data->blockchain = blockchain_create();
	blockchain_data->tx_pool = llist_create(MT_SUPPORT_TRUE);
	if (!blockchain_data->key || !blockchain_data->blockchain ||
		!blockchain_data->tx_pool) {
		printf("Error initializing blockchain data.\n");
		exit(1);
	}

	info->blockchain_data = blockchain_data;
	cli(info, av);
	return (EXIT_SUCCESS);
	(void)ac;
}

/**
 * cli - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int cli(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	setbuf(stdout, NULL);
	while (r != -1 && builtin_ret != -2)
	{
		clear_info(info);
		if (interactive(info))
			printf("$ ");
		r = get_input(info);
		if (r != -1)
		{
			set_info(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
				printf("No such command. Try 'help'.\n");
		}
		else if (interactive(info))
			putchar('\n');
		free_info(info, 0);
	}
	free_info(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_t builtins[] = CLI_COMMANDS;

	for (i = 0; builtins[i].name; i++)
		if (strcmp(info->argv[0], builtins[i].name) == 0)
		{
			info->line_count++;
			built_in_ret = builtins[i].func(info);
			break;
		}
	return (built_in_ret);
}
