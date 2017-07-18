// The header file for linked list.
# ifndef NIL
# define NIL (void *) 0
# endif
# ifndef MTF
# define MTF true
# endif

# ifndef _LL_H
# define _LL_H

# include <stdbool.h>

extern bool moveToFront;

typedef struct listNode listNode;

struct listNode
{
    char *oldspeak, *newspeak;
    listNode *next;
};

listNode *newNode(const char *, const char *);

void delNode (listNode *);
void delLL (listNode *);

listNode *insertLL(listNode **, const char *, const char *);

listNode *findLL(listNode **, const char *);

void printLL(listNode *);


# endif
