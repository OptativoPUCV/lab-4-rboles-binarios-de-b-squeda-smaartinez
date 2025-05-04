#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new; 
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if (tree == NULL) return ;
    if (searchTreeMap(tree, key) != NULL) return ; 

    TreeNode *padre = NULL;
    TreeNode *aux = tree->root ;

    while (aux != NULL)
    { 
        padre = aux ;
        if (tree->lower_than(key, aux->pair->key))
            aux = aux->left; 
        else 
            aux = aux->right; 
    }
    TreeNode *nuevoNodo = createTreeNode(key, value) ; 
    if (nuevoNodo == NULL) return ;
    nuevoNodo->parent = padre ; 
    if (padre == NULL) tree->root = nuevoNodo ;
    else if (tree->lower_than(key, padre->pair->key))
        padre->left = nuevoNodo ; 
    else
        padre->right = nuevoNodo ; 
    
    tree->current = nuevoNodo ;
}

TreeNode * minimum(TreeNode * x)
{
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap * tree, TreeNode* node)
{
    //Verficamos si el nodo no existe
    if( node == NULL) return;

    //el nodo no tiene hijos
    if (node->left == NULL && node->right == NULL)
    {
        if(node->parent == NULL)
        {
            tree->root = NULL;
        }
        
        else if(node->parent->left == node)
        {
            node->parent->left = NULL;
        }
        // Aqui estamos cuando el nodo sea el hijo derecho
        else
        {
            node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
    }
    //el nodo tiene un hijo
    else if((node->left == NULL && node->right != NULL) || (node->left != NULL && node->right == NULL))
    {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        if(node->parent == node)
        {
            tree->root = child;
            child->parent = NULL;
            // la raiz ahora es el hijo 
        }
        else if(node->parent->left == node){
            node->parent->left = child;
            child->parent = node->parent;
        } else {
            node->parent->right = child;
            child->parent = node->parent;
        }
        free(node->pair);
        free(node);
    }

    // el nodo tiene dos hijos
    else
    {
        TreeNode *min = minimum(node->right); 

        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
    }
}

void eraseTreeMap(TreeMap * tree, void* key)
{
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * current = tree->root;
    while (current != NULL)
    {
        if (tree->lower_than(key, current->pair->key))
        {
            current = current->left;
        }
        
        else if (tree->lower_than(current->pair->key, key))
        {
            current = current->right;
        }

        else 
        {
            tree->current = current;
            return current->pair; 
        }
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) 
{
    if (tree->root == NULL) return NULL;
    Pair *auxiliar = searchTreeMap(tree, key);
    if (auxiliar != NULL) return auxiliar;
    TreeNode *current = tree->root;
    TreeNode *posible = NULL;
    while (current != NULL) 
    {
        if (tree->lower_than(key, current->pair->key)) 
        {
            posible = current;
            current = current->left;
        } 
        else 
        {
            current = current->right;
        }
    }
    if (posible != NULL)
    {
        tree->current = posible;
        return posible->pair;
    }
    return NULL;

}

Pair * firstTreeMap(TreeMap * tree) 
{
    if (tree->root == NULL) return NULL;
    TreeNode *minimo = minimum(tree->root);
    tree->current = minimo;
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
    if (tree->current->right != NULL)
    {
        TreeNode *siguiente = minimum(tree->current->right);
        tree->current = siguiente;
        return tree->current->pair;
    }
    else
    {
        TreeNode *auxiliar = tree->current;
        while (auxiliar->parent != NULL && auxiliar->parent->right == auxiliar)
        {
            auxiliar = auxiliar->parent;
        }
        if (auxiliar->parent == NULL) return NULL;
        tree->current = auxiliar->parent;
        return tree->current->pair;
    }
}
