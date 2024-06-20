#include <cstring>
#include <stdlib.h>
struct vardict
{
    struct vardict* prev;
    char* name;
    double value;
    struct vardict* next;
};
typedef struct vardict vardict;
//i know this is a linked list but i will be calling it an array
void setOrAdd(char* name, double value, vardict* head){
    //if head has no next initialise the array
    if(head->next == NULL){
        vardict* insert = (vardict *) malloc(sizeof(vardict));
        insert->name = name;
        insert->value = value;

        head->next = insert;
        insert->prev = head;
    }
    vardict* current = head->next;

    while (current->next != NULL && strcmp(name, current->name) > 0)
    {
        current = current->next;
    }

    //the name was found, overwrite the old value
    if(strcmp(name, current->name) == 0){
        current->value = value;
        return;
    }
    // make a new dict item
    vardict* insert = (vardict *) malloc(sizeof(vardict));
    insert->name = name;
    insert->value = value;

    //the item alphabetically after the name is found place a new item in front of it
    if(strcmp(name, current->name) < 0){
        current->prev->next = insert;
        insert->prev = current->prev->next;

        current->prev = insert;
        insert->next = current;
        return;
    }

    //we are at the end, insert a new item
    current->next = insert;
    insert->prev = current;
}

double findValue(char* name, vardict* head){
    vardict* current = head;

    while (current->next != NULL && strcmp(name, current->name) > 0)
    {
        current = current->next;
    }

    //the name wasnt found in the array
    if(strcmp(current->name, name) != 0){
        return NULL;
    }

    return current->value;
}