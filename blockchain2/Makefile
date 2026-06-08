CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pedantic -I. -I../crypto
LDFLAGS = -L../crypto -lhblk_crypto -lssl -lcrypto

SRC     = main.c block.c blockchain.c pending_pool.c utxo.c account.c mining.c
TARGET  = blockchain2

.PHONY: all clean crypto

all: crypto $(TARGET)

# Ensure the crypto library is built first
crypto:
	$(MAKE) -C ../crypto

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	$(RM) $(TARGET)
