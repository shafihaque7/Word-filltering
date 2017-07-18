// This file has all the method for linked list.
# include "ll.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdint.h>


extern uint32_t Seeks;
extern uint32_t counting;


listNode *newNode(const char *oldspeak, const char *newspeak)
{
    struct listNode *tempNode = (listNode *)malloc(sizeof(listNode)); // Allocates memory for the new node.
    tempNode->oldspeak=strdup(oldspeak); // Copying the string for oldspeak.
    tempNode->newspeak=strdup(newspeak);
    tempNode->next=NIL; // Nothing is connected to next.
    return tempNode;
}

void delNode (listNode *node)
{
    free(node->oldspeak); // Frees oldspeak and newspeak.
    free(node->newspeak);
    free(node);
    return;
}
void delLL (listNode *node)
{
    while(node!=NIL)
    {
        listNode *temp=node; // Deltes the linked list.
        node=node->next;
        delNode(temp);
    }
    return;

}

listNode *insertLL(listNode **head, const char *oldspeak, const char *newspeak)
{

    listNode *t = newNode(oldspeak,newspeak);//Help from a TA
    t->next = *head; // Next is head.
    *head=t; // Head is t.
     return t;
}




listNode *findLL(listNode **head, const char *oldspeak)
{
    counting++;

    if (moveToFront==true){ // This enables move to front rule.
        listNode *current=*head; // Stores the head.
        listNode *previous=NIL; // Stores the previous.
        while(current!=NIL){ // If there is something in the linked list.
            Seeks++;
            if (strcmp(current->oldspeak,oldspeak)==0) // Compares the two strings.
            {
                if(current == *head){ // If the current is same as the head, means it doesn't have to go to the linked list.
                    return *head;
                }
                if(previous!=NIL){ // If there is a previous item in the linked list.
                    previous->next=current->next; // Stores the previous next as the current next.
                    current->next=*head; // Stores the current next as the head.
                    *head=current; // Stores the head for the current node.
                    return *head; // Returns the head.
                }
            }
            previous=current; // Makes previous the same as the current.
            current=current->next; // The current becomes the next of current.
        }
    }
    if (moveToFront==false) // This is when the find linked list is off.
    {
         for (listNode *t=*head;t!=NIL;t=t->next)
         {
             Seeks++;
             if(strcmp(t->oldspeak,oldspeak)==0) // If the string compare is some.
             {
                 return t;
             }
         }


    }
    return NIL;

}

void printLL(listNode *node)
{
    for (listNode *temp=node; temp!=NIL; temp = temp->next) // Prints all the linked list.
    {
        printf("%s ", temp->oldspeak); // Prints the oldspeak and newspeak.
        printf("%s ", temp->newspeak);
    }
    printf("\n");
    return;
}
