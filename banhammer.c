// banhammer.c - contains the main file for this program. It implements bloom filter, hash table and linked list.

# include "bf.h"
# include "hash.h"
# include "ll.h"
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <stdint.h>
# include <stdbool.h>
# include <ctype.h>

bool moveToFront = false; // This is off by default. If the user puts the -m case, it enables the move to front.
uint32_t Seeks = 0; // These are the statisitics that are going to be printed.
double Average = 0; // Counts the average.Calls to linked list divided by the seeks.
uint32_t Dictionary = 0; // This is number of bad words from bad speak.
uint32_t Translations = 0; // This is the number of translations from newspeak.
uint32_t Text = 0; //This is the number of words from the inputted text or file.
uint32_t counting = 0; //This counts how many times the findLL is called.
double DensitiesA; // This is the ratio of bits set to total number of bits.
double DensitiesB; // This is same for the 2nd bloom filter.

char* concat(const char *word1, const char *word2) // I use this method to concatnate the oldspeak and newspeak from newspeak.txt.https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
{
    char *concatWord = malloc(strlen(word1)+strlen(word2)+1); // Initializing the memory for the two words.
    strcpy(concatWord, word1); // Copies word1.
    strcat(concatWord, word2); // Concatnate the first word and the second word.
    return concatWord; // Returns the conncatnated word.
}

int main(int argc, char *argv[])
{
	uint32_t def = 0; // This is for the statistics. It is false by default.
	uint32_t BFSz = 1048576; // This is the default bloom filter size.
	uint32_t hashTableSize = 10000; // This is the default hash table size.
    opterr = 0; //These are used for reading opt.
    uint32_t option;
    uint32_t negativeOne = -1;
    while ((option = getopt(argc, argv, " sh:f:mb")) != negativeOne)//getopt has all the different options.
    {
        switch (option) //Switch is used for efficiency.
        {
            case 's': // S is used for statistics.
            {
                def = 1; // Chages the defailt to one.
				break;
            }
            case 'h': // This allows the user to change the hash table size.
            {
				hashTableSize = atoi(optarg);
                break;
            }
            case 'f': // This allows the user to change the bloom filter size.
            {
				BFSz = atoi(optarg);
                break;
            }
            case 'm': // This enables move to front rule.
            {
                moveToFront = true;
                break;
            }
            case 'b': // This is so that moveToFront is disabled.
            {
                moveToFront = false;
                break;
            }
            default:
            {
                fprintf(stderr, "[UNKNOWN ERROR] Something very bad happend\n");//Indicates that something didn't go right.
                return 2;
            }
        }
    }

    uint32_t initA[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0aB0a}; // First Bloom filter
    uint32_t initB[] = {0xDeadBeef, 0xFadedB0a, 0xCafeD00d, 0xC0c0aB0a}; // Second Bloom filter
    uint32_t initH[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0Babe}; // Hash table
    bloomF *a = newBF(BFSz, initA); // Creates the first bloom filter.
    bloomF *b = newBF(BFSz, initB); // Creates the second bloom filter.
    hashTable *h= newHT(hashTableSize,initH); // Creates a new hash table.


	char badWord[100]; // The buffer size character for reading badspeak.
    FILE* badWordf= fopen("badspeak.txt","r"); // Reading badspeak.txt.
    while(!feof(badWordf))
    {
        fscanf(badWordf, "%s\n",badWord); // Scans whenever there is a new line.
		Dictionary++; // Count the number of words in badspeak.
		setBF(a,badWord); // Sets the first bloom filter.
		setBF(b,badWord); // Sets the second bloom filter.
		insertHT(h,badWord,"NONE"); // Inserts the bad word in the hash table.
    }
	fclose(badWordf); // Closes the badword file.

    char old[100]; // The buffer for old file.
    char new[100]; // The buffer for new file.
    FILE* newspeakf= fopen("newspeak.txt","r"); // This is reading the newspeak file.
    while(!feof(newspeakf))
    {
        fscanf(newspeakf, "%s %s \n",old, new); // Scans the newspeak file if there is a space.
		Translations++; // Translations keeps track of the number of words in newspeak.txt.
		setBF(a,old); // Sets the first bloom filter.
		setBF(b,old); // Sets the second bloom filter.
		insertHT(h,old,new); // Inserts the word in the hash table.
    }
	fclose(newspeakf);

	int k=0;
	char **words1=malloc(k*sizeof(char*)); // This keeps track of badspeak in the user file or text.

	int p=0;
	char **words2=malloc(p*sizeof(char*)); // This keeps track of oldspeak and newspeak.

    int freeScanner(void); // This is used to free the scannner at the end.
	int yylex(void); // These are required to read lex file.
	extern char* yytext;
	extern FILE* yyin;
	int tokenID = 0;
	yyin = stdin;
    while(tokenID != -1)
    { // Lex returns -1 at the end of file.
	   tokenID = yylex();
	   Text++;
	   for(int i = 0; yytext[i]; i++)
       { // This makes everything lower case.
  			yytext[i] = tolower(yytext[i]);
		}
	   if (memBF(a,yytext)==1 && memBF(b,yytext)==1)
       { // If both of the bloom filters are on.
		   listNode *tempCheck=findHT(h,yytext);
		   if (tempCheck!=NULL && strcmp(tempCheck->oldspeak,yytext)==0 )
           { // If the node is not empty. then it string compares both words.
			   if(strcmp(tempCheck->newspeak,"NONE")==0)
               {
				   words1=realloc(words1,(k+1)*sizeof(char*)); // It puts the word in words 1.
				   words1[k]=tempCheck->oldspeak;
				   k++;
			   }
			   else
               {
				   words2=realloc(words2,(p+1)*sizeof(char*)); // Reallocates the second hard.
                   char *concatnated = concat(tempCheck->oldspeak,concat(" -> ",tempCheck->newspeak));

				   words2[p] = concatnated;//concat(tempCheck->oldspeak,concat(" -> ",tempCheck->newspeak)); // It puts the concatnated word in words2p.
				   p++;
			   }
		   }
	   }
    }

	if (def==0)
    { // This happens when statistics is off.
		uint32_t printed=0; // This is used to print it in reverse.
		int m=0;
		char **wordsReversed=malloc(m*sizeof(char*)); // The reversed array is printed here.
		for(int i=0;i<k;i++){
			printed=0; // First it is not printed
			for(int j=0;j<i;j++)
            {
				if (strcmp(words1[i],words1[j])==0) // String compares both words.
                {
					printed=1; //This means it is printed.
				}
			}
			if(printed==0)
            {
				wordsReversed=realloc(wordsReversed,(m+1)*sizeof(char*));
				wordsReversed[m]=words1[i]; // Puts the word in wordsReversed.
				m++;
			}

		}

		if(k!=0) // Prints The commands.
        {
			printf("Dear Comrade,\n\nYou have chosen to use degenerate words that may cause hurt\nfeelings or cause your comrades to think unpleasant thoughts.\nThis is doubleplus bad. To correct your wrongthink and\nsave community consensus we will be sending you to joycamp\nadministered by Miniluv.\n\nYour errors:\n\n");
		}
		if(k==0 && p!=0)
        {
			printf("Dear Comrade,\n\nSubmitting your text helps to preserve feelings and prevent\nbadthink. Some of the words that you used are not goodspeak.\nThe list shows how to turn the oldspeak words into newspeak.\n\n");

		}

		for (int i=m-1; i>=0;i--)
        {
			printf("%s\n",wordsReversed[i]); // Prints the word in reversed.
		}

		if(p!=0)
        {
			printf("\nThink on these words during your vacation!\n\n"); // Prints between badspeak and oldspeak and newspeak.
		}

		uint32_t printed2=0;

		int m2=0;
		char **wordsReversed2=malloc(m2*sizeof(char*)); // This is for the oldspeak and newspeak.

		for(int i=0;i<p;i++)
        {
			printed2=0;
			for(int j=0;j<i;j++)
            {
				if (strcmp(words2[i],words2[j])==0)
                {
					printed2=1;
				}
			}
			if(printed2==0)
            {
				wordsReversed2=realloc(wordsReversed2,(m2+1)*sizeof(char*)); // Puts the word in the reversed array.
				wordsReversed2[m2]=words2[i];
				m2++;
			}
		}
		for (int i=m2-1; i>=0;i--)
        {
			printf("%s\n",wordsReversed2[i]); // Prints the copression.
		}
		free(wordsReversed);
		free(wordsReversed2);
	}
	if(def==1) // This happens only when the statistics is on.
    {
		double doubleCountBFA = countBF(a); // Double so when it prints double.
		double lenCountBFA=lenBF(a);
		DensitiesA=doubleCountBFA/lenCountBFA;

		double doubleCountBFB = countBF(b);
		double lenCountBFB=lenBF(b);
		DensitiesB=doubleCountBFB/lenCountBFB;
        double count=counting; // Takes in double so it prints out double.
        double seek=Seeks;
        Average=count/seek;
		printf("Seeks %d, Average %f, Dictionary %d, Translations %d, Text %d, Densities: %f, %f\n",Seeks, Average, Dictionary, Translations, Text, DensitiesA,DensitiesB);

	}
	free(words1); // Frees everything.
	free(words2);
	delBF(a);
	delBF(b);
	delHT(h);
    freeScanner();
    return 0;
}
