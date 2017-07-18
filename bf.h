# ifndef NIL
# define NIL (void *) 0
# endif
# ifndef _BF_H
# define _BF_H
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <getopt.h>
# include "hash.h"


typedef struct bloomF {
    uint8_t *v; // Vector
    uint32_t l; // Length
    uint32_t s[4]; // Salt
} bloomF;

// Each function has its own hash function, determined by the salt.

uint32_t hashBF(bloomF *, char *);
uint32_t hashBF(bloomF *x, char *key){
    uint32_t k= hash(x->s,key);
    uint32_t a = (k)%(x->l);
    //printf("The length is: %d",x->l);
    //printf("The hash key is: %d\n",a);
    return a;


}

// Create a new Bloom Filter of a given length and hash funtion.

static inline bloomF *newBF(uint32_t l, uint32_t b[])
{
    struct bloomF *newBloom;
    newBloom = (bloomF *)malloc(sizeof(struct bloomF)); // Allocating memory for newBloom.
    newBloom->v = (uint8_t *) calloc(l / 8 + 1, sizeof(uint8_t)); // Making all the bits in the bitvector as zero.
    newBloom->l=l; // The length is same.

    for(int i=0;i<4;i++)
    {
        newBloom->s[i]=b[i]; // Initializing all the salt for the bloomF struct.
    }


    return newBloom;
}

// Delete a Bloom Filter

static inline void delBF(bloomF *v)
{

    free(v->v); // Deletes the vector for the bloom filter.
    //v->l=NULL;
    free(v); // Frees the v for the bloom filter.
}

// Return the value of position k in the Bloom Filter

static inline uint32_t valBF(bloomF *x, uint32_t k)//was uint8_t instead of uint32_t
{
    return ((x->v[k / 8] & (0x1 << k % 8)) != 0); // This detects whether the bitvector is zero or one.
}

static inline uint32_t lenBF(bloomF *x){ return x->l; } // Returns the length of the bloom filter.

// Count bits in the Bloom Filter

static inline uint32_t countBF(bloomF *b) // Counts the bloom filter.
{
    uint32_t count =0;
    for (uint32_t i = 0; i < lenBF(b); ++i) {// The loop converts all the bits to one
        if (valBF(b, i)==1){ // If the bloom filter is one.
            count++;
        }
    }
    return count;
}

// Set an entry in the Bloom filter

static inline void setBF(bloomF *x, char * key)// Sets the bloom filter to one.
{

    uint32_t k= hashBF(x,key);

    //printf("\n%d, %s",k,key);
    x->v[k / 8] |=  (0x1 << k % 8);
}

// Clear an entry in the Bloom Filter

static inline void clrBF(bloomF *x, char *key) // Clears the bloom filter.
{
    uint32_t k= hashBF(x,key);
    x->v[k / 8] &= ~(0x1 << k % 8);
}

// Check membership in the Bloom Filter

static inline uint32_t memBF(bloomF *x, char *key) // If the key is part of the bloom filter.
{
    uint32_t k= hashBF(x,key);//was uint8_t
    return ((x->v[k / 8] & (0x1 << k % 8)) != 0);
}

static inline void printBF(bloomF *x) // Prints all the bits.
{

    for (uint32_t i=0;i< x->l;i++){
        printf("%d",valBF(x,i));
    }
}


# endif
