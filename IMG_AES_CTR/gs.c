#include <stdlib.h>
#include <time.h>

int main()
{
  clock_t t;
  t = clock();
	int p_id;
	p_id=getpid();
	printf("Process ID: %d\n",p_id);
	getchar();
 system("~/S/IMG_AES_CTR/gs.sh");
 t = clock() - t;
 double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 printf("program took %f seconds to execute \n", time_taken);
 return 0;
}
