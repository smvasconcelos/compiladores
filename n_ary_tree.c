#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Define the maximum number of children that a node can have
#define MAX_CHILDREN 12

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

  // Pos order validator
  int posorder;
} Node;

// Recursive function to print all values in an n-ary tree
void print_nary_tree(Node *root)
{
  // Print the data of the current node
  printf("Data: %c, ID: %d\n", root->data, root->id);

  // Iterate over the children of the current node
  for (int i = 0; i < root->num_children; i++)
  {
    // Recursively call the function for each child node
    print_nary_tree(root->children[i]);
  }
}

// Create a new n-ary tree node and return a pointer to it
Node *create_node(char data, int id)
{
  // Allocate memory for the new node
  Node *node = malloc(sizeof(Node));

  // Set the data and initial number of children to 0
  node->data = data;
  node->id = id;
  node->posorder = 0;
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

int get_item_pos(parse_tree_element *tree, int id)
{
  for (int index = 0; tree[index].value != '\0'; index++)
  {
    if (tree[index].id == id)
      return index;
  }
  return -1;
}

void array_ast_to_ast(parse_tree_element *tree, Node *root, int id)
{
  // puts("---");
  // printf("Analisando o char [%c] de Id: [%d];\n", tree[id].value, tree[id].id);
  // puts("---");
  int i = id;
  do
  {
    // Se for uma produção
    if (isupper(tree[i].value))
    {
      // Cria o elemento pai
      Node *newElement = create_node(tree[i].value, tree[i].id);
      // Adiciona ele na arvore
      insert_node(root, newElement);
      // Pega o id do primeiro filho
      int first_child = get_item_pos(tree, 12 * tree[i].id + 1);
      // Se tiver pelo menos um filho repete o processo pros filhos
      if (first_child >= 0)
      {
        // printf("[%c] E uma producao e ", tree[i].value);
        // printf("[%c] Tem filho na posicao [%d] do array\n", tree[i].value, first_child);
        // Gera a arvore pros filhos da produção gerada
        array_ast_to_ast(tree, newElement, first_child);
      }
      // else
      // {
      // printf("[%c] E uma producao\n", tree[i].value);
      // }
    }
    else
    {
      // printf("[%c] Nao e uma producao\n", tree[i].value);
      // printf("[%c] foi inserido como filho de [%c] de id [%d]\n", tree[i].value, root->data, root->id);
      // Cria o elemento
      Node *child = create_node(tree[i].value, tree[i].id);
      Node *clone = create_node(tree[i].value, tree[i].id);
      clone->posorder = 1;
      // Insere o elemento
      insert_node(root, child);
      // Clone para pós ordem
      insert_node(child, clone);
    }
  } while (tree[i].id + 1 == tree[++i].id);
}

int is_extra(char item)
{
  if (item == '(')
    return 1;
  else if (item == ')')
    return 1;
  else if (item == ';')
    return 1;
  else if (item == '=')
    return 1;
  else if (item == 'r')
    return 1;
  else if (item == '{')
    return 1;
  else if (item == '}')
    return 1;
  else
    return 0;
}
// percorre a arvore em pós ordem
void postorder(Node *root)
{
  if (root == NULL)
  {
    return;
  }

  for (int i = 0; i < root->num_children; i++)
  {
    postorder(root->children[i]);
  }

  if ((!isupper(root->data) || !is_extra(root->data)) && root->posorder)
    printf("%c ", root->data);
}

int main()
{
  parse_tree_element *tree = (parse_tree_element *)calloc(1024, sizeof(parse_tree_element));
  file_to_ast_array(tree);

  // Create the root node
  Node *root = create_node('S', 0);
  // Transforma a arvore em array para arvore normal
  array_ast_to_ast(tree, root, 1);
  // Printa arvore
  // print_nary_tree(root);
  postorder(root);
  // char element = '/';
  // Node *result = search(root, element);
  // if (result)
  // {
  //   printf("Found element: [%c] with Id: [%d]", element, result->id);
  // }
}
