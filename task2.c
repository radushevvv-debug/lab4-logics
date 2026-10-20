#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node *create_node(int value)
{
    Node *node = malloc(sizeof(*node));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    node->value = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

Node *insert_unique(Node *root, int value)
{
    if (root == NULL) {
        return create_node(value);
    }

    if (value < root->value) {
        root->left = insert_unique(root->left, value);
    } else if (value > root->value) {
        root->right = insert_unique(root->right, value);
    }

    return root;
}

void print_tree(const Node *root, int depth)
{
    if (root == NULL) {
        return;
    }

    print_tree(root->right, depth + 1);

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }

    printf("%d\n", root->value);

    print_tree(root->left, depth + 1);
}

void free_tree(Node *root)
{
    if (root == NULL) {
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void)
{
    Node *root = NULL;
    int value;

    printf("Enter integers to build the tree (-1 to finish):\n");

    while (scanf("%d", &value) == 1 && value != -1) {
        root = insert_unique(root, value);
    }

    printf("\nTree without duplicates (rotated 90 degrees):\n");
    print_tree(root, 0);

    free_tree(root);

    return 0;
}