#pragma once
#include <stdint.h>
#include <stddef.h>

#define STATUS_BLOWFISH_SUCCESS 0
#define STATUS_BLOWFISH_INVALID_KEY_LENGTH (-1)
#define STATUS_BLOWFISH_KEY_TOO_LONG (-2)

#define BLOWFISH_LITTLE_ENDIAN 0
#define BLOWFISH_BIG_ENDIAN 1

#define BLOWFISH_MIN_KEY_LENGTH 1
#define BLOWFISH_MAX_KEY_LENGTH 56

#define BLOWFISH_BLOCK_SIZE 8

#if defined(__cplusplus)
extern "C" {
#endif

    typedef struct _ACCEL_BLOWFISH_KEY {
        uint32_t SubKey[18];
        uint32_t SBox[4][256];
    } ACCEL_BLOWFISH_KEY;

    void accel_Blowfish_encrypt(uint8_t srcBytes[8],
                                const ACCEL_BLOWFISH_KEY* srcKey,
                                int Endian);

    void accel_Blowfish_decrypt(uint8_t srcBytes[8],
                                const ACCEL_BLOWFISH_KEY* srcKey,
                                int Endian);

    int accel_Blowfish_set_key(const uint8_t srcUserKey[], uint8_t UserKeyLength,
                               ACCEL_BLOWFISH_KEY* dstKey);

#if defined(__cplusplus)
}
#endif
