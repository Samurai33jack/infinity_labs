/* hash table api test dict  30/06/24 CR:""*/

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/ht.h"

#define DICTIONARY "/usr/share/dict/words"
#define WORD_SIZE 42
#define WORDS_AMOUNT 104334

char *dict_arr[WORDS_AMOUNT];

char *Strdup(const char *str)
{    
    size_t len = strlen(str);
    char *dup_str = NULL;
    
    assert(str);

    dup_str = (char*)malloc((len + 1) * sizeof(char));

    while (*str != '\0')
    {
        *dup_str = *str;
        (++dup_str, ++str);
    }
    
    *dup_str = '\0';
    
    return (dup_str-len);
}

void CreateDict(char **dict_arr)
{
    FILE *dict = NULL;
    
    size_t words_counter = 0;

    char word[WORD_SIZE];
    char *cur_str = NULL;

    dict = fopen(DICTIONARY,"r");

    if(NULL == dict)
    {
        fprintf(stderr,"Unable to open %s\n",DICTIONARY);
        exit(1);
    }

    while(!feof(dict))
    {
        cur_str = fgets(word,WORD_SIZE,dict);

        if(cur_str == NULL)
        {
            break;
        }

        dict_arr[words_counter] = Strdup(cur_str);
        words_counter++;
    }

    fclose(dict);
}

void FillHashTable(hash_t *table)
{
    size_t i = 0;

    for (i = 0; i < WORDS_AMOUNT; ++i)
    {
        HTInsert(table, dict_arr[i]);
    }
}

void PrintDict(char **dict_arr)
{
    size_t i = 0;

    for (i = 0; i < WORDS_AMOUNT; ++i)
    {
        printf("%s",dict_arr[i]);
    }
}

size_t HashFuncTest(void *val)
{
    char *cur_str = (char *)val;
    size_t i = 0;

    while ((i < WORDS_AMOUNT) && (0 != strcmp(dict_arr[i], cur_str)))
    {
        ++i;
    }

    return i;
}

int CompFuncTest(void *val1, void *val2)
{
    char *str1 = (char *)val1;
    char *str2 = (char *)val2;

    if ((NULL == str1) || (NULL == str2))
    {
        return 0;
    }

    return strcmp(str1, str2);
}

int main()
{
    hash_t *new_hash_table = NULL;
    char user_input[WORD_SIZE];

    CreateDict(dict_arr);

    new_hash_table = HTCreate(WORDS_AMOUNT, HashFuncTest, CompFuncTest);

    FillHashTable(new_hash_table);
    fgets(user_input, WORD_SIZE, stdin);

    while ((0 != strncmp(user_input, "exit", strlen("exit"))))
    {
        if ((WORDS_AMOUNT >= HashFuncTest(user_input)) && (NULL == user_input))
        {
            printf("word not found in dictionary\n");
        }
        else if (HTFind(new_hash_table, user_input))
        {
            printf("word found in the dictionary\n");
        }
        else
        {
            printf("word not found in dictionary\n");
        }

        fgets(user_input, WORD_SIZE, stdin);
    }

    return 0;
}