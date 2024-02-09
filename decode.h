#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void decode_rec(node *tree, const char *text, FILE *output) //it goes through the text, and points where to and what to do if it meets ., _, *space*
                                                                   //it will be used in decode function to recursively traverse the binary tree
{
    if(tree == NULL) return;

    if(*text == '\0')
    {
        fputc(tree->character, output);
    }

    else if(*text == '/')
    {
        fputc(' ', output);
    }
        
    else if(*text == '.')
    {
        decode_rec(tree -> left, text + 1, output);
    }
    else if(*text == '_')
    {
        decode_rec(tree -> right, text + 1, output);
    }
}

void decode(const char *text, node *root)//decodes the whole input
{
    FILE *output = fopen ("output.txt", "w");
    if (output == NULL) 
        {
            printf("Failed to open 'output.txt' for writing.\n");
            return;
        }

    char *p;
    while(*text)//the while loop will be evaluated till text equals 0 - till when the null-terminator will be encountered
        {
        p = strchr(text, ' '); //strchr function finds in string text the *space* character and returns the pointer to it
                               //p analyzes the input, till space
        if(p != NULL)
        {
            if(p - text != 0)
            {
                char code[p - text + 1];
                memcpy(code, text, p - text);
                code[p - text]='\0';
                decode_rec(root, code, output); //finds the equivalent of the code in the tree
            }
            text = p + 1;
        }
        else
        {
            decode_rec(root, text, output);//analyzes further the input
            break;
        }
    }
    fclose(output);
    fputc('\n', stdout);
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
    drop_tree_rec(root->left);
    drop_tree_rec(root->right);
}

#endif
