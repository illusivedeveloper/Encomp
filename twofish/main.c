#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "twofish.h"
#include "twofish.c"
#include <unistd.h>
//#include <conio.h>
//#include <curses.h>
#include <time.h>

void pthx_byte(byte_t *b, size_t len)
{
	while(len--)
		printf("%02x", *b++);
}

size_t parse_key(char *arg, unsigned char key[KEY_256])
{
	size_t keylen = 0;

	while(keylen < KEY_256 && (!keylen || *(arg - 1))
		&& sscanf(arg, "%02hhx", &key[keylen]) == 1)
		arg += 2, keylen++;
	memset(&key[keylen], 0, KEY_256 - keylen);
	if(keylen <= KEY_128)
		keylen = KEY_128;
	else if(keylen <= KEY_192)
		keylen = KEY_192;
	else keylen = KEY_256;

	return keylen;
}

/* initialization vector, needed for CBC mode
 * feel free to pick your own */
const byte_t g_iv[16] = {
	0x01, 0x02, 0x03, 0xf4,
	0x05, 0x06, 0x07, 0x08,
	0x00, 0x02, 0xf9, 0x0f,
	0x21, 0x23, 0x03, 0x0a,
};

int main(int argc, char *argv[])
{
	clock_t t;
  t = clock();
	int p_id;
	p_id=getpid();
	printf("Process ID: %d\n",p_id);
	getchar();
	byte_t key[KEY_256], input[16], output[16], *outp_copy;
	size_t keylen, inplen, siz;
	unsigned int i, j;
	int first;
	twofish_t *ctx;

	if(argc != 3) {
		fprintf(stderr, "Usage: %s [hex_key] [input]\n"
			"Example: %s 11223344556677889900aabbccddeeff test\n"
			"Uncomplete input or key is padded with zeros to the next largest length\n"
			"Too large keys are truncated to 32 bytes (256 bits)\n",
			argv[0] ? argv[0] : "twofish", argv[0] ? argv[0] : "twofish");
		return EXIT_FAILURE;
	}

	keylen = parse_key(argv[1], key);
	inplen = strlen(argv[2]);

	memset(key + keylen, 0, KEY_256 - keylen);
	twofish_init();
	puts("Twofish initialized");
	ctx = twofish_ctx_new(key, keylen);
	if(!ctx) {
		fprintf(stderr, "Not enough memory\n");
		return EXIT_FAILURE;
	}
	puts("Key schedule performed");

	first     = 1;
	siz       = 0;
	outp_copy = NULL;
	puts("Encrypting...");
	while(siz < inplen) {
		size_t l = inplen - siz > 16 ? 16 : inplen - siz;

		memcpy(input, argv[2] + siz, l);
		memset(input + l, 0, 16 - l);

		/* CBC - if you want ECB comment the next 2 lines*/
		for(i = 0; i < 16; i++)
			input[i] ^= first ? g_iv[i] : output[i];
		if(first) first = 0;

		twofish_encrypt(ctx, input, output);

		siz += 16;
		outp_copy = realloc(outp_copy, siz);
		if(!outp_copy) {
			twofish_ctx_destroy(ctx);
			fprintf(stderr, "Not enough memory\n");
			return EXIT_FAILURE;
		}
		memcpy(outp_copy + siz - 16, output, 16);
		pthx_byte(output, 16);
	}
	puts("\nEncryption completed");

	first = 1;
	puts("Decrypting...");
	for(i = 0; i < siz; i += 16) {
		twofish_decrypt(ctx, outp_copy + i, output);

		/* CBC - if you want ECB comment the next 2 lines*/
		for(j = 0; j < 16; j++)
			output[i + j] ^= first ? g_iv[j] : outp_copy[i];
		if(first) first = 0;
		for(j = i; j < 16 && inplen; j++, inplen--)
			putchar(output[i + j]);
	}
	puts("\nDecryption completed");

	free(outp_copy);
	twofish_ctx_destroy(ctx);
	//getchar();
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	printf("program took %f seconds to execute \n", time_taken);
	return EXIT_SUCCESS;
}
