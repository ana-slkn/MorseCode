#include <stdio.h>
#include <string.h>
#include <ctype.h>//contains the functions which will give the possibility to read
//every type of character, because it will convert the lower case to upper case
#include <stdlib.h>

#ifndef ENCODE_H
#define ENCODE_H

void encode (char *text)
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
        // Convert any lowercase letters in the input text to uppercase
        for(int i = 0; i < strlen(text); i++){       
            if(islower(text[i]))
            text[i] = toupper(text[i]);
        }
        
        FILE *output = fopen("output.txt", "w");
        if (output == NULL) {
            printf("Failed to open 'output.txt' for writing.\n");
            return;
        }

        // Iterate through each character in the input text
        for (int i = 0; i < strlen(text); i++)
        {
            // Check if the character matches any English letter or space
            for (int j = 0; j < 37; j++)
            {
                if(text[i] == englishtext[j])
                {
                    // Print and write the corresponding Morse code to 'output.txt'
                    printf("%s", morseCode[j]);
                    fprintf(output, "%s", morseCode[j]);
                }
            }
        }

        fclose(output);
}
#endif