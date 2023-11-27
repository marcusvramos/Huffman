#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "TAD.h"

Node *createNode(int symbol, int frequency)
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->symbol = symbol;
    node->frequency = frequency;
    node->left = node->right = NULL;

    return node;
}

ListNode *createListNodeByList(List *list)
{
    ListNode *listNode = (ListNode *)malloc(sizeof(ListNode));
    listNode->treeNode = (Node *)malloc(sizeof(Node));
    listNode->treeNode = createNode(list->info.symbol, list->info.frequency);
    listNode->next = NULL;

    return listNode;
}

ListNode *createListNodeByNode(Node *node)
{
    ListNode *listNode = (ListNode *)malloc(sizeof(ListNode));
    listNode->treeNode = node;
    listNode->next = NULL;

    return listNode;
}

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

void writeListToBinaryFile(List *list, char *filename)
{
    FILE *file = fopen(filename, "wb");
    List *aux = list;
    Info auxInfo;

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
    while (aux != NULL)
    {
        auxInfo = aux->info;
        fwrite(&auxInfo, sizeof(Info), 1, file);
        aux = aux->next;
    }

    fclose(file);
}

void writeTableToTextFile(List *list, char filename[])
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    while (list != NULL)
    {
        fprintf(file, "Symbol: %-5d | Word: %-14s | Frequency: %-6d | Code: %-10s\n", list->info.symbol, list->info.word, list->info.frequency, list->info.code);

        list = list->next;
    }

    fclose(file);
}

void writeListToTextFile(char *code, char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s", code);

    fclose(file);
}

void buildHuffmanForest(ListNode **listNode, List *list)
{
    while (list != NULL)
    {
        ListNode *newListNode = createListNodeByList(list); /* CRIANDO O NÓ PARA INSERIR NA LISTA ENCADEADA*/

        // ! VERIFICANDO SE:
        // * A LISTA ESTÁ VAZIA
        // ! OU
        // * SE A FREQUENCIA DO NÓ CRIADO É MENOR DO QUE O PRIMEIRO NÓ EXISTENTE DA LISTA ENCADEADA

        if (*listNode == NULL)
        {
            *listNode = newListNode;
        }
        else
        {
            ListNode *current = *listNode; // * PEGANDO POSIÇÃO PARA PERCORRER A LISTA E ENCONTRAR A POSIÇÃO CORRETA

            // * ENQUANTO A FREQUENCIA DO NÓ ATUAL DA LISTA FOR MENOR QUE O NÓ INSERIDO, EU PERCORRO A PRÓXIMA POSIÇÃO
            while (current->next != NULL && current->next->treeNode->frequency <= list->info.frequency)
            {
                current = current->next;
            }

            // * INSERINDO O NÓ CRIADO NA POSIÇÃO CORRETA
            newListNode->next = current->next;
            current->next = newListNode;
        }
        list = list->next;
    }
}

void buildHuffmanTree(ListNode **listNode, Node *node)
{
    ListNode *newListNode = createListNodeByNode(node); /* CRIANDO O NÓ PARA INSERIR NA LISTA ENCADEADA*/

    // ! VERIFICANDO SE:
    // * A LISTA ESTÁ VAZIA
    // ! OU
    // * SE A FREQUENCIA DO NÓ CRIADO É MENOR DO QUE O PRIMEIRO NÓ EXISTENTE DA LISTA ENCADEADA

    ListNode *listAux = *listNode;

    if (*listNode == NULL)
    {
        newListNode->next = *listNode; // * NO CRIADO APONTA PARA O NÓ EXISTENTE. SE EXISTIR APONTA PRO EXISTENTE, SE NÃO APONTA PRA NULL
        *listNode = newListNode;
    }
    else
    {
        ListNode *current = *listNode; // * PEGANDO POSIÇÃO PARA PERCORRER A LISTA E ENCONTRAR A POSIÇÃO CORRETA

        // * ENQUANTO A FREQUENCIA DO NÓ ATUAL DA LISTA FOR MENOR QUE O NÓ INSERIDO, EU PERCORRO A PRÓXIMA POSIÇÃO
        while (current->next != NULL && current->next->treeNode->frequency <= node->frequency)
        {
            current = current->next;
        }

        // * INSERINDO O NÓ CRIADO NA POSIÇÃO CORRETA
        newListNode->next = current->next;
        current->next = newListNode;
    }
}

Node *buildHuffmanTreeByList(ListNode **listNode)
{
    while (*listNode != NULL && (*listNode)->next != NULL)
    {
        Node *nodeLeft = (*listNode)->treeNode;
        Node *nodeRight = (*listNode)->next->treeNode;

        Node *newNode = createNode(-1, nodeLeft->frequency + nodeRight->frequency);

        newNode->left = nodeLeft;
        newNode->right = nodeRight;

        ListNode *temp = *listNode;
        *listNode = (*listNode)->next;
        free(temp);

        temp = *listNode;
        *listNode = (*listNode)->next;
        free(temp);

        buildHuffmanTree(&(*listNode), newNode);
    }

    return (*listNode)->treeNode;
}

void buildList(List **list, char word[])
{
    int symbol = 0;
    int find = 0;
    if (*list == NULL)
    {
        *list = createList(word, 1, symbol, "");
    }
    else
    {
        List *current = *list;
        symbol++;

        while (current->next != NULL && find == 0)
        {
            if (strcmp(current->info.word, word) == 0)
            {
                find = 1;
            }
            else
            {
                current = current->next;
                symbol++;
            }
        }
        if (find == 0 && current->next == NULL)
        {
            current->next = createList(word, 1, symbol, "");
        }
        else
        {
            current->info.frequency++;
        }
    }
}

Node *findNodeAndCodeInHuffmanTree(Node *node, int symbol, char code[], int index)
{
    if (node != NULL)
    {
        if (node->symbol == symbol)
        {
            code[index] = '\0';
            return node;
        }

        Node *foundLeft = findNodeAndCodeInHuffmanTree(node->left, symbol, code, index + 1);
        if (foundLeft != NULL)
        {
            code[index] = '0';
            return foundLeft;
        }

        Node *foundRight = findNodeAndCodeInHuffmanTree(node->right, symbol, code, index + 1);
        if (foundRight != NULL)
        {
            code[index] = '1';
            return foundRight;
        }
    }
    return NULL;
}

char *findCodeForSymbol(Node *root, int symbol)
{
    char code[256];
    int index = 0;

    Node *foundNode = findNodeAndCodeInHuffmanTree(root, symbol, code, index);

    if (foundNode != NULL)
    {
        return strdup(code);
    }
    else
    {
        return NULL;
    }
}

void addCodeByHuffmanTree(List **list, Node *node, char *code, int index)
{
    List *auxList = *list;
    while ((auxList) != NULL)
    {
        strcpy(auxList->info.code, findCodeForSymbol(node, auxList->info.symbol));
        (auxList) = (auxList)->next;
    }
}

void findWordOnList(List *list, char word[], char code[])
{
    while (list != NULL && strcmp(list->info.word, word) != 0)
    {
        list = list->next;
    }

    if (list != NULL)
    {
        strcpy(code, list->info.code);
    }
}

void splitAndInsert(List **list, char input[])
{
    char *delimiters = " \t\n,.?!";
    int inputLength = strlen(input);

    int start = 0;
    int end = 0;
    int spaceQuantity = 0;

    while (end < inputLength)
    {
        while (end < inputLength && strchr(delimiters, input[end]) != NULL)
        {
            end++;
        }

        start = end;

        while (end < inputLength && strchr(delimiters, input[end]) == NULL)
        {
            end++;
        }

        if (start < end)
        {
            int wordLength = end - start;
            char *word = (char *)malloc(wordLength + 1);
            strncpy(word, input + start, wordLength);

            for (int i = 0; i < wordLength; i++)
            {
                if (word[i] >= 97 && word[i] <= 122)
                {
                    word[i] = word[i] - 32;
                }
            }

            word[wordLength] = '\0';
            buildList(&(*list), word);
            if (start != inputLength)
            {
                buildList(&(*list), " ");
            }
        }
    }
}

void splitAndInsertFile(List *list, Node *node, char input[])
{
    char *delimiters = " \t\n,.?!";
    char binaryCode[1000];
    int inputLength = strlen(input);
    char code[256] = "";

    int start = 0;
    int end = 0;
    Node *auxNode;

    while (end < inputLength)
    {
        while (end < inputLength && strchr(delimiters, input[end]) != NULL)
        {
            end++;
        }

        start = end;

        while (end < inputLength && strchr(delimiters, input[end]) == NULL)
        {
            end++;
        }

        if (start < end)
        {
            int wordLength = end - start;
            char *word = (char *)malloc(wordLength + 1);
            strncpy(word, input + start, wordLength);

            for (int i = 0; i < wordLength; i++)
            {
                if (word[i] >= 97 && word[i] <= 122)
                {
                    word[i] = word[i] - 32;
                }
            }

            word[wordLength] = '\0';

            code[0] = '\0';

            findWordOnList(list, word, code);

            strncat(binaryCode, code, sizeof(binaryCode) - strlen(binaryCode) - 1);
            strncat(binaryCode, "0", sizeof(binaryCode) - strlen(binaryCode) - 1);
        }
    }
    writeListToTextFile(binaryCode, "./files/code.txt");
}

void showList(List *list)
{
    while (list != NULL)
    {
        printf("Symbol: %-5d | Word: %-15s | Frequency: %-5d | Code: %-10s\n", list->info.symbol, list->info.word, list->info.frequency, list->info.code);
        list = list->next;
    }
}

void showHuffmanTree(Node *node, char code[], int index)
{
    if (node->left == NULL && node->right == NULL)
    {
        code[index] = '\0'; // * Adiciona o caractere nulo para indicar o final do código
        printf("Symbol: %-5d\t - Code: %s\n", node->symbol, code);
    }
    if (node->left != NULL)
    {
        code[index] = '0';
        showHuffmanTree(node->left, code, index + 1);
    }

    if (node->right != NULL)
    {
        code[index] = '1';
        showHuffmanTree(node->right, code, index + 1);
    }
}

void loadingEffect(char message[])
{
    printf("%s", message);
    for (int i = 0; i < 5; ++i)
    {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
}

int main()
{
    char firstPhrase[1000];
    char secondPhrase[1000];
    char code[256];

    ListNode *listNode = NULL;
    List *list = NULL;

    FILE *phraseOne = fopen("./files/text1.txt", "r+");
    FILE *phraseTwo = fopen("./files/text2.txt", "r+");

    fgets(firstPhrase, 1000, phraseOne);
    fgets(secondPhrase, 1000, phraseTwo);

    splitAndInsert(&list, firstPhrase);

    loadingEffect("Construindo floresta");

    buildHuffmanForest(&listNode, list);

    printf("Floresta construída\n" );

    sleep(3);

    loadingEffect("Construindo árvore de Huffman");

    Node *huffmanTree = buildHuffmanTreeByList(&listNode);

    printf("Árvore construída\n");

    sleep(3);

    addCodeByHuffmanTree(&list, huffmanTree, code, 0);

    splitAndInsertFile(list, huffmanTree, secondPhrase);

    loadingEffect("Gerando arquivos");

    writeListToBinaryFile(list, "./files/output.dat");
    writeTableToTextFile(list, "./files/output.txt");

    printf("Arquivos gerados na pasta 'files'\n");

    sleep(2);

    printf("========================================================\n");
    printf("output.txt -> arquivo da tabela de consulta da árvore de Huffman\n");
    printf("code.txt -> arquivo da codificação da segunda frase\n");
    printf("output.dat -> arquivo binário com as informações da lista com os valores\n");
    printf("text1.txt -> arquivo da frase 1\n");
    printf("text2.txt -> arquivo da frase 2\n");
    printf("========================================================\n");

    return 0;
}