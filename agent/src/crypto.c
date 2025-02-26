#include "crypto.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <string.h>
#include <stdlib.h>

// Base64 encoding function
static char *base64_encode(const unsigned char *input, int length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, input, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    
    char *result = (char *)malloc(bufferPtr->length + 1);
    memcpy(result, bufferPtr->data, bufferPtr->length);
    result[bufferPtr->length] = 0;
    
    BIO_free_all(bio);
    
    return result;
}

// Base64 decoding function
static unsigned char *base64_decode(const char *input, int *length) {
    BIO *bio, *b64;
    
    int inputLength = strlen(input);
    unsigned char *buffer = (unsigned char *)malloc(inputLength);
    
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(input, inputLength);
    bio = BIO_push(b64, bio);
    
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    *length = BIO_read(bio, buffer, inputLength);
    
    BIO_free_all(bio);
    
    return buffer;
}
char *encrypt_message(const char *message, const unsigned char *key) {
    const char *secret_key = "THISISASECRETKEY";
    int key_len = strlen(secret_key);
    int message_len = strlen(message);
    
    // Encrypt with XOR
    unsigned char *encrypted = (unsigned char *)malloc(message_len);
    for (int i = 0; i < message_len; i++) {
        encrypted[i] = message[i] ^ secret_key[i % key_len];
    }
    
    // Base64 encode
    char *encoded = base64_encode(encrypted, message_len);
    
    free(encrypted);
    return encoded;
}

char *decrypt_message(const char *encrypted_message, const unsigned char *key) {
    const char *secret_key = "THISISASECRETKEY";
    int key_len = strlen(secret_key);
    
    // Base64 decode
    int decoded_len;
    unsigned char *decoded = base64_decode(encrypted_message, &decoded_len);
    
    // Decrypt with XOR
    for (int i = 0; i < decoded_len; i++) {
        decoded[i] = decoded[i] ^ secret_key[i % key_len];
    }
    
    // Null terminate
    decoded[decoded_len] = '\0';
    
    return (char *)decoded;
}
