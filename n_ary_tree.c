#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Define the maximum number of children that a node can have
#define MAX_CHILDREN 5

typedef struct parse_tree
{
  int id;
  char value;
} parse_tree_element;

// Define the n-ary tree node structure
typedef struct Node
{
  // The data stored in the node
  char data;

  // The id stored in the node
  int id;

  // An array of pointers to the child nodes
  struct Node *children[MAX_CHILDREN];

  // The number of children that the node has
  int num_children;
} Node;

// Create a new n-ary tree node and return a pointer to it
Node *create_node(char data, int id)
{
  // Allocate memory for the new node
  Node *node = malloc(sizeof(Node));

  // Set the data and initial number of children to 0
  node->data = data;
  node->id = id;
  node->num_children = 0;

  // Return a pointer to the new node
  return node;
}

// Insert a new node as a child of the given parent node
void insert_node(Node *parent, Node *child)
{
  // Check if the parent node has room for more children
  if (parent->num_children < MAX_CHILDREN)
  {
    // Add the child to the parent's list of children and increment the number of children
    parent->children[parent->num_children] = child;
    parent->num_children++;
  }
}

// Search for a given value in the tree
Node *search(Node *root, int id)
{
  // Check if the root is NULL
  if (!root)
  {
    return NULL;
  }

  // Check if the value is in the root
  if (root->id == id)
  {
    return root;
  }

  // Recursively search the child nodes for the id
  for (int i = 0; i < root->num_children; i++)
  {
    if (search(root->children[i], id))
    {
      return root;
    }
  }

  // If we reach here, the value was not found in the tree
  return NULL;
}

// Define a function that generates the RPN of an n-ary tree
void generate_rpn(Node *node)
{
  // Check if the node is NULL
  if (!node)
  {
    return;
  }

  // Output the data of the current node
  printf("%c ", node->data);

  // Generate the RPNs of the node's children
  for (int i = 0; i < node->num_children; i++)
  {
    generate_rpn(node->children[i]);
  }
}

void file_to_ast_array(parse_tree_element *tree)
{
  int id;
  char code;
  int i = 0;
  FILE *f = fopen("tree.txt", "r");

  while (1)
  {
    int stop = fscanf(f, "%c,%d\n", &code, &id);
    if (stop == EOF)
      break;
    tree[i].value = code;
    tree[i].id = id;
    i++;
  }
}

int array_ast_to_ast(parse_tree_element *tree, Node *root)
{
  int first_children;
  for (int i = 1; tree[i].value != '\0'; i++)
  {
    printf("Code : %c ; Id: %d;\n", tree[i].value, tree[i].id);
    if (isupper(tree[i].value))
    {
      first_children = get_item_pos(tree, 12 * i + 1);
    }
  }
}

int get_item_pos(parse_tree_element *tree, int id)
{
  for (int index = 0; tree[index].value != '\0'; index++)
  {
    if (tree[index].id == id)
      return index;
  }
  return -1;
}

int main()
{
  parse_tree_element *tree = (parse_tree_element *)calloc(1024, sizeof(parse_tree_element));
  file_to_ast_array(tree);
  for (int i = 0; tree[i].value != '\0'; i++)
  {
    printf("Code : %c ; Id: %d;\n", tree[i].value, tree[i].id);
  }

  // // Create the root node
  Node *root = create_node('S', 0);
  array_ast_to_ast(root, tree);
}
