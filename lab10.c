#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
};

struct Trie
{
    struct TrieNode *root;
};

struct TrieNode *createTrieNode()
{
    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    pNode->isEndOfWord = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        pNode->children[i] = NULL;
    }
    return pNode;
}

struct Trie *createTrie()
{
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    trie->root = createTrieNode();
    return trie;
}

void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *currentNode = pTrie->root;

    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) continue;
        if (currentNode->children[index] == NULL)
        {
            currentNode->children[index] = createTrieNode();
        }

        currentNode = currentNode->children[index];
    }

    currentNode->isEndOfWord++;
}

int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct TrieNode *currentNode = pTrie->root;

    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) return 0;
        if (currentNode->children[index] == NULL)
        {
            return 0;
        }

        currentNode = currentNode->children[index];
    }

    return currentNode->isEndOfWord;
}

void deallocateTrieNode(struct TrieNode *node)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != NULL)
        {
            deallocateTrieNode(node->children[i]);
        }
    }
    free(node);
}

struct Trie *deallocateTrie(struct Trie *pTrie)
{
    deallocateTrieNode(pTrie->root);
    free(pTrie);
    return NULL;
}

int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open the file\n");
        return 0;
    }

    int numWords;
    fscanf(file, "%d", &numWords);
    for (int i = 0; i < numWords; i++)
    {
        pInWords[i] = (char *)malloc(256 * sizeof(char));
        fscanf(file, "%s", pInWords[i]);
    }
    fclose(file);

    return numWords;
}

int main(void)
{
    char *inWords[256];

    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");

    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }

    return 0;
}
