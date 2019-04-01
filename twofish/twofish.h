#pragma once

#include <stdint.h>

#define ROTL(x, r)  (((x) << (r)) | ((x) >> (8 * sizeof(word_t) - (r))))
#define ROTR(x, r)  (((x) >> (r)) | ((x) << (8 * sizeof(word_t) - (r))))
#define ROTR4(x, r) (((x) >> (r)) | (((x) & (1 << ((r)-1))) << (4 - (r))))

#define KEY_128 (128/8)
#define KEY_192 (192/8)
#define KEY_256 (256/8)

#define RHO 0x01010101

#define g(ctx, x) h((x), (byte_t *)ctx->sbox, ctx->keylen / 8)

typedef uint8_t byte_t;
typedef uint32_t word_t;

struct _twofish {
	size_t keylen;
	byte_t sbox[4][4];
	word_t ksched[40];
};
typedef struct _twofish twofish_t;

void twofish_init();
twofish_t *twofish_ctx_new(byte_t *key, size_t keylen);
void twofish_encrypt(twofish_t *ctx, byte_t input[16], byte_t output[16]);
void twofish_decrypt(twofish_t *ctx, byte_t input[16], byte_t output[16]);
void twofish_ctx_destroy(twofish_t *ctx);