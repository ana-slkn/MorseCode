#include <stdio.h>
#include <string.h>
#include <ctype.h>//contains the functions which will give the possibility to read
//every type of character, because it will convert the lower case to upper case
#include <stdlib.h>
#include "output.h"

#ifndef ENCODE_H
#define ENCODE_H

int encode (const char *text)
{
    char englishtext[27]={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', ' '};
    char morseCode[37][7] = {
        "._ ",     //A
        "_... ",   //B
        "_._. ",   //C
        "_.. ",    //D
        ". ",      //E
        ".._. ",   //F
        "__. ",    //G
        ".... ",   //H
        ".. ",     //I
        ".___ ",   //J
        "_._ ",    //K
        "._.. ",   //L
        "__ ",     //M
        "_. ",     //N
        "___ ",    //O
        ".__. ",   //P
        "__._ ",   //Q
        "._. ",    //R
        "... ",    //S
        "_ ",      //T
        ".._ ",    //U
        "..._ ",   //V
        ".__ ",    //W
        "_.._ ",   //X
        "_.__ ",   //Y
        "__.. ",   //Z
        "/ "       //SPACE
        };

    FILE *output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Failed to open 'output.txt' for writing.\n");
        return 0;
    }
    // Iterate through each character in the input text
    for (size_t i = 0; text[i] != '\0'; i++)
    {
        int letter = toupper((unsigned char)text[i]);
        // Check if the character matches any English letter or space
        for (size_t j = 0; j < sizeof englishtext / sizeof englishtext[0]; j++)
        {
            if(letter == englishtext[j])
            {
                // Print and write the corresponding Morse code to 'output.txt'
                printf("%s", morseCode[j]);
                fprintf(output, "%s", morseCode[j]);
                break;
            }
        }
    }
    return finish_output(output);
}
#endif