#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "TAD.h"

List *createList(char word[], int frequency, int symbol, char code[])
{
    List *newList = (List *)malloc(sizeof(List));

    newList->next = NULL;
    strcpy(newList->info.code, code);
    newList->info.frequency = frequency;
    strcpy(newList->info.word, strdup(word));
    newList->info.symbol = symbol;

    return newList;
}

Node *createNode(int symbol, int frequency)
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->symbol = symbol;
    node->frequency = frequency;
    node->left = node->right = NULL;

    return node;
}

void buildHuffmanTree(Info info, Node **node)
{
    Node *auxNode = *node;
    int i = 0;
    while (info.code[i] != '\0')
    {
        if (info.code[i] == '0')
        {
            if (auxNode->left == NULL)
            {
                auxNode->left = createNode(-1, 0);
            }
            auxNode = auxNode->left;
        }
        else if (info.code[i] == '1')
        {
            if (auxNode->right == NULL)
            {
                auxNode->right = createNode(-1, 0);
            }
            auxNode = auxNode->right;
        }
        i++;
    }
    auxNode->symbol = info.symbol;
}

List *readBinaryFile(char *filename, Node **node)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    List *list = NULL;
    Info info;

    fread(&info, sizeof(Info), 1, file);
    while (!feof(file))
    {
        List *newNode = createList(info.word, info.frequency, info.symbol, strdup(info.code));
        
        buildHuffmanTree(info, &(*node));
        newNode->next = list;
        list = newNode;

        fread(&info, sizeof(Info), 1, file);
    }

    fclose(file);
    return list;
}

void findWordOnList(List *list, int symbol)
{
    int find = 0;
    while (list != NULL && find == 0)
    {
        if (list->info.symbol == symbol)
        {
            printf("%s", list->info.word);
        }
        list = list->next;
    }
}

void decode(List *list, Node *node)
{
    FILE *ptrCode = fopen("./files/code.txt", "rb");
    char code;

    if (ptrCode != NULL)
    {
        int i = 0;
        Node *auxNode = node;

        while (!feof(ptrCode))
        {
            code = fgetc(ptrCode);
            if (code == '0')
            {
                auxNode = auxNode->left;
            }
            else if (code == '1')
            {
                auxNode = auxNode->right;
            }

            if (auxNode->right == NULL && auxNode->left == NULL)
            {
                findWordOnList(list, auxNode->symbol);
                auxNode = node;
            }
        }
    }
}

int main()
{
    Node *node = createNode(-1, 0);
    List *readList = readBinaryFile("./files/output.dat", &node);

    char code[256];

    decode(readList, node);

    return 0;
}
