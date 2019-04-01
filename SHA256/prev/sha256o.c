#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <time.h>

int main (void) {
  clock_t t;
  t = clock();
	int p_id;
	p_id=getpid();
	printf("Process ID: %d\n",p_id);
	getchar();
	//const
  char *s = "Rosetta_code";
  unsigned char *d = SHA256(s, strlen(s), 0);

	int i;
	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", d[i]);
	putchar('\n');

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  printf("program took %f seconds to execute \n", time_taken);
	return 0;
}
