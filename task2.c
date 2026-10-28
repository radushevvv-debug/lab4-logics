#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LABEL_LENGTH 64
#define BRANCH_GAP 3

typedef struct Node {
    int value;
    int excluded_count;
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
    node->excluded_count = 0;
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
    } else {
        root->excluded_count++;
    }

    return root;
}

int get_height(const Node *root)
{
    if (root == NULL) {
        return 0;
    }

    int left_height = get_height(root->left);
    int right_height = get_height(root->right);

    return 1 + (left_height > right_height
                ? left_height : right_height);
}

int get_width(const Node *root, int level)
{
    int label_width = snprintf(NULL, 0, "%d (%d)",
                               root->value, level);

    if (root->left == NULL && root->right == NULL) {
        return label_width;
    }

    int left_width = root->left != NULL
        ? get_width(root->left, level + 1) : 1;

    int right_width = root->right != NULL
        ? get_width(root->right, level + 1) : 1;

    int children_width =
        left_width + BRANCH_GAP + right_width;

    return label_width > children_width
        ? label_width : children_width;
}

void draw_tree(const Node *root, int level, int start,
               int width, char **picture)
{
    char label[LABEL_LENGTH];

    int length = snprintf(label, sizeof(label),
                          "%d (%d)", root->value, level);

    int center = start + width / 2;

    memcpy(picture[2 * level] + center - length / 2,
           label, (size_t)length);

    if (root->left == NULL && root->right == NULL) {
        return;
    }

    int left_width = root->left != NULL
        ? get_width(root->left, level + 1) : 1;

    int right_width = root->right != NULL
        ? get_width(root->right, level + 1) : 1;

    int children_width =
        left_width + BRANCH_GAP + right_width;

    int left_start =
        start + (width - children_width) / 2;

    int right_start =
        left_start + left_width + BRANCH_GAP;

    if (root->left != NULL) {
        int child_center = left_start + left_width / 2;

        picture[2 * level + 1]
               [(center + child_center) / 2] = '/';

        draw_tree(root->left, level + 1,
                  left_start, left_width, picture);
    }

    if (root->right != NULL) {
        int child_center = right_start + right_width / 2;

        picture[2 * level + 1]
               [(center + child_center) / 2] = '\\';

        draw_tree(root->right, level + 1,
                  right_start, right_width, picture);
    }
}

void print_tree(const Node *root)
{
    if (root == NULL) {
        printf("Tree is empty\n");
        return;
    }

    int rows = 2 * get_height(root) - 1;
    int width = get_width(root, 0);

    char **picture =
        malloc((size_t)rows * sizeof(*picture));

    if (picture == NULL) {
        fprintf(stderr,
                "Memory allocation error while printing\n");
        return;
    }

    for (int row = 0; row < rows; row++) {
        picture[row] = malloc((size_t)width + 1);

        if (picture[row] == NULL) {
            for (int i = 0; i < row; i++) {
                free(picture[i]);
            }

            free(picture);

            fprintf(stderr,
                    "Memory allocation error while printing\n");
            return;
        }

        memset(picture[row], ' ', (size_t)width);
        picture[row][width] = '\0';
    }

    draw_tree(root, 0, 0, width, picture);

    for (int row = 0; row < rows; row++) {
        int end = width;

        while (end > 0 && picture[row][end - 1] == ' ') {
            end--;
        }

        picture[row][end] = '\0';

        puts(picture[row]);
        free(picture[row]);
    }

    free(picture);
}

int print_excluded(const Node *root)
{
    if (root == NULL) {
        return 0;
    }

    int repeated_values = print_excluded(root->left);

    if (root->excluded_count > 0) {
        printf("%d: excluded %d extra occurrence(s)\n",
               root->value, root->excluded_count);

        repeated_values++;
    }

    repeated_values += print_excluded(root->right);

    return repeated_values;
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

    printf("Enter integers to build the tree "
           "(-1 to finish):\n");

    while (scanf("%d", &value) == 1 && value != -1) {
        root = insert_unique(root, value);
    }

    printf("\nTree without duplicates: value (level)\n");
    print_tree(root);

    printf("\nExcluded duplicates:\n");

    if (print_excluded(root) == 0) {
        printf("No duplicate numbers were entered\n");
    }

    free_tree(root);

    return 0;
}