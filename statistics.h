#ifndef STATISTICS_H
#define STATISTICS_H

#include <ctype.h>
#include <stdio.h>

void statistics(const char *text, int is_morse)
{
    if (is_morse) {
        size_t dots = 0;
        size_t dashes = 0;
        for (; *text; ++text) {
            if (*text == '.')
                ++dots;
            else if (*text == '_' || *text == '-')
                ++dashes;
        }
        printf("\nOccurence of dots is: %zu\nOccurence of dashes is: %zu\n", dots, dashes);
        return;
    }

    size_t occurrence[26] = {0};
    for (; *text; ++text) {
        int letter = toupper((unsigned char)*text);
        if (letter >= 'A' && letter <= 'Z')
            ++occurrence[letter - 'A'];
    }
    for (int i = 0; i < 26; ++i) {
        if (occurrence[i] != 0)
            printf("Character %c occured %zu time(s)\n", 'A' + i, occurrence[i]);
    }
}

#endif
