#ifndef CRYPTO_H
#define CRYPTO_H

char *encrypt_message(const char *message, const unsigned char *key);
char *decrypt_message(const char *encrypted_message, const unsigned char *key);

#endif
