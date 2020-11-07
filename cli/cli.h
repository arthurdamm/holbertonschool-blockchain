#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#include <hblk_crypto.h>
#include <blockchain.h>
#include <transaction.h>
#include <provided.h>
#include <llist.h>

#define BLOCK_DATA "New Holberton on the Block"
#define WALLET_SAVE_USAGE "wallet_save <path>\n"
#define WALLET_LOAD_USAGE "wallet_load <path>\n"
#define SEND_USAGE "send <amount> <address>\n"
#define LOAD_USAGE "load <path>\n"
#define SAVE_USAGE "save <path>\n"

#define EXIT_HELP "Exits the CLI shell.\n"
#define HELP_HELP "Displays info on CLI commands.\n"
#define WALLET_SAVE_HELP WALLET_SAVE_USAGE "Saves wallet EC_KEY pair to <path>.\n"
#define WALLET_LOAD_HELP WALLET_LOAD_USAGE "Loads wallet EC_KEY pair from <path>.\n"
#define WALLET_SHOW_HELP "Displays info about current wallet EC_KEY pair.\n"
#define SEND_HELP SEND_USAGE "Sends currency to wallet.\n"
#define LOAD_HELP LOAD_USAGE "Deserializes blockchain from <path>.\n"
#define SAVE_HELP SAVE_USAGE "Serializes blockchain to <path>.\n"
#define MINE_HELP "Mines a new block.\n"
#define INFO_HELP "Displays info about blockchain. Can use -b flag for brief.\n"

#define READ_BUF_SIZE 1024
/* 1 if using system getline() */
#define USE_GETLINE 0

#define CLI_COMMANDS { \
		{"exit", cmd_exit, EXIT_HELP}, \
		{"help", cmd_help, HELP_HELP}, \
		{"wallet_load", cmd_wallet_load, WALLET_LOAD_HELP}, \
		{"wallet_save", cmd_wallet_save, WALLET_SAVE_HELP}, \
		{"wallet_show", cmd_wallet_show, WALLET_SHOW_HELP}, \
		{"send", cmd_send, SEND_HELP}, \
		{"mine", cmd_mine, MINE_HELP}, \
		{"info", cmd_info, INFO_HELP}, \
		{"save", cmd_save, SAVE_HELP}, \
		{"load", cmd_load, LOAD_HELP}, \
		{NULL, NULL, NULL} \
	}

extern char **environ;

/**
 * struct Blockchain_data - stores info about current blockchain
 * @key: EC_KEY pair, currently loaded wallet
 * @blockchain: the currently loaded blockchain
 * @tx_pool: local transaction pool
 */
typedef struct blockchain_data
{
	EC_KEY *key;
	blockchain_t *blockchain;
	llist_t *tx_pool;
} blockchain_data_t;

#define BLOCKCHAIN_DATA_INIT \
{NULL, NULL, NULL}

/**
 *struct Info - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@status: the return status of the last exec'd command
 *@readfd: the fd from which to read line input
 *@blockchain_data: stored data about current blockchain
 */
typedef struct Info
{
	char *arg;
	char **argv;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;

	int status;
	int readfd;

	blockchain_data_t *blockchain_data;
} info_t;

#define INFO_INIT \
{NULL, NULL, 0, 0, 0, 0, NULL, 0, 0, NULL}

/**
 *struct builtin - contains a builtin string and related function
 *@name: the builtin command name
 *@func: the command function
 *@help: help string
 */
typedef struct builtin
{
	char *name;
	int (*func)(info_t *);
	char *help;
} builtin_t;

/* builtins1.c */
int cmd_exit(info_t *);
int cmd_help(info_t *);
int cmd_wallet_load(info_t *);
int cmd_wallet_save(info_t *info);
int cmd_wallet_show(info_t *info);

/* builtins2.c */
int cmd_send(info_t *info);
int cmd_mine(info_t *info);
int cmd_info(info_t *info);
int cmd_save(info_t *info);
int cmd_load(info_t *info);

/* cli.c */
int cli(info_t *, char **);
int find_builtin(info_t *);

/* util1.c */
char **strtow(char *, char *);
void ffree(char **);
int bfree(void **);
int interactive(info_t *);
int is_delim(char, char *);

/* util2.c */
void *_realloc(void *, unsigned int, unsigned int);
void print_hex_buffer(uint8_t const *buf, size_t len);
int isarg(info_t *info, char *arg);

/* getline.c module */
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);


/* info.c module */
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

#endif
