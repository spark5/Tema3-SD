#ifndef _HASH_H
#define _HASH_H

#define SEED 0x3252D141

typedef unsigned char uint8_t;
typedef unsigned long uint32_t;

void MurmurHash3_x86_32 ( const void * key, int len,
			 uint32_t seed, void * out );

unsigned int MurmurHash( void *key);

#endif
