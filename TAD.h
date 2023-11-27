struct node
{
    int frequency, symbol;
    struct node *left, *right;
};
typedef struct node Node;

struct listNode
{
    Node *treeNode;
    struct listNode *next;
};
typedef struct listNode ListNode;

struct info
{
    char word[100], code[100];
    int frequency, symbol;
};
typedef struct info Info;

struct list
{
    Info info;
    struct list *next;
};
typedef struct list List;