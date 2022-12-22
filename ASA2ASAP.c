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

NodeBinary *deserialize_tree(FILE *fp);

void print_inorder(NodeBinary *root);

void print_preorder(NodeBinary *root);
void print_postorder(NodeBinary *root);

int ASA2ASAP()
{
  FILE *tree_file_ASAP = fopen("ab_tree.txt", "r");
  NodeBinary *root = deserialize_tree(tree_file_ASAP);

  printf("Polonesa Reversa: \n");
  print_preorder(root);
  printf("\n");
  printf("Polonesa: ");
  print_postorder(root);
  printf("\n");
}
