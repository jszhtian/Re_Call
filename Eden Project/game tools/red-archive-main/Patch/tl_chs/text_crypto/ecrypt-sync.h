/* ecrypt-sync.h */

/* 
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 * 
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef ECRYPT_SYNC
#define ECRYPT_SYNC

#include "ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/* 
 * The name of your cipher.
 */

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; ECRYPT_KEYSIZE(i) <= ECRYPT_MAXKEYSIZE; ++i)
 *   {
 *     keysize = ECRYPT_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define ECRYPT_MAXKEYSIZE 256                 /* [edit] */
#define ECRYPT_KEYSIZE(i) (128 + (i)*32)      /* [edit] */

#define ECRYPT_MAXIVSIZE 256                  /* [edit] */
#define ECRYPT_IVSIZE(i) (64 + (i)*64)        /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/* 
 * ECRYPT_ctx is the structure containing the representation of the
 * internal state of your cipher. 
 */

typedef struct
{
  /* 
   * [edit]
   *
   * Put here all state variable needed during the encryption process.
  */
  uint32_t P[1024];
  uint32_t Q[1024];
  uint32_t counter2048;
  uint32_t key[8];
  uint32_t iv[8];
  uint32_t keysize;   /* key size in bytes */
  uint32_t ivsize;    /* iv size in bytes  */ 
} ECRYPT_ctx;

/* ------------------------------------------------------------------------- */

/* Mandatory functions */

/*
 * Key and message independent initialization. This function will be
 * called once when the program starts (e.g., to build expanded S-box
 * tables).
 */
void ECRYPT_init(void);

/*
 * Key setup. It is the user's responsibility to select the values of
 * keysize and ivsize from the set of supported values specified
 * above.
 */
void ECRYPT_keysetup(
  ECRYPT_ctx* ctx, 
  const uint8_t* key, 
  uint32_t keysize,                /* Key size in bits. */ 
  uint32_t ivsize);                /* IV size in bits. */ 
  
/*
 * IV setup. After having called ECRYPT_keysetup(), the user is
 * allowed to call ECRYPT_ivsetup() different times in order to
 * encrypt/decrypt different messages with the same key but different
 * IV's.
 */
void ECRYPT_ivsetup(
  ECRYPT_ctx* ctx, 
  const uint8_t* iv);

/*
 * Encryption/decryption of arbitrary length messages.
 *
 * For efficiency reasons, the API provides two types of
 * encrypt/decrypt functions. The ECRYPT_encrypt_bytes() function
 * (declared here) encrypts byte strings of arbitrary length, while
 * the ECRYPT_encrypt_blocks() function (defined later) only accepts
 * lengths which are multiples of ECRYPT_BLOCKLENGTH.
 * 
 * The user is allowed to make multiple calls to
 * ECRYPT_encrypt_blocks() to incrementally encrypt a long message,
 * but he is NOT allowed to make additional encryption calls once he
 * has called ECRYPT_encrypt_bytes() (unless he starts a new message
 * of course). For example, this sequence of calls is acceptable:
 *
 * ECRYPT_keysetup();
 *
 * ECRYPT_ivsetup();
 * ECRYPT_encrypt_blocks();
 * ECRYPT_encrypt_blocks();
 * ECRYPT_encrypt_bytes();
 *
 * ECRYPT_ivsetup();
 * ECRYPT_encrypt_blocks();
 * ECRYPT_encrypt_blocks();
 *
 * ECRYPT_ivsetup();
 * ECRYPT_encrypt_bytes();
 * 
 * The following sequence is not:
 *
 * ECRYPT_keysetup();
 * ECRYPT_ivsetup();
 * ECRYPT_encrypt_blocks();
 * ECRYPT_encrypt_bytes();
 * ECRYPT_encrypt_blocks();
 */

/*
 * By default ECRYPT_encrypt_bytes() and ECRYPT_decrypt_bytes() are
 * defined as macros which redirect the call to a single function
 * ECRYPT_process_bytes(). If you want to provide separate encryption
 * and decryption functions, please undef
 * ECRYPT_HAS_SINGLE_BYTE_FUNCTION.
 */
#define ECRYPT_HAS_SINGLE_BYTE_FUNCTION  1  /* [edit] */
#ifdef ECRYPT_HAS_SINGLE_BYTE_FUNCTION

#define ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, msglen)   \
  ECRYPT_process_bytes(0, ctx, plaintext, ciphertext, msglen)

#define ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, msglen)   \
  ECRYPT_process_bytes(1, ctx, ciphertext, plaintext, msglen)

void ECRYPT_process_bytes(
  int action,                 /* 0 = encrypt; 1 = decrypt; */
  ECRYPT_ctx* ctx, 
  const uint8_t* input, 
  uint8_t* output, 
  uint32_t msglen);                /* Message length in bytes. */ 

#else

void ECRYPT_encrypt_bytes(
  ECRYPT_ctx* ctx, 
  const u8* plaintext, 
  u8* ciphertext, 
  u32 msglen);                /* Message length in bytes. */ 

void ECRYPT_decrypt_bytes(
  ECRYPT_ctx* ctx, 
  const u8* ciphertext, 
  u8* plaintext, 
  u32 msglen);                /* Message length in bytes. */ 

#endif

/* ------------------------------------------------------------------------- */

/* Optional features */

/* 
 * For testing purposes it can sometimes be useful to have a function
 * which immediately generates keystream without having to provide it
 * with a zero plaintext. If your cipher cannot provide this function
 * (e.g., because it is not strictly a synchronous cipher), please
 * reset the ECRYPT_GENERATES_KEYSTREAM flag.
 */

#define ECRYPT_GENERATES_KEYSTREAM
#ifdef ECRYPT_GENERATES_KEYSTREAM

void ECRYPT_keystream_bytes(
  ECRYPT_ctx* ctx,
  uint8_t* keystream,
  uint32_t length);                /* Length of keystream in bytes. */

#endif

/* ------------------------------------------------------------------------- */

/* Optional optimizations */

/* 
 * By default, the functions in this section are implemented using
 * calls to functions declared above. However, you might want to
 * implement them differently for performance reasons.
 */

/*
 * All-in-one encryption/decryption of (short) packets.
 *
 * The default definitions of these functions can be found in
 * "ecrypt-sync.c". If you want to implement them differently, please
 * undef the ECRYPT_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define ECRYPT_USES_DEFAULT_ALL_IN_ONE        /* [edit] */

/*
 * Undef ECRYPT_HAS_SINGLE_PACKET_FUNCTION if you want to provide
 * separate packet encryption and decryption functions.
 */
#define ECRYPT_HAS_SINGLE_PACKET_FUNCTION     /* [edit] */
#ifdef ECRYPT_HAS_SINGLE_PACKET_FUNCTION

#define ECRYPT_encrypt_packet(                                        \
    ctx, iv, plaintext, ciphertext, mglen)                            \
  ECRYPT_process_packet(0,                                            \
    ctx, iv, plaintext, ciphertext, mglen)

#define ECRYPT_decrypt_packet(                                        \
    ctx, iv, ciphertext, plaintext, mglen)                            \
  ECRYPT_process_packet(1,                                            \
    ctx, iv, ciphertext, plaintext, mglen)

void ECRYPT_process_packet(
  int action,                 /* 0 = encrypt; 1 = decrypt; */
  ECRYPT_ctx* ctx, 
  const uint8_t* iv,
  const uint8_t* input, 
  uint8_t* output, 
  uint32_t msglen);

#else

void ECRYPT_encrypt_packet(
  ECRYPT_ctx* ctx, 
  const u8* iv,
  const u8* plaintext, 
  u8* ciphertext, 
  u32 msglen);

void ECRYPT_decrypt_packet(
  ECRYPT_ctx* ctx, 
  const u8* iv,
  const u8* ciphertext, 
  u8* plaintext, 
  u32 msglen);

#endif

/*
 * Encryption/decryption of blocks.
 * 
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * ECRYPT_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define ECRYPT_BLOCKLENGTH 4                  /* [edit] */

#define ECRYPT_USES_DEFAULT_BLOCK_MACROS      /* [edit] */
#ifdef ECRYPT_USES_DEFAULT_BLOCK_MACROS

#define ECRYPT_encrypt_blocks(ctx, plaintext, ciphertext, blocks)  \
  ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext,                 \
    (blocks) * ECRYPT_BLOCKLENGTH)

#define ECRYPT_decrypt_blocks(ctx, ciphertext, plaintext, blocks)  \
  ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext,                 \
    (blocks) * ECRYPT_BLOCKLENGTH)

#ifdef ECRYPT_GENERATES_KEYSTREAM

#define ECRYPT_keystream_blocks(ctx, keystream, blocks)            \
  ECRYPT_keystream_bytes(ctx, keystream,                           \
    (blocks) * ECRYPT_BLOCKLENGTH)

#endif

#else

/*
 * Undef ECRYPT_HAS_SINGLE_BLOCK_FUNCTION if you want to provide
 * separate block encryption and decryption functions.
 */
#define ECRYPT_HAS_SINGLE_BLOCK_FUNCTION      /* [edit] */
#ifdef ECRYPT_HAS_SINGLE_BLOCK_FUNCTION

#define ECRYPT_encrypt_blocks(ctx, plaintext, ciphertext, blocks)     \
  ECRYPT_process_blocks(0, ctx, plaintext, ciphertext, blocks)

#define ECRYPT_decrypt_blocks(ctx, ciphertext, plaintext, blocks)     \
  ECRYPT_process_blocks(1, ctx, ciphertext, plaintext, blocks)

void ECRYPT_process_blocks(
  int action,                 /* 0 = encrypt; 1 = decrypt; */
  ECRYPT_ctx* ctx, 
  const u8* input, 
  u8* output, 
  u32 blocks);                /* Message length in blocks. */

#else

void ECRYPT_encrypt_blocks(
  ECRYPT_ctx* ctx, 
  const u8* plaintext, 
  u8* ciphertext, 
  u32 blocks);                /* Message length in blocks. */ 

void ECRYPT_decrypt_blocks(
  ECRYPT_ctx* ctx, 
  const u8* ciphertext, 
  u8* plaintext, 
  u32 blocks);                /* Message length in blocks. */ 

#endif

#ifdef ECRYPT_GENERATES_KEYSTREAM

void ECRYPT_keystream_blocks(
  ECRYPT_ctx* ctx,
  u8* keystream,
  u32 blocks);                /* Keystream length in blocks. */ 

#endif

#endif

/*
 * If your cipher can be implemented in different ways, you can use
 * the ECRYPT_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DECRYPT_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (ECRYPT_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same ECRYPT_BLOCKLENGTH, etc.). 
 */
#define ECRYPT_MAXVARIANT 1                   /* [edit] */

#ifndef ECRYPT_VARIANT
#define ECRYPT_VARIANT 1
#endif

#if (ECRYPT_VARIANT > ECRYPT_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif


/* ===========================================================================

                 Added functions:    HC-256 Mandatory functions 

============================================================================== */

#define f1(x)    (ROTR32((x),7) ^ ROTR32((x),18) ^ ((x) >> 3))
#define f2(x)    (ROTR32((x),17) ^ ROTR32((x),19) ^ ((x) >> 10))



__inline uint32_t h1(ECRYPT_ctx* ctx, uint32_t u) {
	uint32_t tem; 			
	unsigned char a,b,c,d;			
	a = (unsigned char) ((u));		
	b = (unsigned char) ((u) >> 8);	
	c = (unsigned char) ((u) >> 16);	
	d = (unsigned char) ((u) >> 24);	
	tem = ctx->Q[a]+ctx->Q[256+b]+ctx->Q[512+c]+ctx->Q[768+d];
	return (tem);
}



__inline uint32_t h2(ECRYPT_ctx* ctx, uint32_t u) {
	uint32_t tem; 			
	unsigned char a,b,c,d;			
	a = (unsigned char) ((u));		
	b = (unsigned char) ((u) >> 8);	
	c = (unsigned char) ((u) >> 16);	
	d = (unsigned char) ((u) >> 24);	
	tem = ctx->P[a]+ctx->P[256+b]+ctx->P[512+c]+ctx->P[768+d];
	return (tem);
}



__inline uint32_t generate(ECRYPT_ctx* ctx) /*one step of the cipher*/
{
		uint32_t i,i3, i10, i12, i1023;
		uint32_t output;

		i   = ctx->counter2048 & 0x3ff;
		i3  = (i - 3) & 0x3ff;
		i10 = (i - 10) & 0x3ff;
		i12 = (i - 12) & 0x3ff;
		i1023 = (i - 1023) & 0x3ff;

		if (ctx->counter2048 < 1024) {
			ctx->P[i] = ctx->P[i] + ctx->P[i10] + (ROTR32(ctx->P[i3],10)^ROTR32(ctx->P[i1023],23))+ctx->Q[(ctx->P[i3]^ctx->P[i1023])&0x3ff];
			output = h1(ctx,ctx->P[i12]) ^ ctx->P[i];
		}
		else {                                   
			ctx->Q[i] = ctx->Q[i] + ctx->Q[i10] + (ROTR32(ctx->Q[i3],10)^ROTR32(ctx->Q[i1023],23))+ctx->P[(ctx->Q[i3]^ctx->Q[i1023])&0x3ff];
			output = h2(ctx, ctx->Q[i12]) ^ ctx->Q[i];
		}
		ctx->counter2048 = (ctx->counter2048+1) & 0x7ff;
		return (output);
}





__inline void ECRYPT_init(void) {
}  /* No operation performed */


__inline void ECRYPT_keysetup(
  ECRYPT_ctx* ctx, 
  const uint8_t* key, 
  uint32_t keysize,                /* Key size in bits. */ 
  uint32_t ivsize)                 /* IV size in bits. */
{ 
  uint32_t i;  

  ctx->keysize = keysize >> 3;
  ctx->ivsize = ivsize >> 3;
  
  for (i = 0; i < 8; i++) ctx->key[i] = 0;
  for (i = 0; (i < ctx->keysize) & (i < 32); i++) {
	  	ctx->key[i >> 2] =  ctx->key[i >> 2] | key[i];
		ctx->key[i >> 2] = ROTL32(ctx->key[i >> 2],8);
  }
 
} /* initialize the key, save the iv size*/




__inline void ECRYPT_ivsetup(
  ECRYPT_ctx* ctx, 
  const uint8_t* iv)
{ 
    uint32_t W[2560],i;
  
    /* initialize the iv */
    for (i = 0; i < 8; i++) ctx->iv[i] = 0;
    for (i = 0; (i < ctx->ivsize) & (i < 32); i++) {
		ctx->iv[i >> 2] =  ctx->iv[i >> 2] | iv[i];
		ctx->iv[i >> 2] = ROTL32(ctx->iv[i >> 2],8);
    }
  
    /* setup the table P and Q */ 

	for (i = 0; i < 8;  i++) W[i] = ctx->key[i];
	for (i = 8; i < 16; i++) W[i] = ctx->iv[i-8];

	for (i = 16; i < 2560; i++) W[i] = f2(W[i-2]) + W[i-7] + f1(W[i-15]) + W[i-16]+i; 

	for (i = 0; i < 1024; i++)  ctx->P[i] = W[i+512];
	for (i = 0; i < 1024; i++)  ctx->Q[i] = W[i+1536];

	ctx->counter2048 = 0;
      
      /* run the cipher 4096 steps before generating the output */
	for (i = 0; i < 4096; i++)  generate(ctx);  
}



__inline void ECRYPT_process_bytes(
  int action,                 /* 0 = encrypt; 1 = decrypt; */
  ECRYPT_ctx* ctx, 
  const uint8_t* input, 
  uint8_t* output, 
  uint32_t msglen)                /* Message length in bytes. */ 
{
  uint32_t i, j, msglen32, keystreamword;

  msglen32 = msglen >> 2;

  for (i = 0; i < msglen32; i++) { 
      keystreamword = generate(ctx);                 /*generate a 32-bit word*/
      for (j = 0; j < 4; j++) {
	      *output = *input ^ keystreamword;       /*encrypt one byte*/
          output += 1; 
          input +=1;
		  keystreamword = keystreamword >> 8;
	  }
  }  

  keystreamword = generate(ctx);
  for (i = 0; i < (msglen & 3); i++) {
 	  *output = *input ^ keystreamword;       /*encrypt one byte*/
      output += 1; 
      input +=1;
      keystreamword = keystreamword >> 8;
  }
}

