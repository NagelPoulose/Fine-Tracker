#include <stdio.h>
#include <stdlib.h>
#include <string.h>

This program is written by: Nagel Poulose */

// node structure for the BST
typedef struct Node {
  char name[26];
  int fine;
  struct Node *left;
  struct Node *right;
} Node;

// function that takes a name and fine and creates a new node
Node *createNode(char *name, int fine) {
  Node *node = (Node *)malloc(sizeof(Node));
  strcpy(node->name, name);
  node->fine = fine;
  node->left = node->right = NULL;
  return node;
}

// function that inserts a new owner or update an existing owner's fine
Node *insert(Node *node, char *name, int fine, int depth, int *insertedDepth) {
  if (node == NULL) { 
    *insertedDepth = depth;
    return createNode(name, fine);
  }
  // these lines compare the names lexicographically
  int cmp = strcmp(name, node->name); 
  if (cmp == 0) {                     
    node->fine += fine;               
    *insertedDepth = depth;           
  } else if (cmp < 0) {               
    node->left = insert(node->left, name, fine, depth + 1, insertedDepth);
  } else { 
    node->right = insert(node->right, name, fine, depth + 1, insertedDepth);
  }
  return node; 
}

// function that looks for the owner
Node *search(Node *node, char *name, int depth, int *foundDepth) {
  if (node == NULL) 
    return NULL;
  int cmp = strcmp(name, node->name); 
  if (cmp == 0) {                     
    *foundDepth = depth;              
    return node;

    //depending on if the name is less or more it will search in the left or right 
  } else if (cmp < 0) {               
    return search(node->left, name, depth + 1, foundDepth);
  } else { 
    return search(node->right, name, depth + 1, foundDepth);
  }
}

// function that finds the maximum node
Node *findMax(Node *node, int *depth) {
  while (node->right != NULL) { 
    node = node->right;
    (*depth)++; 
  }
  return node; 
}

// delete function that gets rid of an owner
Node *deleteNode(Node *node, char *name, int *found) {
  if (node == NULL) { 
    return NULL;
  }
  int cmp = strcmp(name, node->name); 
  if (cmp < 0) {                      
    node->left = deleteNode(node->left, name, found);
  } else if (cmp > 0) { 
    node->right = deleteNode(node->right, name, found);
  } else {      
    *found = 1; 
    // cosiders if the node has any no children  
    if (node->left == NULL && node->right == NULL) {
      free(node);  
      node = NULL; 
    }
    // considers if node has only right child 
    else if (node->left == NULL) {
      Node *temp = node;  
      node = node->right; 
      free(temp);         
    }
    // considers if node has only left child 
    else if (node->right == NULL) {
      Node *temp = node; 
      node = node->left; 
      free(temp);        
    }
    // considers if node has two children 
    else {
      int tempDepth = 0;
      Node *temp =
          findMax(node->left, &tempDepth); 
      strcpy(node->name, temp->name); 
      node->fine = temp->fine;        
      node->left = deleteNode(node->left, temp->name, found); 
    }
  }
  return node; 
}

// function calculates the height of the subtrees
int height(Node *node) {
  if (node == NULL)
    return -1;                    
  int lh = height(node->left);    
  int rh = height(node->right);   
  return (lh > rh ? lh : rh) + 1; 
}

// function adds together all the fines
void sumFines(Node *node, long long *total) {
  if (node == NULL)
    return;
  *total += node->fine;         
  sumFines(node->left, total);  
  sumFines(node->right, total); 
}

// function counts all the nodes
void countNodes(Node *node, int *count) {
  if (node == NULL)
    return;
  (*count)++;                     
  countNodes(node->left, count);  
  countNodes(node->right, count); 
}

// function to calculate  fines for owners with names less than or equal to the given name 
void calcBelow(Node *node, char *name, long long *total) {
  if (node == NULL)
    return;
  int cmp = strcmp(name, node->name); 
  if (cmp < 0) {                      
    calcBelow(node->left, name, total);
  } else {                               
    calcBelow(node->left, name, total);  
    *total += node->fine;                
    calcBelow(node->right, name, total); 
  }
}

// function that frees all nodes
void freeTree(Node *node) {
  if (node == NULL)
    return;
  freeTree(node->left);  
  freeTree(node->right); 
  free(node);            
}

int main() {
  int n;
  
  // reads in the amount of commands 
  scanf("%d", &n);   
  Node *root = NULL; 
  for (int i = 0; i < n; i++) {
    char cmd[15], name[26];
    int fine;
    scanf("%s", cmd); 
    
    //runs the add command 
    if (strcmp(cmd, "add") == 0) { 
      scanf("%s %d", name, &fine); 
      int depth = -1;
      Node *node = search(root, name, 0, &depth);      
      if (node != NULL) {                              
        node->fine += fine;                            
        printf("%s %d %d\n", name, node->fine, depth); 
      } else {
        int insertedDepth = -1;

        // calls insert function to insert new owner
        root = insert(root, name, fine, 0, &insertedDepth); 
        printf("%s %d %d\n", name, fine, insertedDepth); 
      }
      
      //runs the deduct command
    } else if (strcmp(cmd, "deduct") == 0) { 
      scanf("%s %d", name, &fine);           
      int depth = -1;
      Node *node = search(root, name, 0, &depth); 
      if (node == NULL) {                         
        printf("%s not found\n", name);
        continue;
      }

      //handles deleting the owner from the tree cause they no longer have fine
      node->fine -= fine;    
      if (node->fine <= 0) { 
        int found = 0;
        root = deleteNode(root, name, &found); 
        printf("%s removed\n", name);          
      } else {
        printf("%s %d %d\n", name, node->fine, depth); 
      }

      //runs the search command 
    } else if (strcmp(cmd, "search") == 0) { 
      scanf("%s", name);                     
      int depth = -1;
      Node *node = search(root, name, 0, &depth); 
      if (node == NULL) {                         
        printf("%s not found\n", name);
      } else {
        printf("%s %d %d\n", name, node->fine, depth); 
      }

      //runs the average command 
    } else if (strcmp(cmd, "average") == 0) { 
      if (root == NULL) {
        printf("0.00\n"); 
      } else {
        long long total = 0;
        int count = 0;
        sumFines(root, &total);             
        countNodes(root, &count);           
        double avg = (double)total / count; 
        printf("%.2f\n", avg); 
      }
      
      //runs the height balance command first getting the height of left and compares
    } else if (strcmp(cmd, "height_balance") == 0) { 
      int lh = height(root ? root->left : NULL);  
      int rh = height(root ? root->right : NULL); 
      
      // prints the heights and states whether or not the tree is balanced
      printf("left height = %d right height = %d %s\n", lh, rh,
             lh == rh ? "balanced" : "not balanced");
    } else if (strcmp(cmd, "calc_below") == 0) { 
      scanf("%s", name);                         
      long long total = 0;
      calcBelow(root, name,
                &total);       
      printf("%lld\n", total); 
    }
  }
  freeTree(root); 
  return 0;
}
