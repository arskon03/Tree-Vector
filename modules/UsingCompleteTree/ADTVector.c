#include "ADTVector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define LEFT    0
#define RIGHT   1
#define ROOT    2

struct vector_node {
    Pointer value;
    VectorNode left;
    VectorNode right;
    int pos;
};

struct vector {
    VectorNode root;
    int size;
    DestroyFunc destroy;
};

VectorNode create_node(Pointer value, int pos)
{
    VectorNode node = malloc(sizeof(*node));

    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->pos = pos;

    return node;
}

void destroy_node(VectorNode node, DestroyFunc destroy)
{
    if (destroy != NULL)
        destroy(node->value);

    free(node);
}

// Function that finds the direction of the
byte find_direction(int pos)
{
    if (pos == 1)
        return ROOT;

    return pos % 2 ? RIGHT : LEFT;
}

// Function that finds the path from the root to the specified node
// If path == NULL then the given pos is the root
byte* find_path(uint pos)
{
    int height = log2((double)pos);
    if (height == 0)    // Pos == root
        return NULL;

    // The path that need to be taken to get to the root is the height of node long
    byte* path = malloc(sizeof(*path) * height);

    // Cause we start from the target we start to fill up from the end of the array
    for(int i = height-1; i >= 0; i++)
    {
        // The node is a right child if its odd
        path[i] = find_direction(pos);
        pos /= 2;   // New node to check is its parent
    }

    return path;
}

VectorNode get_node_at_pos(Vector vec, int pos)
{
    // Stop if pos is outside of bounds
    assert(pos <= vec->size || pos >= 1);

    byte* path = find_path(vec->size);  // Path to get to the position

    VectorNode node = vec->root; // We set the node to change to the root

    // We change the node to change only if we are not inserting in the root
    if (path != NULL)
    {
        int height = log2((double)vec->size);

        // Change the node to change to the left or right of the current node to change
        for (int i = 0; i < height; i++)
            node = path[i] == LEFT ? node->left : node->right;   
    }
    free(path);

    return node;
}

// Function that finds
VectorNode node_find(VectorNode node, Pointer value, CompareFunc compare)
{
    if (node == NULL)
        return false;
    
    VectorNode res = NULL;
    // Check the left or right subtree at random order
    if (rand()%2)
    {
        res = node_find(node->right, value, compare);
        if (res == NULL)
            res = node_find(node->left, value, compare);
    }
    else
    {
        res = node_find(node->left, value, compare);
        if (res == NULL)
            res = node_find(node->right, value, compare);
    }

    // If we haven't found the value then check the current node
    if (res == NULL)
        res = compare(value, node->value) == 0 ? node : NULL;

    return res;
}

// Destroys subtree with root node
void tree_destroy(VectorNode node, DestroyFunc destroy)
{
    if (node == NULL)
        return;

    tree_destroy(node->left, destroy);
    tree_destroy(node->right, destroy);

    destroy_node(node, destroy);
}

Vector vector_create(int size, DestroyFunc destroy_value)
{
    Vector vec = malloc(sizeof(*vec));

    vec->size = 0;
    vec->destroy = destroy_value;
    vec->root = NULL;

    for (int i = 0; i < size; i++)
        vector_insert_last(vec, NULL);

    return vec;
}

int vector_size(Vector vec)
{
    return vec->size;
}

void vector_insert_last(Vector vec, Pointer value)
{
    vec->size++;
    int pos = vec->size;    // Position to insert to

    VectorNode to_insert = create_node(value, pos);

    if (pos == 1)
        vec->root = to_insert;
    else
    {
        // Find parent of node to insert
        VectorNode parent = get_node_at_pos(vec, pos/2);
        
        // Insert at appropriate direction
        int res = find_direction(pos);
        if (res == LEFT)
            parent->left = to_insert;
        else
            parent->right = to_insert;
    }
}

void vector_remove_last(Vector vec)
{
    int pos = vec->size;    // Position to delete
    vec->size--;

    VectorNode to_delete = vec->root;
    // If we are not deleting the root
    if (pos > 1)
    {
        // Get the parent of node to delete
        VectorNode parent = get_node_at_pos(vec, pos/2);    // Parent of node to delete
        int dir = find_direction(pos);

        // Then set to NULL the child of the parent and set the node to delete
        if (dir == LEFT)
        {
            to_delete = parent->left;
            parent->left = NULL;
        }
        else
        {
            to_delete = parent->right;
            parent->right = NULL;
        }
    }
    // If we are deleting the root then set it to NULL
    else
        vec->root = NULL;
    

    // Destroy node
    destroy_node(to_delete, vec->destroy);
}

Pointer vector_get_at(Vector vec, int pos)
{
    VectorNode node = get_node_at_pos(vec, pos+1);

    return node->value;
}

void vector_set_at(Vector vec, int pos, Pointer value)
{
    VectorNode node = get_node_at_pos(vec, pos+1);

    node->value = value;
}

Pointer vector_find(Vector vec, Pointer value, CompareFunc compare)
{
    // Generate random seed
    srand(time(NULL));
    srand(rand());

    return node_find(vec->root, value, compare)->value;
}

DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value)
{
    DestroyFunc old = vec->destroy;

    vec->destroy = destroy_value;
    return old;
}

void vector_destroy(Vector vec)
{
    tree_destroy(vec->root, vec->destroy);

    free(vec);
}

VectorNode vector_first(Vector vec)
{
    return vec->root;
}

VectorNode vector_last(Vector vec)
{
    return get_node_at_pos(vec, vec->size);
}

VectorNode vector_next(Vector vec, VectorNode node)
{
    if (node->pos == vec->size)
        return VECTOR_EOF;

    return get_node_at_pos(vec, node->pos + 1);
}

VectorNode vector_previous(Vector vec, VectorNode node)
{
    if (node->pos == 1)
        return VECTOR_BOF;

    return get_node_at_pos(vec, node->pos - 1);
}

Pointer vector_node_value(Vector vec, VectorNode node)
{
    return node->value;
}

VectorNode vector_find_node(Vector vec, Pointer value, CompareFunc compare)
{
    srand(time(NULL));
    srand(rand());

    return node_find(vec->root, value, compare);
}