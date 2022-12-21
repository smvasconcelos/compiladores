#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct NodeBinary
{
  // The data stored in the node
  char data;
  // The id stored in the node
  int id;
  // An array of pointers to the child nodes
  struct NodeBinary *right;
  struct NodeBinary *left;

} NodeBinary;

NodeBinary *deserialize_tree(FILE *fp)
{
  char data;
  int id;
  int itemsRead = fscanf(fp, "%c %d ", &data, &id);

  if (itemsRead != 2)
  {
    return NULL;
  }

  NodeBinary *node = malloc(sizeof(NodeBinary));
  node->data = data;
  node->id = id;
  node->left = NULL;
  node->right = NULL;

  char c;
  itemsRead = fscanf(fp, " %c", &c);
  if (itemsRead == 1 && c == 'L')
  {
    node->left = deserialize_tree(fp);
  }

  itemsRead = fscanf(fp, " %c", &c);
  if (itemsRead == 1 && c == 'R')
  {
    node->right = deserialize_tree(fp);
  }

  return node;
}

void print_inorder(NodeBinary *root)
{
  if (root == NULL)
    return;
  print_inorder(root->left);
  printf("%c", root->data);
  print_inorder(root->right);
}

void print_preorder(NodeBinary *root)
{
  if (root == NULL)
    return;
  printf("%c", root->data);
  print_preorder(root->left);
  print_preorder(root->right);
}

void print_postorder(NodeBinary *root)
{
  if (root == NULL)
  {
    return;
  }
  print_postorder(root->left);
  print_postorder(root->right);
  printf("%c", root->data);
}

int main()
{
  FILE *tree_file = fopen("ab_tree.txt", "r");
  NodeBinary *root = deserialize_tree(tree_file);

  printf("Polonesa Reversa: ");
  print_preorder(root);
  printf("\n");
  printf("Polonesa: ");
  print_postorder(root);
  printf("\n");
}
