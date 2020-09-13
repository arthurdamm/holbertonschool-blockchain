CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3 -c

SRC0 = sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c
OBJ0 = $(SRC0:.c=.o)
NAME0 = libhblk_crypto.a

.PHONY: clean oclean flcean libhblk_crypto.a

clean:
	$(RM) *~ $(NAME0)

oclean:
	$(RM) $(OBJ0)

fclean: clean oclean

libhblk_crypto.a: hblk_crypto.h $(OBJ0)
	ar rcs $(NAME0) $(OBJ0)
