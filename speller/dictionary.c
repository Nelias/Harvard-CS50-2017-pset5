/**
 * Implements a dictionary's functionality.
 */
 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "dictionary.h"

int dictionary_size = 0;

// size of a hashtable
#define SIZE 100000

// nodes for a linked list
typedef struct node
{
    char word[LENGTH+1];
    struct node *next;
}
node;

// declare a hash table
node* hashtable[SIZE] = {NULL};

// a hash function for a hash table
int hash (const char *word)
{
    int hash = 2;
    int c;
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (isalpha(word[i]))
        {
            c = word[i] - 'a' + 1;
        }
        else
        {
            c = 27;
        }
            
        hash = ((hash << 5) + hash * 33 + c) % SIZE;
    }
    return hash;    
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // DONE
    // open dictionary
    FILE *large_dictionary = fopen(dictionary, "r");
    if (large_dictionary == NULL)
    {
        printf("could not open dictionary\n");
        return false;
    }
    
    char word[LENGTH+1];

    while (fscanf(large_dictionary, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        
        strcpy(new_node->word, word);
        int index = hash(word);
    
        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_node;
            new_node->next = NULL;
        }
        else
        {
            new_node->next = hashtable[index];
            hashtable[index] = new_node;
        }  
        
        dictionary_size++;
    }
    
    fclose(large_dictionary);
    
    return true;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // DONE
    // a temporary variable that stores a word in all lower case
    char lower_cased_word[LENGTH+1];
    int length_of_word = strlen(word);
    
    for(int i = 0; i < length_of_word; i++)
    {
        lower_cased_word[i] = tolower(word[i]);
        lower_cased_word[length_of_word] = '\0';
    }
    // find what index of the array the word should be in
    int index = hash(lower_cased_word);
    
    // if hashtable is empty at index, return false
    if (hashtable[index] == NULL)
    {
        return false;
    }
    
    node *cursor = hashtable[index];
    
    while (cursor != NULL)
    {
        if (strcmp(lower_cased_word, cursor->word) == 0)
        {
            return true;
        }
        
        cursor = cursor->next;
    }
    
    return false;
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // DONE
    if (dictionary_size > 0)
    {
        return dictionary_size;
    }
    else 
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // DONE
    int index = 0;
    node *head = hashtable[index];
    node *cursor = head;
    
    while (cursor != NULL)
    {
        node *temp = cursor;
        cursor = cursor->next;
        free(temp);
    }
    
    return true;
}