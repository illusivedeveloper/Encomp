#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

//4 registers a,b,c,d of 32 bit register based on which calculation will be done

int valueA=0x67452301;
int valueB=(unsigned int)0xEFCDAB89;
int valueC=(unsigned int)0x98BADCFE;
int valueD=0x10325476;

int a;
int b;
int c;
int d;

int a1;
int b1;
int c1;
int d1;

int shft[] = {7,12,17,22,5,9,14,20,4,11,16,23,6,10,15,21};
int table[64];

int *m; //Message needs to encrypted (ASCII of each character of a message will be stored)
int *p; //Padding bytes
int *bfr; //Buffer bytes
int *md; //The MD5 output

int ms; //Message size
int ps; //Padding byte size
int hd=16; //Buffer size & MD5 output size is same as 16 taken as one variable
int numblock; //Number blocks
int mbit; //Message length bits

//Function Definitions

void initializ();
void init();
int byte(int);
int ul(int,int);
int ur(int,int);
int uor(int,int);
int rotateleft(int,int);
void compute1();
void compute2();
void compute3();
void process(char*);
char* input();

void initializ() //Initializing MD5 Table
{
	double p=pow(2,32),val;
	for (int i = 0; i < 64; i++)
	{
		val=fabs(sin(i+1));
		table[i]=(unsigned int)(val*p);
	}
}

void init() //Computing & Allocating Memory
{
	int i;
	numblock=ur((ms+8),6)+1; //number blocks calculated
	ps=(numblock<<6)-ms; //calculated padding byte size
	p=(int*)malloc(ps*sizeof(int)); //allocating memory to padding bytes
	bfr=(int*)malloc(hd*sizeof(int)); //allocating memory to buffer bytes
	md=(int*)malloc(hd*sizeof(int)); //allocating memory to md5 output
	p[0]=-128;
	mbit=ms<<3;
	for(i=1;i<ps;i++)
	{
		p[i]=0;
	}
	for(i=0;i<hd;i++)
	{
		bfr[i]=0;
	}
	a=valueA;
	b=valueB;
	c=valueC;
	d=valueD;
}

int byte(int val) // Range conversion of a integer to signed char from -128 to 127
{
	while(!(val>=-128&&val<128))
	{
		val-=256;
	}
	return val;
}

int ul(int a,int b) //Unsigned shift left in 32-bit mode
{
	uint32_t x,y;
	x=a;
	y=b;
	return (x<<y);
}

int ur(int a,int b) //Unsigned shift right in 32-bit mode
{
	uint32_t x,y;
	x=a;
	y=b;
	return (x>>y);
}

int uor(int a,int b) //OR operation in 32-bit mode
{
	uint32_t x,y;
	x=a;
	y=b;
	return (x|y);
}

int rotateleft(int i,int d) //2's complement binary representation of the specified int value i left by the specified d
{
	uint32_t x,y;
	x=i;
	y=d;
	return uor(ul(x,y),ur(x,32-y));
}

void compute1() //initializing padding bytes
{
	int i;
	for (i=0;i<8;i++)
	{
		p[ps-8+i]=byte(mbit);
		mbit=ur(mbit,8);
	}
}

void compute2() //initializing buffer bytes and computing the main auxilary function of md5
{
	int i,j,k,d16,f,t,val,temp;
	for(i=0;i<numblock;i++)
	{
		k=i<<6;
		for(j=0;j<64;j++,k++)
		{
			val=(k<ms)?m[k]:p[k-ms];
			bfr[ur(j,2)]=uor(ul(val,24),ur(bfr[ur(j,2)],8));
		}
		a1=a;
		b1=b;
		c1=c;
		d1=d;
		for(j=0;j<64;j++)
		{
			d16=ur(j,4);
			f=0;
			t=j;
			if(d16==0)
			{
				f=(b&c)|(~b&d);
			}
			else if(d16==1)
			{
				f=(b&d)|(c&~d);
				t=((t*5)+1)&(0x0F);
			}
			else if(d16==2)
			{
				f=b^c^d;
				t=((t*3)+5)&(0x0F);
			}
			else if(d16==3)
			{
				f=c^(b|~d);
				t=(t*7)&(0x0F);
			}
			temp=b+rotateleft(a+f+bfr[t]+table[j],shft[(d16<<2)|(j&3)]);
			a=d;
			d=c;
			c=b;
			b=temp;
		}
		a+=a1;
		b+=b1;
		c+=c1;
		d+=d1;
	}
}

void compute3() //initializing md5 output using above computation functions
{
	int kk;
	int i,j,k,n;
	char hexad[2];
	k=0;
	printf("MD5 : 0x");
	for(i=0;i<4;i++)
	{
		n=(i==0)?a:((i==1)?b:((i==2)?c:d));
		for(j=0;j<4;j++)
		{
			md[k]=byte(n);
			kk=(md[k]<0)?(md[k]+256):md[k];
			if(kk<=0x0F)
			{
				sprintf(hexad,"0%x",kk);
			}
			else
			{
				sprintf(hexad,"%x",kk);
			}
			//printf("check : md[%d]=%d\t->\t%s\n",k,md[k],hexad);
			printf("%s",hexad);
			k++;
			n=ur(n,8);
		}
	}
	printf("\n");
}

void process(char *data) //manipulating function calls of this program
{
	int i;
	ms=strlen(data); //message size
	m=(int*)malloc(ms*sizeof(int)); //allocating memory to message
	for(i=0;i<ms;i++)
	{
		m[i]=data[i];
	}
	initializ();
	init();
	compute1();
	compute2();
	compute3();
}

char* input() //for taking input from user
{
	char x,*y;
	char arr[1024];
	int i=0;
	while(x=getchar())
	{
		if(x=='\n'||x==EOF)
		{
			break;
		}
		arr[i++]=x;
	}
	y=(char*)malloc((i+1)*sizeof(char));
	i--;
	while(i>=0)
	{
		y[i]=arr[i];
		i--;
	}
	return y;
}

int main()
{
	clock_t t;
	t = clock();
	int p_id;
	p_id=getpid();
	printf("Process ID: %d\n",p_id);
	char *str;
	printf("Enter a string to be encrypted\n");
	str=input();
	t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  printf("Program took %f seconds to execute \n", time_taken);
	process(str);
	return 0;
}
