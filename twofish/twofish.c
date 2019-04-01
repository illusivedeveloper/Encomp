#include <stdlib.h>
#include <string.h>
#include "twofish.h"


byte_t g_mds[4][4] = {
	{ 0x01, 0xef, 0x5b, 0x5b, },
	{ 0x5b, 0xef, 0xef, 0x01, },
	{ 0xef, 0x5b, 0x01, 0xef, },
	{ 0xef, 0x01, 0xef, 0x5b, },
};

byte_t g_rs[4][8] = {
	{ 0x01, 0xa4, 0x55, 0x87, 0x5a, 0x58, 0xdb, 0x9e, },
	{ 0xa4, 0x56, 0x82, 0xf3, 0x1e, 0xc6, 0x68, 0xe5, },
	{ 0x02, 0xa1, 0xfc, 0xc1, 0x47, 0xae, 0x3d, 0x19, },
	{ 0xa4, 0x55, 0x87, 0x5a, 0x58, 0xdb, 0x9e, 0x03, },
};

byte_t g_q[2][256];

void twofish_init()
{
	byte_t t[2][4][16] = {
		{
			{ 0x8, 0x1, 0x7, 0xd, 0x6, 0xf, 0x3, 0x2, 0x0, 0xb, 0x5, 0x9, 0xe, 0xc, 0xa, 0x4, },
			{ 0xe, 0xc, 0xb, 0x8, 0x1, 0x2, 0x3, 0x5, 0xf, 0x4, 0xa, 0x6, 0x7, 0x0, 0x9, 0xd, },
			{ 0xb, 0xa, 0x5, 0xe, 0x6, 0xd, 0x9, 0x0, 0xc, 0x8, 0xf, 0x3, 0x2, 0x4, 0x7, 0x1, },
			{ 0xd, 0x7, 0xf, 0x4, 0x1, 0x2, 0x6, 0xe, 0x9, 0xb, 0x3, 0x0, 0x8, 0x5, 0xc, 0xa, },
		},
		{
			{ 0x2, 0x8, 0xb, 0xd, 0xf, 0x7, 0x6, 0xe, 0x3, 0x1, 0x9, 0x4, 0x0, 0xa, 0xc, 0x5, },
			{ 0x1, 0xe, 0x2, 0xb, 0x4, 0xc, 0x3, 0x7, 0x6, 0xd, 0xa, 0x5, 0xf, 0x9, 0x0, 0x8, },
			{ 0x4, 0xc, 0x7, 0x5, 0x1, 0x6, 0x9, 0xa, 0x0, 0xe, 0xd, 0x8, 0x2, 0xb, 0x3, 0xf, },
			{ 0xb, 0x9, 0x5, 0x1, 0xc, 0x3, 0xd, 0xe, 0x6, 0x4, 0x7, 0xf, 0x2, 0x0, 0x8, 0xa, },
		},

	};
	unsigned int i;

	for(i = 0x00; i < 0x100; i++) {
		word_t a[5], b[5], x = (byte_t)i;

		a[0] = x / 16;
		b[0] = x % 16;
		a[1] = a[0] ^ b[0];
		b[1] = (a[0] ^ ROTR4(b[0], 1) ^ (8 * a[0])) % 16;
		
		/* q[0] */
		a[2] = t[0][0][a[1]];
		b[2] = t[0][1][b[1]];
		a[3] = a[2] ^ b[2];
		b[3] = (a[2] ^ ROTR4(b[2], 1) ^ (8 * a[2])) % 16;
		a[4] = t[0][2][a[3]];
		b[4] = t[0][3][b[3]];
		g_q[0][x] = 16 * b[4] + a[4];
		
		/* q[1] */
		a[2] = t[1][0][a[1]];
		b[2] = t[1][1][b[1]];
		a[3] = a[2] ^ b[2];
		b[3] = (a[2] ^ ROTR4(b[2], 1) ^ (8 * a[2])) % 16;
		a[4] = t[1][2][a[3]];
		b[4] = t[1][3][b[3]];
		g_q[1][x] = 16 * b[4] + a[4];
	}
}

byte_t gf2_8_mul(byte_t a, byte_t b, uint16_t reduce)
{
	uint16_t ac;
	byte_t prod;

	ac   = a;
	prod = 0;
	while(b) {
		if(b & 0x01)
			prod ^= ac;
		ac <<= 1;
		b  >>= 1;
		if(ac & 0x0100)
			ac ^= reduce;
	}

	return prod;
}

word_t h(word_t x, byte_t *l, size_t k)
{
	/* max k is expected to be 4 */
	unsigned int i, j;
	byte_t z[4], y[4];

	*(word_t *)&y = x;

	if(k == 4) {
		for(i = 0; i < 4; i++)
			y[i] = g_q[(!i || i == 3 ? 1 : 0)][y[i]] ^ l[3 * 4 + i];
	}

	if(k >= 3) {
		for(i = 0; i < 4; i++)
			y[i] = g_q[!i || i == 1 ? 1 : 0][y[i]] ^ l[2 * 4 + i];
	}
	
	for(i = 0; i < 4; i++)
		y[i] = g_q[i % 2 ? 0 : 1][g_q[i < 2 ? 0 : 1][g_q[i % 2 ? 1 : 0][y[i]] ^ l[4 + i]] ^ l[i]];
	
	
	/* matrix product, MDS x y
	 * reduction polynomial is x^8 + x^6 + x^5 + x^3 + 1
	 * which is 0b0000000101101001 = 0x0169 */
	memset(z, 0, sizeof(z));
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++)
			z[i] ^= gf2_8_mul(g_mds[i][j], y[j], 0x0169);
	}

	return *(word_t *)&z;
}

twofish_t *twofish_ctx_new(byte_t *key, size_t keylen)
{
	unsigned int i, k;
	twofish_t *ctx;
	word_t m_even[4], m_odd[4], *m = (word_t *)key;

	switch(keylen) {
		case KEY_128:
		case KEY_192:
		case KEY_256:
			/* in bits: (keylen * 8) / 64 */
			k = keylen / 8; 
			break;
		default:
			return NULL;
	}

	ctx = malloc(sizeof(twofish_t));
	if(!ctx) return NULL;
	
	ctx->keylen = keylen;
	for(i = 0; i < k; i++) {
		unsigned int j, n;

		m_even[i] = m[2 * i];
		m_odd[i]  = m[2 * i + 1];

		/* matrix product RS x { m_8i, m_8i+1, ... , m_8i+7, }
	   	 * reduction polynomial is x^8 + x^6 + x^3 + x^2 + 1
		 * which is 0b0000000101001101 = 0x014d */
		for(j = *(word_t *)&ctx->sbox[i] = 0; j < 4; j++) {
			for(n = 0; n < 8; n++)
				ctx->sbox[i][j] ^= gf2_8_mul(g_rs[j][n], key[8 * i + n], 0x014d);
		}
	}

 	for(i = 0; i < 20; i++) {
		word_t a, b;

		a = h(2 * i * RHO, (byte_t *)m_even, k);
		b = ROTL(h((2 * i + 1) * RHO, (byte_t *)m_odd, k), 8);
		ctx->ksched[2 * i]     = a + b;
		ctx->ksched[2 * i + 1] = ROTL(a + 2 * b, 9);
	}

	return ctx;
}

void F(twofish_t *ctx, word_t r[2], word_t f[2], unsigned int round)
{
	word_t t[2];

	t[0] = g(ctx, r[0]);
	t[1] = g(ctx, ROTL(r[1], 8));
	/* PHT */
	f[0] = t[0] + t[1] + ctx->ksched[2 * round + 8];
	f[1] = t[0] + 2 * t[1] + ctx->ksched[2 * round + 9];
}

void twofish_encrypt(twofish_t *ctx, byte_t input[16], byte_t output[16])
{
	unsigned int i;
	word_t tmp, *oper = (word_t *)output;

	memcpy(output, input, 16);

	/* input whitening */
	for(i = 0; i < 4; i++)
		oper[i] ^= ctx->ksched[i];
	
	/* 16 rounds */
	for(i = 0; i < 16; i++) {
		word_t f[2];

		F(ctx, oper, f, i);
		oper[2] = ROTR(oper[2] ^ f[0], 1);
		oper[3] = ROTL(oper[3], 1) ^ f[1];

		/* swap oper[0] <-> oper[2], oper[1] <-> oper[3] */
		tmp = oper[0], oper[0] = oper[2], oper[2] = tmp;
		tmp = oper[1], oper[1] = oper[3], oper[3] = tmp;
	}

	/* undo last swap */
	tmp = oper[0], oper[0] = oper[2], oper[2] = tmp;
	tmp = oper[1], oper[1] = oper[3], oper[3] = tmp;

	/* output whitening */
	for(i = 0; i < 4; i++)
		oper[i] ^= ctx->ksched[i + 4];
}

void twofish_decrypt(twofish_t *ctx, byte_t input[16], byte_t output[16])
{
	int i;
	word_t tmp, *oper = (word_t *)output;
	
	memcpy(output, input, 16);
	
	/* undo output whitening */
	for(i = 0; i < 4; i++)
		oper[i] ^= ctx->ksched[i + 4];
	
	/* redo last swap */
	tmp = oper[0], oper[0] = oper[2], oper[2] = tmp;
	tmp = oper[1], oper[1] = oper[3], oper[3] = tmp;

	/* reverse 16 rounds */
	for(i = 15; i >= 0; i--) {
		word_t f[2];

		/* undo last swap */
		tmp = oper[0], oper[0] = oper[2], oper[2] = tmp;
		tmp = oper[1], oper[1] = oper[3], oper[3] = tmp;

		F(ctx, oper, f, i);
		oper[2] = ROTL(oper[2], 1) ^ f[0];
		oper[3] = ROTR(oper[3] ^ f[1], 1);
	}
	
	/* undo input whitening */
	for(i = 0; i < 4; i++)
		oper[i] ^= ctx->ksched[i];
}

void twofish_ctx_destroy(twofish_t *ctx)
{
	free(ctx);
}
