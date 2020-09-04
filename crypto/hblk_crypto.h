#ifndef HBLK_CRYPTO
#define HBLK_CRYPTO

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/sha.h>

void _print_hex_buffer(uint8_t const *buf, size_t len);

uint8_t *sha256(int8_t const *s, size_t len,
	uint8_t digest[SHA256_DIGEST_LENGTH]);

#endif
