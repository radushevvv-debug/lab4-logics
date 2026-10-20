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

Node *insert(Node *root, int value)
{
    if (root == NULL) {
        return create_node(value);
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    } else {
        root->right = insert(root->right, value);
    }

    return root;
}

const Node *find(const Node *root, int value)
{
    if (root == NULL || root->value == value) {
        return root;
    }

    if (value < root->value) {
        return find(root->left, value);
    }

    return find(root->right, value);
}

int count_occurrences(const Node *root, int value)
{
    if (root == NULL) {
        return 0;
    }

    int count = (root->value == value);

    count += count_occurrences(root->left, value);
    count += count_occurrences(root->right, value);

    return count;
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
    int target;

    printf("Enter integers to build the tree (-1 to finish):\n");

    while (scanf("%d", &value) == 1 && value != -1) {
        root = insert(root, value);
    }

    printf("\nTree (rotated 90 degrees):\n");
    print_tree(root, 0);

    printf("\nEnter number to find: ");

    if (scanf("%d", &target) != 1) {
        fprintf(stderr, "Invalid input\n");
        free_tree(root);
        return EXIT_FAILURE;
    }

    if (find(root, target) != NULL) {
        printf("Value %d found\n", target);
    } else {
        printf("Value %d not found\n", target);
    }

    printf("Occurrences of %d: %d\n",
           target, count_occurrences(root, target));

    free_tree(root);

    return 0;
}