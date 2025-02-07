/* ecrypt-sync.c */

/* *** Please do not edit this file. *** */

#include "ecrypt-sync.h"

#ifdef ECRYPT_USES_DEFAULT_ALL_IN_ONE

/*
 * Default implementation of all-in-one encryption/decryption of
 * (short) packets.
 */

#ifdef ECRYPT_HAS_SINGLE_PACKET_FUNCTION

void ECRYPT_process_packet(
  int action,
  ECRYPT_ctx* ctx,
  const uint8_t* iv,
  const uint8_t* input,
  uint8_t* output,
  uint32_t msglen)
{
  ECRYPT_ivsetup(ctx, iv);

#ifdef ECRYPT_HAS_SINGLE_BYTE_FUNCTION
  ECRYPT_process_bytes(action, ctx, input, output, msglen);
#else
  if (action == 0)
    ECRYPT_encrypt_bytes(ctx, input, output, msglen);
  else
    ECRYPT_decrypt_bytes(ctx, input, output, msglen);
#endif
}

#else

void ECRYPT_encrypt_packet(
  ECRYPT_ctx* ctx,
  const u8* iv,
  const u8* plaintext,
  u8* ciphertext,
  u32 msglen)
{
  ECRYPT_ivsetup(ctx, iv);
  ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, msglen);
}

void ECRYPT_decrypt_packet(
  ECRYPT_ctx* ctx,
  const u8* iv,
  const u8* ciphertext,
  u8* plaintext,
  u32 msglen)
{
  ECRYPT_ivsetup(ctx, iv);
  ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, msglen);
}

#endif

#endif
