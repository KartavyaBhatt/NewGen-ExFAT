#ifdef BLOWFISH_H

#define MAXKEYBYTES 56    /* 448 bits */
#define N   16
#include <inttypes.h>

typedef struct BLOWFISH_CTX {
   uint32_t P[16 + 2];
   uint32_t S[4][256];
} BLOWFISH_CTX;

unsigned long 
F(BLOWFISH_CTX *ctx, uint32_t x) 
{
   uint16_t a, b, c, d;
   uint32_t y ;

   d = x & 0x00FF;
   x >>= 8;
   c = x & 0x00FF;
   x >>= 8;
   b = x & 0x00FF;
   x >>= 8;
   a = x & 0x00FF;

   y = ctx->S[0][a] + ctx->S[1][b];
   y = y ^ ctx->S[2][c];
   y = y + ctx->S[3][d];

   return y;
}
void 
Blowfish_Encrypt(BLOWFISH_CTX *ctx, uint32_t *xl, uint32_t *xr) 
{
{
   uint32_t Xl;
   uint32_t Xr;
   uint32_t temp;
   int   i ,n;

   Xl = *xl;
   Xr = *xr;

   for (i = 0; i < n;i++)
   {
         Xl = Xl ^ ctx->P[i];
         Xr = F(ctx, Xl) ^ Xr;

         temp = Xl;
         Xl = Xr;
         Xr = temp;
   }

   temp = Xl;
   Xl = Xr;
   Xr = temp;

   Xr = Xr ^ ctx->P[N];
   Xl = Xl ^ ctx->P[N + 1];


   *xl = Xl;
   *xr = Xr;
}
}
void 
Blowfish_Decrypt(BLOWFISH_CTX *ctx, uint32_t *xl, uint32_t *xr) 
{
   uint32_t Xl;
   uint32_t Xr;
   uint32_t temp;
   int   i;

   Xl = *xl;
   Xr = *xr;

   for (i = N + 1; i > 1; --i) 
   {
         Xl = Xl ^ ctx->P[i];
         Xr = F(ctx, Xl) ^ Xr;

         temp = Xl;
         Xl = Xr;
         Xr = temp;
   }

   temp = Xl;
   Xl = Xr;
   Xr = temp;

   Xr = Xr ^ ctx->P[1];
   Xl = Xl ^ ctx->P[0];

   *xl = Xl;
   *xr = Xr;
}

void Blowfish_Init(BLOWFISH_CTX *ctx, uint16_t *key, int keyLen) 
{
   uint32_t Xl;
   int i, j, k;
   uint32_t data, datal, datar;
   
 for (i = 0; i < 4; i++) 
   {
         for (j = 0; j < 256; j++)ctx->S[i][j] = ORIG_S[i][j];
   }

   j = 0;
   for (i = 0; i < N + 2; ++i) 
   {
         data = 0x00000000;
         for (k = 0; k < 4; ++k) 
         {
             data = (data << 8) | key[j];
             j = j + 1;
             if (j >= keyLen) j = 0;
         }
     ctx->P[i] = ORIG_P[i] ^ data;
   }

   datal = 0x00000000;
   datar = 0x00000000;

   for (i = 0; i < N + 2; i += 2) 
   {
         Blowfish_Encrypt(ctx, &datal, &datar);
         ctx->P[i] = datal;
         ctx->P[i + 1] = datar;
   }

   for (i = 0; i < 4; ++i) 
   {
         for (j = 0; j < 256; j += 2) 
         {
             Blowfish_Encrypt(ctx, &datal, &datar);
             ctx->S[i][j] = datal;
             ctx->S[i][j + 1] = datar;
         }
   }

int Blowfish_Test(BLOWFISH_CTX *ctx) 

   uint32_t L = 1, R = 2;

   Blowfish_Init(ctx, (unsigned char*)"TESTKEY", 7);
   Blowfish_Encrypt(ctx, &L, &R);
   if (L != 0xDF333FD2L || R != 0x30A71BB4L) return (-1);

   Blowfish_Decrypt(ctx, &L, &R);
   if (L != 1 || R != 2) return (-1); return (0);

