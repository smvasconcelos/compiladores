#include <stdio.h>
#include <stdlib.h>
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

// Converrts a saved file ast to an array
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

// Gets the position of a given id in the array
int get_item_pos(parse_tree_element *tree, int id)
{
  for (int index = 0; tree[index].value != '\0'; index++)
  {
    if (tree[index].id == id)
      return index;
  }
  return -1;
}

// Checks if the item is an extra char
int is_extra(char item)
{
  if (item == '(')
    return 1;
  else if (item == ')')
    return 1;
  else if (item == ';')
    return 1;
  else if (item == '{')
    return 1;
  else if (item == '}')
    return 1;
  else if (item == '.')
    return 1;
  else
    return 0;
}

//  Converts an array ast to an ast using struct
void array_ast_to_ast(parse_tree_element *tree, Node *root, int id)
{
  // puts("---");
  // printf("Analisando o char [%c] de Id: [%d];\n", tree[id].value, tree[id].id);
  // puts("---");
  int i = id;
  do
  {
    // Se for uma produção
    printf("[%c] E uma producao [%d] ID da producao\n", tree[i].value, tree[i].id);
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
      // }
    }
    else
    {
      // printf("[%c] Nao e uma producao\n", tree[i].value);
      // printf("[%c] foi inserido como filho de [%c] de id [%d]\n", tree[i].value, root->data, root->id);

      if (!is_extra(tree[i].value))
      {
        // Cria o elemento
        Node *child = create_node(tree[i].value, tree[i].id);
        // Insere o elemento
        insert_node(root, child);
      }
    }
  } while (tree[i].id + 1 == tree[++i].id);
}

NodeBinary *create_node_at()
{
  // Allocate memory for the new node
  NodeBinary *node = calloc(1, sizeof(NodeBinary));

  node->right = NULL;
  node->left = NULL;

  return node;
}

NodeBinary *get_deepest_node_right(NodeBinary *root)
{
  if (root->right == NULL)
    return root;

  return get_deepest_node_right(root->right);
}

NodeBinary *get_deepest_node_left(NodeBinary *root)
{
  if (root->left == NULL)
    return root;

  return get_deepest_node_left(root->left);
}

// Generates de Abstract Tree
NodeBinary *generate_at(Node *root)
{
  /*
   * Produções
   * p1: S → M
   * p2: S -> G M
   * p3: S -> N G M
   * p4: N → n(){ A; r(E); }
   * p5: G → g(){ A; r(E); }
   * p6: M → m(){ A; r(E); }
   * p7: A → CB
   * p8: B -> .
   * p9: B -> ;CB
   * p10: E → 0
   * p11: E -> 1
   * p12: E -> x
   * p13: E -> y
   * p14: E -> (EXE)
   * p15: X → +
   * p16: X -> -
   * p17: X -> *
   * p18: X -> /
   * p19: C → h=g()
   * p20: C -> i=n()
   * p21: C -> j=E
   * p22: C -> k=E
   * p23: C -> z=E
   * p24: C -> (EXE)
   * p25: C -> w(E){ CD
   * p26: C -> f(E){ CD
   * p27: C -> o(E; E; E){ CD
   * p28: D -> }
   * p29: D -> ;CD
   */
  if (root == NULL)
    return NULL;

  NodeBinary *at = create_node_at();

  switch (root->data)
  {
  case 'S':
    /*
     * p1: S → M
     * p2: S -> G M
     * p3: S -> N G M
     * p4: N → n(){ A; r(E); }
     * p5: G → g(){ A; r(E); }
     * p6: M → m(){ A; r(E); }
     */
    at->data = root->children[0]->data;
    at->id = root->children[0]->id;
    // printf("A raiz e [%c]\n", at->data);
    at->left = generate_at(root->children[1]);
    printf("Esquerda e [%c]\n", at->left->data);
    at->right = generate_at(root->children[2]);
    // printf("Direita e [%c]\n", at->right->data);
    // Tratamento para o próximo filho que é r e não gera nada
    // porem tem um E como filho
    at->right->right = generate_at(root->children[3]);
    // printf("Direita direita e [%c]\n", at->right->right->data);
    break;

  case 'E':
    /*
     * p7: E → 0
     * p7: E -> 1
     * p9: E -> x
     * p10: E -> y
     * p11: E -> (EXE)
     */
    if (root->num_children == 1)
    {
      at->data = root->children[0]->data;
      at->id = root->children[0]->id;
      // printf("O filho de [%c] [%d] e [%c] [%d]\n", root->data, root->id, at->data, at->id);
    }
    else
    {
      // printf("O filho de [%c] [%d] e [%c] [%d]\n", root->data, root->id, at->data, at->id);
      at->data = root->children[1]->children[0]->data;
      at->id = root->children[1]->children[0]->id;
      at->right = generate_at(root->children[2]);
      at->left = generate_at(root->children[0]);
    }
    break;

  case 'X':
    /*
     * p12: X → +
     * p13: X -> -
     * p14: X -> *
     * p15: X -> /
     */
    at->data = root->children[0]->data;
    at->id = root->children[0]->id;
    // printf("O filho de [%c] [%d] e [%c] [%d]\n", root->data, root->id, at->data, at->id);
    break;

  case 'A':
    // * p7: A → CB
    at->data = root->data;
    at->id = root->id;
    at->left = generate_at(root->children[0]);
    at->right = generate_at(root->children[1]);
    break;

  case 'B':
    /*
     * p8: B -> .
     * p9: B -> ;CB
     */
    if (root->num_children == 0)
    {
      free(at);
      return NULL;
    }
    else
    {
      at->data = root->data;
      at->id = root->data;
      at->left = generate_at(root->children[0]);
      at->right = generate_at(root->children[1]);
    }
    break;

  case 'C':

    // printf("Parent {%c} {%d} \n", root->data, root->id);
    /*
     * p19: C → h=g()
     * p20: C -> i=n()
     * p21: C -> j=E
     * p22: C -> k=E
     * p23: C -> z=E
     * p24: C -> (EXE)
     * p25: C -> w(E){ CD
     * p26: C -> f(E){ CD
     * p27: C -> o(E; E; E){ CD
     * p28: D -> }
     * p29: D -> ;CD
     */
    if (root->children[0]->data == 'f' || root->children[0]->data == 'w')
    {
      // Pai é f
      at->data = root->children[0]->data;
      at->id = root->children[0]->id;
      // Retorna a subarvore da produção E
      at->left = generate_at(root->children[1]);
      // Retorna a subarvore da produção C
      at->right = generate_at(root->children[2]);

      // NodeBinary *temp = NULL;
      // temp = get_deepest_node_right(at);
      at->right->right->right = generate_at(root->children[3]);
      //   printf("O filho de [%c] [%d] e temp e [%c] [%d]\n", root->children[3]->data, root->children[3]->id, temp->data, temp->id);
      //   printf("Right e [%c] [%d]\n", temp->right->data, temp->right->id);
    }
    else if (root->children[0]->data == 'o')
    {
      // Pai é o
      at->data = root->children[0]->data;
      at->id = root->children[0]->id;
      // Retorna a subarvore da produção E1
      at->left = generate_at(root->children[1]);
      // Retorna a subarvore da produção E2
      at->right = generate_at(root->children[2]);
      // Retorna a subarvore da produção E3
      at->right->left = generate_at(root->children[3]);
      // Retorna a subarvore da produção C
      at->right->right = generate_at(root->children[4]);
      at->right->right->right = generate_at(root->children[4]);
    }
    else if (root->children[0]->data == 'h' || root->children[0]->data == 'i')
    {
      // Pai é =
      at->data = root->children[1]->data;
      at->id = root->children[1]->id;
      // Filho é h i
      at->left = create_node_at();
      at->left->data = root->children[0]->data;
      at->left->id = root->children[0]->id;
      // Filho é g n
      at->right = create_node_at();
      at->right->data = root->children[2]->data;
      at->right->id = root->children[2]->id;
    }
    else if (root->children[1]->data == '=')
    {
      // Pai é =
      at->data = root->children[1]->data;
      at->id = root->children[1]->id;
      // Filho é j k z
      at->left = create_node_at();
      at->left->data = root->children[0]->data;
      at->left->id = root->children[0]->id;
      // Retorna a subarvore da produção E
      at->right = generate_at(root->children[2]);
    }
    else if (root->children[1]->data == 'X')
    {
      at->data = root->children[1]->children[0]->data;
      at->id = root->children[1]->children[0]->id;
      at->right = generate_at(root->children[2]);
      at->left = generate_at(root->children[0]);
    }
    break;

  case 'D':
    if (root->num_children == 0)
    {
      free(at);
      return NULL;
    }
    else
    {
      // Pai é D
      at->data = root->data;
      at->id = root->id;
      // printf("O filho de [%c] [%d] e root e [%c] [%d]\n", root->children[0]->data, root->children[0]->id, root->data, root->id);
      at->left = generate_at(root->children[0]);
      at->right = generate_at(root->children[1]);
    }
    break;

  case 'r':
    at->data = root->data;
    at->id = root->id;
    break;

  default:
    break;
  }
  if (!at->data)
  {
    free(at);
    return NULL;
  }
  return at;
}

void print_inorder(NodeBinary *root)
{
  if (root == NULL)
    return;
  print_inorder(root->left);
  printf("Data [%c] \t Id [%d] \n", root->data, root->id);
  print_inorder(root->right);
}

void print_preorder(NodeBinary *root)
{
  if (root == NULL)
    return;
  printf("Data [%c] \t Id [%d] \n", root->data, root->id);
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
  printf("Data [%c] \t Id [%d] \n", root->data, root->id);
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
  print_nary_tree(root);
  puts("Okay");
  NodeBinary *at = generate_at(root);
  puts("Done");
  puts("Inorder");
  print_inorder(at);
  puts("Posorder");
  print_postorder(at);
  puts("preorder");
  print_preorder(at);
}
