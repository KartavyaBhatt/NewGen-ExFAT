#include <stdio.h>
#include <string.h>
#include "blowfish.h"

int
main (void)
   {
 BLOWFISH_CTX ctx;
   int n;

   /* must be less than 56 bytes */
   char *key = "a random number string would be a better key";
   int keylen = strlen(key);

   uint16_t *plaintext_string = "this is our message";
   int plaintext_len = strlen(plaintext_string);

   uint16_t ciphertext_buffer[256];
   uint16_t *ciphertext_string = &ciphertext_buffer[0];
   int ciphertext_len = 0;

   uint32_t message_left;
   uint32_t message_right;
   int block_len;



   Blowfish_Init(&ctx, key, keylen);

   printf("Plaintext message string is: %s\n", plaintext_string);

   /* encrypt the plaintext message string */
   printf("Encrypted message string is: ");

   while (plaintext_len)
   {
     message_left = message_right = 0UL;

   /* crack the message string into a 64-bit block (ok, really two 32-bit blocks); pad with zeros if necessary */
     for (block_len = 0; block_len < 4; block_len++)
     {
       message_left = message_left << 8;
       if (plaintext_len)
       {
           message_left += *plaintext_string++;
           plaintext_len--;
       }
       else message_left += 0;
     }
     for (block_len = 0; block_len < 4; block_len++)
     {
       message_right = message_right << 8;
       if (plaintext_len)
       {
           message_right += *plaintext_string++;
           plaintext_len--;
       }
       else message_right += 0;
     }
   /* encrypt and print the results */
     Blowfish_Encrypt(&ctx, &message_left, &message_right);
     printf("%lx%lx", message_left, message_right);

   /* save the results for decryption below */
     *ciphertext_string++ = (uint16_t)(message_left >> 24);
     *ciphertext_string++ = (uint16_t)(message_left >> 16);
     *ciphertext_string++ = (uint16_t)(message_left >> 8);
     *ciphertext_string++ = (uint16_t)message_left;
     *ciphertext_string++ = (uint16_t)(message_right >> 24);
     *ciphertext_string++ = (uint16_t)(message_right >> 16);
     *ciphertext_string++ = (uint16_t)(message_right >> 8);
     *ciphertext_string++ = (uint16_t)message_right;
     ciphertext_len += 8;
printf("\n");

   /* reverse the process */
     printf("Decrypted message string is: ");

     ciphertext_string = &ciphertext_buffer[0];
     while(ciphertext_len)
     {
        message_left = message_right = 0UL;

        for (block_len = 0; block_len < 4; block_len++)
        {
          message_left = message_left << 8;
          message_left += *ciphertext_string++;
          if (ciphertext_len)
           ciphertext_len--;
        }
        for (block_len = 0; block_len < 4; block_len++)
        {
           message_right = message_right << 8;
           message_right += *ciphertext_string++;
           if (ciphertext_len)
           ciphertext_len--;
        }

        Blowfish_Decrypt(&ctx, &message_left, &message_right);

   /* if plaintext message string padded, extra zeros here */

        printf("%c%c%c%c%c%c%c%c",
        (int)(message_left >> 24), (int)(message_left >> 16),
        (int)(message_left >> 8), (int)(message_left),
        (int)(message_right >> 24), (int)(message_right >> 16),
        (int)(message_right >> 8), (int)(message_right));
}

printf("\n");

return 0;
}
