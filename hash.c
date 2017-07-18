// This file contains all the method for hash fuction.
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include "aes.h"
# include "hash.h"
# include "ll.h"

static inline int realLength ( int l)
{
	return 16 * (l / 16 + (l % 16 ? 1 : 0) );
}

uint32_t hash ( uint32_t salt[], const char * key )
{
	uint32_t output [4] = { 0x0 };
	uint32_t sum = 0x0;

	int keyL = strlen ( key ) ;



	uint8_t * realKey = ( uint8_t *) calloc ( realLength ( keyL ) , sizeof (uint8_t ) ) ;
	memcpy ( realKey , key , keyL ) ;
	for ( int i = 0; i < realLength ( keyL ) ; i += 16)
	{
		AES128_ECB_encrypt (( uint8_t *) salt, ( uint8_t *) realKey + i, ( uint8_t *) output ) ; // Output
		sum ^= output [0] ^ output [1] ^ output [2] ^ output [3];
	}
	free(realKey);
	return sum;
}

hashTable * newHT ( uint32_t l, uint32_t s[])
{
    struct hashTable *tempTable;
	tempTable=malloc(sizeof(hashTable)); // Allocates the memory for the tempTable.
    tempTable->l=l;
    for(int i=0;i<4;i++){
        tempTable->s[i]=s[i]; // Puts all the salts.
    }
	tempTable->h=calloc(l,sizeof(listNode)); // Makes all the table for size.
    return tempTable;
}

void delHT ( hashTable *h)
{
	//free(h->s);

	//Help from TA
	for(uint32_t i=0; i<h->l; i++) // The TA helped for deleteing the hash table.
	{
		delLL(h->h[i]);
	}


	free(h->h);
	free(h);

}

listNode * findHT ( hashTable *tempTable , const char *oldspeak) // Finds the hashtable, which calls the linked list.
{
	//should have findLL somewhere
	uint32_t k = (hash(tempTable->s,oldspeak))%(tempTable->l);


	return findLL(&(tempTable->h[k]),oldspeak);



}

void insertHT ( hashTable * tempTable, const char * oldspeak, const char *newspeak)
{
	uint32_t k = hash(tempTable->s,oldspeak)%tempTable->l;
	//uint32_t k2 = hash(tempTable->s,newspeak)%tempTable->l;
	if (tempTable->h[k]==NULL)
	{
		tempTable->h[k]= newNode(oldspeak,newspeak);

		//printf("\n%d:%s\n",count,oldspeak);
	}
	else
	{

		//printf("\nEnters linked list:%d:%s\n",count,oldspeak);
		tempTable->h[k]=insertLL(&(tempTable->h[k]),oldspeak,newspeak);// If there is already a hash key.

	}



}

void printHT ( const hashTable *tempTable)
{
	for (uint32_t i=0; i<tempTable->l;i++)
	{
		listNode *a=tempTable->h[i]; // Prints the hash table.
		printLL(a);

	}

}
