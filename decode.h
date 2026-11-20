#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "output.h"

#ifndef DECODE_H
#define DECODE_H

// The static keyword, when used outside of any function, 
// makes the variable local to the source file, in our case - "decode.h". 
// It cannot be accessed from other files in the same program.

static const char *engtext[] = 
{
    "._",   //A
    "_...", //B
    "_._.", //C
    "_..",  //D
    ".",    //E
    ".._.", //F
    "__.",  //G
    "....", //H
    "..",   //I
    ".___", //J
    "_._",  //K
    "._..", //L
    "__",   //M
    "_.",   //N
    "___",  //O
    ".__.", //P
    "__._", //Q
    "._.",  //R
    "...",  //S
    "_",    //T
    ".._",  //U
    "..._", //V
    ".__",  //W
    "_.._", //X
    "_.__", //Y
    "__..", //Z
};

typedef struct node
{
    char character;
    struct node *left;
    struct node *right;
} node;

static char decode_rec(const node *tree, const char *token, const char *end)
{
    if (tree == NULL)
        return '\0';
    if (token == end)
        return tree->character;
    if (*token == '.')
        return decode_rec(tree->left, token + 1, end);
    if (*token == '_' || *token == '-')
        return decode_rec(tree->right, token + 1, end);
    return '\0';
}

int decode(const char *text, node *root)
{
    FILE *output = fopen("output.txt", "w");
    if (output == NULL) {
        fprintf(stderr, "Failed to open 'output.txt' for writing.\n");
        return 0;
    }

    const char *cursor = text;
    while (*cursor) {
        if (isspace((unsigned char)*cursor)) {
            ++cursor;
            continue;
        }

        const char *token = cursor;
        do {
            ++cursor;
        } while (*cursor && !isspace((unsigned char)*cursor));

        char letter = (*token == '/' && cursor == token + 1)
                      ? ' ' : decode_rec(root, token, cursor);
        if (letter == '\0') {
            fprintf(stderr, "Invalid Morse token at byte %zu.\n",
                    (size_t)(token - text) + 1);
            finish_output(output);
            return 0;
        }
        if (fputc(letter, output) == EOF) {
            finish_output(output);
            return 0;
        }
    }

    if (!finish_output(output)) return 0;
    fputc('\n', stdout);
    return 1;
}

static void insert_rec(node **tree, char letter, const char *text)//recursive function, which analyzes the text, and goes (in tree) to left(if .), right( if _) or prints the letter needed
{
    if(*tree == NULL)
        *tree = calloc(1, sizeof(**tree));
    if(*text == '\0')
        (*tree) -> character = letter;
    else if(*text == '.')
        insert_rec(&(*tree) -> left, letter, text + 1);
    else if(*text == '_')
        insert_rec(&(*tree) -> right, letter, text + 1);
}

//inline suggests to the compiler that it should attempt to insert the 
// function's code at the point where the function is called, rather than 
// generating a function call. The idea is to reduce the overhead of a function 
// call and potentially improve performance.

// This function navigates a binary tree based on the characters in the 'text'
// parameter. It inserts the 'letter' into the tree according to the characters
// in 'text'.
static inline void insert(char letter, const char *text, node *root) 
{
    if(*text == '.')
        insert_rec(&(root -> left), letter, text + 1);
    else if(*text == '_')
        insert_rec(&(root -> right), letter, text + 1);
}

void create_tree(node **root)//function which creates the tree
{
    *root = calloc(1, sizeof(**root));
    int i;
    for(i = 0; i < 26; ++i)
        insert('A' + i, engtext[i], *root);
}


void drop_tree_rec(node *root)//drops the tree recursively, in order to free the memory
{
    if(root){
        drop_tree_rec(root->left);
        drop_tree_rec(root->right);
        free(root);
    }
}

void drop_tree(node *root)
{
    drop_tree_rec(root);
}

#endif
