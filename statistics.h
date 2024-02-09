#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef STATISTICS_H
#define STATISTICS_H

void statistics (char *text)
{
    char engtext[36]={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    int dot = 0;
    int dash = 0;
    int c = 0;

    if (text[0] == '.' || text[0] == '_')
    {
        for (int i=0; i<strlen(text); i++)
        {
            if(text[i]=='.')
            {
                dot++;
            }
            if (text[i]=='_')
            {
            dash++;
            }
        }
        printf("\nOccurence of dots is: %d\nOccurence of dashes is: %d\n", dot, dash);
    }
    else
    {
        int occurence[36];

        for (int i=0; i < 36; i++)
        {
            occurence[i] = 0;
        }

        for(int i = 0; i < strlen(text); i++)
        {
            if(islower(text[i]))
            {
                text[i] = toupper(text[i]);
            }
        }

        for (int i = 0; i < strlen(text); i++)
        {
            if(text[i] >= 'A' && text[i] <= 'Z')
            {
                occurence[text[i] - 65]++;
            }
        }
        for (int i = 0; i < 36; i++)
        {
            if(occurence [i] != 0)
            {
                printf("Character %c occured %d time(s)\n", (i + 65), occurence[i]);
            }
        }
    }
}

#endif