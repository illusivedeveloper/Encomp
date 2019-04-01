#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <time.h>

int padding = RSA_PKCS1_PADDING;

RSA * createRSA(unsigned char * key,int public)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        printf( "Failed to create key BIO");
        return 0;
    }
    if(public)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        printf( "Failed to create RSA");
    }

    return rsa;
}

int public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,1);
    int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}
int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,0);
    int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
    return result;
}


int private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,0);
    int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}
int public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,1);
    int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,padding);
    return result;
}

void printLastError(char *msg)
{
    char * err = malloc(130);;
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), err);
    printf("%s ERROR: %s\n",msg, err);
    free(err);
}

int main(){
  clock_t t;
  t = clock();
	int p_id;
	p_id=getpid();
	printf("Process ID: %d\n",p_id);
	getchar();

  char plainText[2048/8] = "Hello this is Nitish"; //key length : 2048

 char publicKey[]="-----BEGIN PUBLIC KEY-----\n"\
 "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvGuWqEpy12bYg7k2lMl9\n"\
 "icFHoFaBbIRtpN8paouA1H3XQk0NR/a/QdDO9hez0MyfFDcfwDFy0gTf3MrcS3y8\n"\
 "ekhfNogJfcCEwWFib0cUSDfSl0zzmB0pCjTplCPNcQJYhfKwyITp3ttJ0HK4v5P2\n"\
 "kpznUWdRI3AAyQFVGNcR6HTi7aYZzicD0/vw00DVl9c9K1me6O77RF4eslQB+jLQ\n"\
 "qoLqhXSWBHN4/2JcD9n2S4r1ltooKWTozAVjMIbzQ1Wg+EXQr7uZmiuh8GfNPSSQ\n"\
 "YppbXqfwPG/h8CH1DoI0FyIyUKQ5hgZZhFF0fq2co+h3M5qGxs9fAmMHx42x74ce\n"\
 "1QIDAQAB\n"\
 "-----END PUBLIC KEY-----\n";

 char privateKey[]="-----BEGIN RSA PRIVATE KEY-----\n"\
 "MIIEowIBAAKCAQEAvGuWqEpy12bYg7k2lMl9icFHoFaBbIRtpN8paouA1H3XQk0N\n"\
 "R/a/QdDO9hez0MyfFDcfwDFy0gTf3MrcS3y8ekhfNogJfcCEwWFib0cUSDfSl0zz\n"\
 "mB0pCjTplCPNcQJYhfKwyITp3ttJ0HK4v5P2kpznUWdRI3AAyQFVGNcR6HTi7aYZ\n"\
 "zicD0/vw00DVl9c9K1me6O77RF4eslQB+jLQqoLqhXSWBHN4/2JcD9n2S4r1ltoo\n"\
 "KWTozAVjMIbzQ1Wg+EXQr7uZmiuh8GfNPSSQYppbXqfwPG/h8CH1DoI0FyIyUKQ5\n"\
 "hgZZhFF0fq2co+h3M5qGxs9fAmMHx42x74ce1QIDAQABAoIBAQCsjk98Mio4UIzL\n"\
 "Haz52AlrRiIFmhOPQTx0uJdiZv4GHHUt2AjotfX0HXnccFqSR0MMrWsbhW2lHyex\n"\
 "OetECqlFx8/CzhT7IHuz6i7OwCa3+L+ZJrUTwujZ2xlrPNqhu0c87MSv7uGL4gIY\n"\
 "r2e4s5YmMSG7MEZjkOh3LQw04ooY+paXXl5hMW9Ke+WgGdBiyR3zfMOAzFjqcgiK\n"\
 "v1Ft/PlVXb3bzZMAVhdHMNtCHHgLWfN0NzBvbXKvy91e8+sYHGNt9UoqBLMkdfKj\n"\
 "gnzBFly8MdFON6rauJpLl/C8uY6zbUTM9fgwbLo/aKrg89tSqM3lucZIXdkHZcnt\n"\
 "RIkvLD6lAoGBAPMuUb6SwSpUTaDiuknwjWBNGliXCUIV2lnMQ2+tyg3qbBhDU1nh\n"\
 "csj9GPDw2PKhMjy8/chf4NvfLsZ/aLHhN4H9wXYN8kvq4u646pUZaAn+AaaWzEzX\n"\
 "QhtF3tUy0n4qXy2ifeN0XkDjo2na/CXvTXJCzTD4W8fwmfO5JDx2bE7XAoGBAMZa\n"\
 "SMtPMJCxxaBCfYJu/iRIte56Q9Nz4bvt/prvNvDDXD4eB9X2IqvDhJBMOMP+WQRn\n"\
 "9zNCR7vydBHcYmKVmRRIsWL/zHr9fhAhZO50LO8R6TdVVZeE2MofmITYXvdlw8FV\n"\
 "j2PqsX1lKXhVCaOvcJBxrhVK/nIfTHuZ0mNTTKYzAoGARmLcxDVD9dWK1otpcidP\n"\
 "NFV8z7F/u4IJvj4SMsvxlM+KVAeAo55OlAA17N85XBjuKohlSE8ADYPuzQNpsTrH\n"\
 "zzc8fMgSnoixAHnDZD6DT4kWvzocjg0sFCrIEZtxM5VeER5cI6617UA2N1hr7Srz\n"\
 "lqPhMlBgeGGaneGzrFB2jucCgYBO4CYkWug4EclBsk8QqCx9p0BZhdsk4eb5AWoG\n"\
 "mGujkDxFI1IAXnFpFkXHz9ee8KzbWLBvfMLj1UUcuSOSimSmMUYA1TGuEtGnfnAR\n"\
 "drdLljbEXzW7HAPNVIUlBpm1DY2reuebVv+fmrPD+yTrJ1++wMX2B61pGlMir4d/\n"\
 "IefQCwKBgFzCtGfxuTDxPVfXXS4Lc3odNp73cDDT6SRzoeUd06JMGaaOflp/mHrV\n"\
 "CWhr835YAgTqst5WoNjfIHDOT1JrYhnyhZ0Yu1kHfTeCnuYZR6jQD65w/7WanZVi\n"\
 "JHz8hrxIxZ7kHpVHsHpQBXvuyCSPqGIl2GqHexdzsw1GiEQgEkmy\n"\
 "-----END RSA PRIVATE KEY-----\n";


unsigned char  encrypted[4098]={};
unsigned char decrypted[4098]={};

int encrypted_length= public_encrypt(plainText,strlen(plainText),publicKey,encrypted);
if(encrypted_length == -1)
{
    printLastError("Public Encrypt failed ");
    exit(0);
}
printf("Encrypted length =%d\n",encrypted_length);

int decrypted_length = private_decrypt(encrypted,encrypted_length,privateKey, decrypted);
if(decrypted_length == -1)
{
    printLastError("Private Decrypt failed ");
    exit(0);
}
printf("Decrypted Text =%s\n",decrypted);
printf("Decrypted Length =%d\n",decrypted_length);


encrypted_length= private_encrypt(plainText,strlen(plainText),privateKey,encrypted);
if(encrypted_length == -1)
{
    printLastError("Private Encrypt failed");
    exit(0);
}
printf("Encrypted length =%d\n",encrypted_length);

decrypted_length = public_decrypt(encrypted,encrypted_length,publicKey, decrypted);
if(decrypted_length == -1)
{
    printLastError("Public Decrypt failed");
    exit(0);
}
printf("Decrypted Text =%s\n",decrypted);
printf("Decrypted Length =%d\n",decrypted_length);



t = clock() - t;
double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
printf("program took %f seconds to execute \n", time_taken);
return EXIT_SUCCESS;
}
